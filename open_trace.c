// SPDX-License-Identifier: GPL-2.0
// open_trace.c — Userspace program for the file I/O tracer
//
// Usage:  sudo ./open_trace <PID1> [PID2] ...
//    or:  sudo ./open_trace --docker <container1> [container2] ...
//
// Loads the BPF object, inserts target PIDs or cgroup IDs into BPF
// maps, attaches all tracepoint programs, and polls the ring buffer.
// In --docker mode, events are labeled with the container name.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <bpf/libbpf.h>
#include <bpf/bpf.h>
#include "open_trace.skel.h"

// -----------------------------------------------------------------
// Must match the definitions in open_trace.bpf.c
// -----------------------------------------------------------------
#define TASK_COMM_LEN  16
#define MAX_FILENAME   256
#define MAX_TTY_NAME   32

enum event_type {
    EVENT_OPENAT = 0,
    EVENT_READ   = 1,
    EVENT_WRITE  = 2,
    EVENT_CLOSE  = 3,
    EVENT_STAT   = 4,
    EVENT_FSTAT  = 5,
};

struct event {
    __u32 pid;
    __u32 type;
    __u32 flags;
    __s32 fd;
    __u64 size;
    __u64 cgroup_id;
    char  comm[TASK_COMM_LEN];
    char  filename[MAX_FILENAME];
    char  tty[MAX_TTY_NAME];
};

// -----------------------------------------------------------------
// Container name resolution: cgroup_id → container name
// -----------------------------------------------------------------
#define MAX_CONTAINERS 16

static struct {
    __u64 cgroup_id;
    char  name[64];
} containers[MAX_CONTAINERS];
static int num_containers = 0;
static int docker_mode = 0;

static void register_container(__u64 cgroup_id, const char *name)
{
    if (num_containers >= MAX_CONTAINERS)
        return;
    containers[num_containers].cgroup_id = cgroup_id;
    strncpy(containers[num_containers].name, name, 63);
    containers[num_containers].name[63] = '\0';
    num_containers++;
}

static const char *lookup_container(__u64 cgroup_id)
{
    for (int i = 0; i < num_containers; i++) {
        if (containers[i].cgroup_id == cgroup_id)
            return containers[i].name;
    }
    return NULL;
}

// -----------------------------------------------------------------
// Resolve Docker container → cgroup ID
//
// 1. Get container PID via docker inspect
// 2. Read /proc/<PID>/cgroup to find the cgroup path
// 3. Stat the cgroup directory to get its inode number
//    (bpf_get_current_cgroup_id() returns the inode of the
//     cgroup v2 unified hierarchy entry)
// -----------------------------------------------------------------
static __u32 get_docker_pid(const char *name)
{
    char cmd[256];
    snprintf(cmd, sizeof(cmd),
             "docker inspect -f '{{.State.Pid}}' %s 2>/dev/null", name);

    FILE *fp = popen(cmd, "r");
    if (!fp)
        return 0;

    char buf[32];
    if (!fgets(buf, sizeof(buf), fp)) {
        pclose(fp);
        return 0;
    }
    pclose(fp);
    return (__u32)atoi(buf);
}

static __u64 get_cgroup_id_for_pid(__u32 pid)
{
    char path[256], line[512];
    snprintf(path, sizeof(path), "/proc/%u/cgroup", pid);

    FILE *f = fopen(path, "r");
    if (!f)
        return 0;

    // Find the cgroup v2 unified entry "0::<path>"
    // or any cgroup v1 entry with "docker" in the path
    char cgroup_path[256] = {0};
    while (fgets(line, sizeof(line), f)) {
        // Remove trailing newline
        char *nl = strchr(line, '\n');
        if (nl) *nl = '\0';

        // Prefer cgroup v2 unified hierarchy: "0::<path>"
        if (strncmp(line, "0::", 3) == 0) {
            strncpy(cgroup_path, line + 3, sizeof(cgroup_path) - 1);
            break;
        }
        // Fallback: cgroup v1 with "docker" in path
        if (strstr(line, "docker") && cgroup_path[0] == '\0') {
            char *third_colon = strchr(line, ':');
            if (third_colon) third_colon = strchr(third_colon + 1, ':');
            if (third_colon)
                strncpy(cgroup_path, third_colon + 1,
                        sizeof(cgroup_path) - 1);
        }
    }
    fclose(f);

    if (cgroup_path[0] == '\0')
        return 0;

    // Try multiple cgroup filesystem paths to find the directory
    // and get its inode number (which equals bpf_get_current_cgroup_id())
    const char *prefixes[] = {
        "/sys/fs/cgroup/unified",     // cgroup v2 on hybrid systems
        "/sys/fs/cgroup",             // cgroup v2 pure
        "/sys/fs/cgroup/memory",      // cgroup v1 memory controller
        "/sys/fs/cgroup/pids",        // cgroup v1 pids controller
        NULL,
    };

    struct stat st;
    char full_path[512];
    for (int i = 0; prefixes[i]; i++) {
        snprintf(full_path, sizeof(full_path), "%s%s",
                 prefixes[i], cgroup_path);
        if (stat(full_path, &st) == 0) {
            return (__u64)st.st_ino;
        }
    }

    return 0;
}

// -----------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------

static const char *type_str(enum event_type t)
{
    switch (t) {
    case EVENT_OPENAT: return "OPENAT";
    case EVENT_READ:   return "READ";
    case EVENT_WRITE:  return "WRITE";
    case EVENT_CLOSE:  return "CLOSE";
    case EVENT_STAT:   return "STAT";
    case EVENT_FSTAT:  return "FSTAT";
    default:           return "???";
    }
}

// Decode common O_* flags into a human-readable string
static const char *decode_flags(__u32 flags)
{
    static char buf[256];
    buf[0] = '\0';

    int accmode = flags & O_ACCMODE;
    if (accmode == O_RDONLY)       strcat(buf, "O_RDONLY");
    else if (accmode == O_WRONLY)  strcat(buf, "O_WRONLY");
    else if (accmode == O_RDWR)   strcat(buf, "O_RDWR");

    if (flags & O_CREAT)     strcat(buf, "|O_CREAT");
    if (flags & O_EXCL)      strcat(buf, "|O_EXCL");
    if (flags & O_TRUNC)     strcat(buf, "|O_TRUNC");
    if (flags & O_APPEND)    strcat(buf, "|O_APPEND");
    if (flags & O_NONBLOCK)  strcat(buf, "|O_NONBLOCK");
    if (flags & O_DIRECTORY) strcat(buf, "|O_DIRECTORY");
    if (flags & O_CLOEXEC)   strcat(buf, "|O_CLOEXEC");

    if (buf[0] == '\0')
        snprintf(buf, sizeof(buf), "0x%x", flags);

    return buf;
}

// Get current timestamp as HH:MM:SS.mmm
static const char *timestamp(void)
{
    static char buf[32];
    struct timespec ts;
    struct tm tm;

    clock_gettime(CLOCK_REALTIME, &ts);
    localtime_r(&ts.tv_sec, &tm);
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d.%03ld",
             tm.tm_hour, tm.tm_min, tm.tm_sec, ts.tv_nsec / 1000000);
    return buf;
}

// -----------------------------------------------------------------
// Global flag for clean shutdown
// -----------------------------------------------------------------
static volatile sig_atomic_t exiting = 0;

static void sig_handler(int sig)
{
    (void)sig;
    exiting = 1;
}

// -----------------------------------------------------------------
// Ring buffer callback
// -----------------------------------------------------------------
static int handle_event(void *ctx, void *data, size_t data_sz)
{
    (void)ctx;

    if (data_sz < sizeof(struct event)) {
        fprintf(stderr, "Warning: event too small (%zu bytes)\n", data_sz);
        return 0;
    }

    const struct event *e = data;
    const char *ts = timestamp();
    const char *tname = type_str(e->type);

    // Determine the source column
    char source[64];
    if (docker_mode) {
        const char *cname = lookup_container(e->cgroup_id);
        if (cname)
            snprintf(source, sizeof(source), "%s", cname);
        else
            snprintf(source, sizeof(source), "cg:%llu",
                     (unsigned long long)e->cgroup_id);
    } else {
        snprintf(source, sizeof(source), "%s",
                 (e->tty[0] && e->tty[0] != '?') ? e->tty : "-");
    }

    // Build the detail column based on event type
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    char detail[512];
    switch (e->type) {
    case EVENT_OPENAT:
        snprintf(detail, sizeof(detail), "flags=%s file=%s",
                 decode_flags(e->flags), e->filename);
        break;
    case EVENT_READ:
        snprintf(detail, sizeof(detail), "fd=%d size=%llu",
                 e->fd, (unsigned long long)e->size);
        break;
    case EVENT_WRITE:
        snprintf(detail, sizeof(detail), "fd=%d size=%llu",
                 e->fd, (unsigned long long)e->size);
        break;
    case EVENT_CLOSE:
        snprintf(detail, sizeof(detail), "fd=%d", e->fd);
        break;
    case EVENT_STAT:
        snprintf(detail, sizeof(detail), "path=%s", e->filename);
        break;
    case EVENT_FSTAT:
        snprintf(detail, sizeof(detail), "fd=%d", e->fd);
        break;
    default:
        detail[0] = '\0';
        break;
    }
#pragma GCC diagnostic pop

    printf("%-12s %-8u %-14s %-16s %-8s %s\n",
           ts, e->pid, source, e->comm, tname, detail);

    return 0;
}

// -----------------------------------------------------------------
// Bump RLIMIT_MEMLOCK (required on older kernels for BPF maps)
// -----------------------------------------------------------------
static int bump_memlock_rlimit(void)
{
    struct rlimit rlim = {
        .rlim_cur = RLIM_INFINITY,
        .rlim_max = RLIM_INFINITY,
    };
    return setrlimit(RLIMIT_MEMLOCK, &rlim);
}

// -----------------------------------------------------------------
// libbpf debug output callback
// -----------------------------------------------------------------
static int libbpf_print_fn(enum libbpf_print_level level,
                            const char *format, va_list args)
{
    if (level == LIBBPF_DEBUG)
        return 0;
    return vfprintf(stderr, format, args);
}

// -----------------------------------------------------------------
// main
// -----------------------------------------------------------------
int main(int argc, char **argv)
{
    struct open_trace_bpf *skel = NULL;
    struct ring_buffer    *rb   = NULL;
    int err;
    int total_targets = 0;

    if (argc < 2) {
        fprintf(stderr,
            "Usage: %s <PID1> [PID2] ...\n"
            "       %s --docker <container1> [container2] ...\n"
            "\n"
            "  PID mode:    trace specific PIDs (by TTY)\n"
            "  Docker mode: trace all processes in Docker containers\n",
            argv[0], argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "--docker") == 0) {
        docker_mode = 1;
        if (argc < 3) {
            fprintf(stderr,
                "Error: --docker requires at least one container name\n");
            return 1;
        }
    }

    // Set up libbpf error/debug callback
    libbpf_set_print(libbpf_print_fn);

    // Bump RLIMIT_MEMLOCK for BPF map memory
    bump_memlock_rlimit();

    // Register signal handlers for clean exit
    signal(SIGINT,  sig_handler);
    signal(SIGTERM, sig_handler);

    // ----------------------------------------------------------
    // 1. Open the BPF skeleton
    // ----------------------------------------------------------
    skel = open_trace_bpf__open();
    if (!skel) {
        fprintf(stderr, "Failed to open BPF skeleton\n");
        return 1;
    }

    // ----------------------------------------------------------
    // 2. Load & verify the BPF programs and maps
    // ----------------------------------------------------------
    err = open_trace_bpf__load(skel);
    if (err) {
        fprintf(stderr, "Failed to load BPF skeleton: %d\n", err);
        goto cleanup;
    }

    // ----------------------------------------------------------
    // 3. Set up targets (PIDs or Docker container cgroups)
    // ----------------------------------------------------------
    if (docker_mode) {
        int cg_map_fd = bpf_map__fd(skel->maps.target_cgroups);
        __u32 val = 1;

        for (int i = 2; i < argc; i++) {
            const char *cname = argv[i];

            __u32 cpid = get_docker_pid(cname);
            if (cpid == 0) {
                fprintf(stderr,
                    "Warning: cannot find container '%s' "
                    "(is it running?)\n", cname);
                continue;
            }

            __u64 cgid = get_cgroup_id_for_pid(cpid);
            if (cgid == 0) {
                fprintf(stderr,
                    "Warning: cannot determine cgroup ID for '%s' "
                    "(PID %u)\n", cname, cpid);
                continue;
            }

            err = bpf_map_update_elem(cg_map_fd, &cgid, &val, BPF_ANY);
            if (err) {
                fprintf(stderr, "Failed to add cgroup for '%s': %s\n",
                        cname, strerror(errno));
                continue;
            }

            register_container(cgid, cname);
            printf("  Tracking %-14s (PID=%u, cgroup_id=%llu)\n",
                   cname, cpid, (unsigned long long)cgid);
            total_targets++;
        }
    } else {
        int pid_map_fd = bpf_map__fd(skel->maps.target_pids);
        __u32 val = 1;

        for (int i = 1; i < argc; i++) {
            __u32 pid = (__u32)atoi(argv[i]);
            if (pid == 0) {
                fprintf(stderr, "Warning: ignoring invalid PID '%s'\n",
                        argv[i]);
                continue;
            }
            err = bpf_map_update_elem(pid_map_fd, &pid, &val, BPF_ANY);
            if (err) {
                fprintf(stderr, "Failed to add PID %u: %s\n",
                        pid, strerror(errno));
                goto cleanup;
            }
            printf("  Tracking PID %u\n", pid);
            total_targets++;
        }
    }

    if (total_targets == 0) {
        fprintf(stderr, "Error: no valid targets to trace\n");
        err = -1;
        goto cleanup;
    }

    // ----------------------------------------------------------
    // 4. Attach the BPF programs to their tracepoints
    // ----------------------------------------------------------
    err = open_trace_bpf__attach(skel);
    if (err) {
        fprintf(stderr, "Failed to attach BPF programs: %d\n", err);
        goto cleanup;
    }

    // ----------------------------------------------------------
    // 5. Create the ring buffer manager
    // ----------------------------------------------------------
    rb = ring_buffer__new(bpf_map__fd(skel->maps.events),
                          handle_event, NULL, NULL);
    if (!rb) {
        fprintf(stderr, "Failed to create ring buffer: %s\n",
                strerror(errno));
        err = -1;
        goto cleanup;
    }

    // ----------------------------------------------------------
    // 6. Poll the ring buffer for events
    // ----------------------------------------------------------
    printf("\nTracing file I/O for %d target(s)... Hit Ctrl-C to stop.\n\n",
           total_targets);
    printf("%-12s %-8s %-14s %-16s %-8s %s\n",
           "TIME", "PID", "SOURCE", "COMM", "TYPE", "DETAILS");
    printf("%-12s %-8s %-14s %-16s %-8s %s\n",
           "------------", "--------", "--------------",
           "----------------", "--------",
           "----------------------------------------");

    while (!exiting) {
        err = ring_buffer__poll(rb, 100 /* timeout ms */);
        if (err == -EINTR) {
            err = 0;
            break;
        }
        if (err < 0) {
            fprintf(stderr, "Error polling ring buffer: %d\n", err);
            break;
        }
    }

cleanup:
    ring_buffer__free(rb);
    open_trace_bpf__destroy(skel);

    printf("\nDone.\n");
    return err < 0 ? 1 : 0;
}
