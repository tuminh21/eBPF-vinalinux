// SPDX-License-Identifier: GPL-2.0
// open_trace.bpf.c — eBPF kernel program to trace file I/O + network syscalls
//
// File I/O:  openat, read, write, close, stat, fstat
// Network:   socket, connect, accept4, bind, sendto, recvfrom
// Filters by multiple PIDs (hash map) or cgroup IDs (for Docker).
// Events delivered via ring buffer.

#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

#define TASK_COMM_LEN  16
#define MAX_FILENAME   256
#define MAX_TTY_NAME   32

#define AF_INET   2
#define AF_INET6  10

// -----------------------------------------------------------------
// Event types
// -----------------------------------------------------------------
enum event_type {
    // File I/O
    EVENT_OPENAT  = 0,
    EVENT_READ    = 1,
    EVENT_WRITE   = 2,
    EVENT_CLOSE   = 3,
    EVENT_STAT    = 4,
    EVENT_FSTAT   = 5,
    // Network
    EVENT_SOCKET  = 6,
    EVENT_CONNECT = 7,
    EVENT_ACCEPT  = 8,
    EVENT_BIND    = 9,
    EVENT_SENDTO  = 10,
    EVENT_RECVFROM= 11,
};

// -----------------------------------------------------------------
// Event structure shared between kernel and userspace
// -----------------------------------------------------------------
struct event {
    __u32 pid;
    __u32 type;              // enum event_type
    __u32 flags;             // openat flags; socket: family<<16|type
    __s32 fd;                // file descriptor
    __u64 size;              // bytes requested (read/write/sendto/recvfrom)
    __u64 cgroup_id;         // cgroup ID (for container identification)
    __u16 af;                // address family (AF_INET / AF_INET6)
    __u16 port;              // network port (host byte order)
    __u32 addr4;             // IPv4 address (network byte order)
    __u8  addr6[16];         // IPv6 address (network byte order)
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

// Hash map: key=cgroup_id, value=1 (presence). For Docker containers.
struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 16);
    __type(key, __u64);
    __type(value, __u32);
} target_cgroups SEC(".maps");

// Ring buffer for sending events to userspace (256 KB).
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} events SEC(".maps");

// -----------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------

// Check if current process matches our targets (by PID or cgroup).
static __always_inline __u32 check_target(void)
{
    __u64 pid_tgid = bpf_get_current_pid_tgid();
    __u32 pid = pid_tgid >> 32;

    __u32 *found = bpf_map_lookup_elem(&target_pids, &pid);
    if (found)
        return pid;

    __u64 cgid = bpf_get_current_cgroup_id();
    __u32 *cg_found = bpf_map_lookup_elem(&target_cgroups, &cgid);
    if (cg_found)
        return pid;

    return 0;
}

// Read the TTY name from current->signal->tty->name via CO-RE.
static __always_inline void read_tty_name(char *buf, int buflen)
{
    struct task_struct *task = (void *)bpf_get_current_task();

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

// Check if an fd refers to a regular file (not pipe/socket/tty/proc).
static __always_inline int is_regular_file(int fd)
{
    if (fd < 0)
        return 0;

    struct task_struct *task = (void *)bpf_get_current_task();

    struct files_struct *files = BPF_CORE_READ(task, files);
    if (!files)
        return 0;

    struct fdtable *fdt = BPF_CORE_READ(files, fdt);
    if (!fdt)
        return 0;

    unsigned int max_fds = BPF_CORE_READ(fdt, max_fds);
    if ((unsigned int)fd >= max_fds)
        return 0;

    struct file **fd_array = BPF_CORE_READ(fdt, fd);
    struct file *f = NULL;
    bpf_probe_read_kernel(&f, sizeof(f), &fd_array[fd]);
    if (!f)
        return 0;

    struct inode *inode = BPF_CORE_READ(f, f_inode);
    if (!inode)
        return 0;

    unsigned short i_mode = BPF_CORE_READ(inode, i_mode);
    return (i_mode & 0170000) == 0100000;
}

// Parse a userspace sockaddr pointer and store address info in event.
// Reads sa_family, then IPv4 or IPv6 address and port.
static __always_inline void parse_sockaddr(struct event *e,
                                            const void *uaddr,
                                            int addrlen)
{
    if (!uaddr || addrlen < 2) {
        e->af = 0;
        e->port = 0;
        e->addr4 = 0;
        return;
    }

    // Read just the family first (2 bytes)
    __u16 family = 0;
    bpf_probe_read_user(&family, sizeof(family), uaddr);
    e->af = family;

    if (family == AF_INET && addrlen >= 8) {
        // struct sockaddr_in layout: family(2) + port(2) + addr(4)
        __u16 port_be = 0;
        bpf_probe_read_user(&port_be, 2, uaddr + 2);
        e->port = __builtin_bswap16(port_be);

        bpf_probe_read_user(&e->addr4, 4, uaddr + 4);
    } else if (family == AF_INET6 && addrlen >= 28) {
        // struct sockaddr_in6 layout: family(2) + port(2) + flowinfo(4) + addr(16)
        __u16 port_be = 0;
        bpf_probe_read_user(&port_be, 2, uaddr + 2);
        e->port = __builtin_bswap16(port_be);

        bpf_probe_read_user(&e->addr6, 16, uaddr + 8);
    } else {
        e->port = 0;
        e->addr4 = 0;
    }
}

// ================================================================
//  FILE I/O TRACEPOINTS
// ================================================================

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
    __u32 pid = check_target();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_OPENAT;
    e->flags     = (__u32)ctx->flags;
    e->fd        = -1;
    e->size      = 0;
    e->cgroup_id = bpf_get_current_cgroup_id();
    e->af        = 0;
    e->port      = 0;
    e->addr4     = 0;

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
    __u32 pid = check_target();
    if (!pid)
        return 0;

    if (!is_regular_file((__s32)ctx->fd))
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_READ;
    e->flags     = 0;
    e->fd        = (__s32)ctx->fd;
    e->size      = (__u64)ctx->count;
    e->cgroup_id = bpf_get_current_cgroup_id();
    e->af        = 0;
    e->port      = 0;
    e->addr4     = 0;

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
    __u32 pid = check_target();
    if (!pid)
        return 0;

    if (!is_regular_file((__s32)ctx->fd))
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_WRITE;
    e->flags     = 0;
    e->fd        = (__s32)ctx->fd;
    e->size      = (__u64)ctx->count;
    e->cgroup_id = bpf_get_current_cgroup_id();
    e->af        = 0;
    e->port      = 0;
    e->addr4     = 0;

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
    __u32 pid = check_target();
    if (!pid)
        return 0;

    if (!is_regular_file((__s32)ctx->fd))
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_CLOSE;
    e->flags     = 0;
    e->fd        = (__s32)ctx->fd;
    e->size      = 0;
    e->cgroup_id = bpf_get_current_cgroup_id();
    e->af        = 0;
    e->port      = 0;
    e->addr4     = 0;

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
    __u32 pid = check_target();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_STAT;
    e->flags     = 0;
    e->fd        = -1;
    e->size      = 0;
    e->cgroup_id = bpf_get_current_cgroup_id();
    e->af        = 0;
    e->port      = 0;
    e->addr4     = 0;

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
    __u32 pid = check_target();
    if (!pid)
        return 0;

    if (!is_regular_file((__s32)ctx->fd))
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_FSTAT;
    e->flags     = 0;
    e->fd        = (__s32)ctx->fd;
    e->size      = 0;
    e->cgroup_id = bpf_get_current_cgroup_id();
    e->af        = 0;
    e->port      = 0;
    e->addr4     = 0;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// ================================================================
//  NETWORK TRACEPOINTS
// ================================================================

// -----------------------------------------------------------------
// Tracepoint: sys_enter_socket
// -----------------------------------------------------------------
struct sys_enter_socket_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           family;
    long           type;
    long           protocol;
};

SEC("tracepoint/syscalls/sys_enter_socket")
int trace_socket(struct sys_enter_socket_args *ctx)
{
    __u32 pid = check_target();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_SOCKET;
    // Pack family and socket type into flags for userspace decoding
    e->flags     = ((__u32)ctx->family << 16) | ((__u32)ctx->type & 0xFFFF);
    e->fd        = -1;  // fd not known yet (returned by syscall)
    e->size      = 0;
    e->cgroup_id = bpf_get_current_cgroup_id();
    e->af        = (__u16)ctx->family;
    e->port      = 0;
    e->addr4     = 0;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_connect
// -----------------------------------------------------------------
struct sys_enter_connect_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           fd;
    const void    *uservaddr;
    long           addrlen;
};

SEC("tracepoint/syscalls/sys_enter_connect")
int trace_connect(struct sys_enter_connect_args *ctx)
{
    __u32 pid = check_target();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_CONNECT;
    e->flags     = 0;
    e->fd        = (__s32)ctx->fd;
    e->size      = 0;
    e->cgroup_id = bpf_get_current_cgroup_id();

    parse_sockaddr(e, ctx->uservaddr, (__s32)ctx->addrlen);

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_accept4
// -----------------------------------------------------------------
struct sys_enter_accept4_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           fd;
    const void    *upeer_sockaddr;
    long           upeer_addrlen;
    long           flags;
};

SEC("tracepoint/syscalls/sys_enter_accept4")
int trace_accept(struct sys_enter_accept4_args *ctx)
{
    __u32 pid = check_target();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_ACCEPT;
    e->flags     = 0;
    e->fd        = (__s32)ctx->fd;  // listening socket fd
    e->size      = 0;
    e->cgroup_id = bpf_get_current_cgroup_id();
    // Peer addr is not available at entry time (filled on exit)
    e->af        = 0;
    e->port      = 0;
    e->addr4     = 0;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_bind
// -----------------------------------------------------------------
struct sys_enter_bind_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           fd;
    const void    *umyaddr;
    long           addrlen;
};

SEC("tracepoint/syscalls/sys_enter_bind")
int trace_bind(struct sys_enter_bind_args *ctx)
{
    __u32 pid = check_target();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_BIND;
    e->flags     = 0;
    e->fd        = (__s32)ctx->fd;
    e->size      = 0;
    e->cgroup_id = bpf_get_current_cgroup_id();

    parse_sockaddr(e, ctx->umyaddr, (__s32)ctx->addrlen);

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_sendto
// -----------------------------------------------------------------
struct sys_enter_sendto_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           fd;
    long           buff;
    long           len;
    long           flags;
    const void    *addr;
    long           addr_len;
};

SEC("tracepoint/syscalls/sys_enter_sendto")
int trace_sendto(struct sys_enter_sendto_args *ctx)
{
    __u32 pid = check_target();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_SENDTO;
    e->flags     = 0;
    e->fd        = (__s32)ctx->fd;
    e->size      = (__u64)ctx->len;
    e->cgroup_id = bpf_get_current_cgroup_id();

    // sendto may or may not have a dest addr (NULL for connected sockets)
    if (ctx->addr)
        parse_sockaddr(e, ctx->addr, (__s32)ctx->addr_len);
    else {
        e->af   = 0;
        e->port = 0;
        e->addr4 = 0;
    }

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

// -----------------------------------------------------------------
// Tracepoint: sys_enter_recvfrom
// -----------------------------------------------------------------
struct sys_enter_recvfrom_args {
    unsigned short common_type;
    unsigned char  common_flags;
    unsigned char  common_preempt_count;
    int            common_pid;
    int            __syscall_nr;
    long           fd;
    long           ubuf;
    long           size;
    long           flags;
    const void    *addr;
    long           addr_len;
};

SEC("tracepoint/syscalls/sys_enter_recvfrom")
int trace_recvfrom(struct sys_enter_recvfrom_args *ctx)
{
    __u32 pid = check_target();
    if (!pid)
        return 0;

    struct event *e = bpf_ringbuf_reserve(&events, sizeof(*e), 0);
    if (!e)
        return 0;

    e->pid       = pid;
    e->type      = EVENT_RECVFROM;
    e->flags     = 0;
    e->fd        = (__s32)ctx->fd;
    e->size      = (__u64)ctx->size;
    e->cgroup_id = bpf_get_current_cgroup_id();
    // Source addr not available at entry time (filled on exit)
    e->af        = 0;
    e->port      = 0;
    e->addr4     = 0;

    bpf_get_current_comm(&e->comm, sizeof(e->comm));
    e->filename[0] = '\0';
    read_tty_name(e->tty, sizeof(e->tty));

    bpf_ringbuf_submit(e, 0);
    return 0;
}

char LICENSE[] SEC("license") = "GPL";
