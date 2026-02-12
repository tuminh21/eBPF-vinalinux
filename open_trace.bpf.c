// SPDX-License-Identifier: GPL-2.0
// open_trace.bpf.c — eBPF kernel program to trace file I/O syscalls
//
// Traces: openat, read, write, close, stat/fstat
// Filters by multiple PIDs (hash map, set from userspace).
// Reports TTY name so events can be correlated to terminals.
// Events delivered via ring buffer.

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#define TASK_COMM_LEN  16
#define MAX_FILENAME   256
#define MAX_TTY_NAME   32

// -----------------------------------------------------------------
// Event types
// -----------------------------------------------------------------
enum event_type {
    EVENT_OPENAT = 0,
    EVENT_READ   = 1,
    EVENT_WRITE  = 2,
    EVENT_CLOSE  = 3,
    EVENT_STAT   = 4,
    EVENT_FSTAT  = 5,
};

// -----------------------------------------------------------------
// Event structure shared between kernel and userspace
// -----------------------------------------------------------------
struct event {
    __u32 pid;
    __u32 type;              // enum event_type
    __u32 flags;             // openat flags
    __s32 fd;                // file descriptor (read/write/close/fstat)
    __u64 size;              // bytes requested (read/write)
    char  comm[TASK_COMM_LEN];
    char  filename[MAX_FILENAME]; // openat filename or stat path
    char  tty[MAX_TTY_NAME];     // terminal name (e.g. "pts/0")
};

// -----------------------------------------------------------------
// Maps
// -----------------------------------------------------------------

// Hash map: key=PID, value=1 (presence). Up to 16 PIDs.
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16);
    __type(key, __u32);
    __type(value, __u32);
} target_pids SEC(".maps");

// Ring buffer for sending events to userspace (256 KB).
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} events SEC(".maps");

// -----------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------

// Check if current process PID is in our target set.
// Returns the PID if matched, 0 otherwise.
static __always_inline __u32 check_target_pid(void)
{
    __u64 pid_tgid = bpf_get_current_pid_tgid();
    __u32 pid = pid_tgid >> 32;

    __u32 *found = bpf_map_lookup_elem(&target_pids, &pid);
    if (!found)
        return 0;

    return pid;
}

// Read the TTY name from current->signal->tty->name via CO-RE.
static __always_inline void read_tty_name(char *buf, int buflen)
{
    struct task_struct *task = (void *)bpf_get_current_task();

    // task->signal->tty may be NULL (e.g. daemon processes)
    struct signal_struct *sig = BPF_CORE_READ(task, signal);
    if (!sig) {
        buf[0] = '?';
        buf[1] = '\0';
        return;
    }

    struct tty_struct *tty = BPF_CORE_READ(sig, tty);
    if (!tty) {
        buf[0] = '?';
        buf[1] = '\0';
        return;
    }

    BPF_CORE_READ_STR_INTO(buf, tty, name);
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_openat
// -----------------------------------------------------------------
struct sys_enter_openat_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           dfd;
    const char    *filename;
    long           flags;
    long           mode;
};

SEC("tracepoint/syscalls/sys_enter_openat")
int trace_openat(struct sys_enter_openat_args *ctx)
{
    __u32 pid = check_target_pid();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid   = pid;
    e->type  = EVENT_OPENAT;
    e->flags = (__u32)ctx->flags;
    e->fd    = -1;
    e->size  = 0;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    bpf_probe_read_user_str(&e->filename, sizeof(e->filename), ctx->filename);
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_read
// -----------------------------------------------------------------
struct sys_enter_read_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           fd;
    long           buf;
    long           count;
};

SEC("tracepoint/syscalls/sys_enter_read")
int trace_read(struct sys_enter_read_args *ctx)
{
    __u32 pid = check_target_pid();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid   = pid;
    e->type  = EVENT_READ;
    e->flags = 0;
    e->fd    = (__s32)ctx->fd;
    e->size  = (__u64)ctx->count;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_write
// -----------------------------------------------------------------
struct sys_enter_write_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           fd;
    long           buf;
    long           count;
};

SEC("tracepoint/syscalls/sys_enter_write")
int trace_write(struct sys_enter_write_args *ctx)
{
    __u32 pid = check_target_pid();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid   = pid;
    e->type  = EVENT_WRITE;
    e->flags = 0;
    e->fd    = (__s32)ctx->fd;
    e->size  = (__u64)ctx->count;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_close
// -----------------------------------------------------------------
struct sys_enter_close_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           fd;
};

SEC("tracepoint/syscalls/sys_enter_close")
int trace_close(struct sys_enter_close_args *ctx)
{
    __u32 pid = check_target_pid();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid   = pid;
    e->type  = EVENT_CLOSE;
    e->flags = 0;
    e->fd    = (__s32)ctx->fd;
    e->size  = 0;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_newstat  (stat() — takes a path)
// -----------------------------------------------------------------
struct sys_enter_newstat_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    const char    *filename;
    long           statbuf;
};

SEC("tracepoint/syscalls/sys_enter_newstat")
int trace_stat(struct sys_enter_newstat_args *ctx)
{
    __u32 pid = check_target_pid();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid   = pid;
    e->type  = EVENT_STAT;
    e->flags = 0;
    e->fd    = -1;
    e->size  = 0;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    bpf_probe_read_user_str(&e->filename, sizeof(e->filename), ctx->filename);
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_newfstat  (fstat() — takes an fd)
// -----------------------------------------------------------------
struct sys_enter_newfstat_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           fd;
    long           statbuf;
};

SEC("tracepoint/syscalls/sys_enter_newfstat")
int trace_fstat(struct sys_enter_newfstat_args *ctx)
{
    __u32 pid = check_target_pid();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid   = pid;
    e->type  = EVENT_FSTAT;
    e->flags = 0;
    e->fd    = (__s32)ctx->fd;
    e->size  = 0;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
