# Makefile for open_trace — eBPF openat() tracer (libbpf CO-RE)
#
# Targets:
#   make          — build everything
#   make clean    — remove generated files
#   make vmlinux  — regenerate vmlinux.h only

CLANG   ?= clang
LLC     ?= llc
BPFTOOL ?= bpftool
CC      ?= gcc

# Architecture for BPF CO-RE (adjust if not x86_64)
ARCH := $(shell uname -m | sed 's/x86_64/x86/' \
                           | sed 's/aarch64/arm64/' \
                           | sed 's/ppc64le/powerpc/' \
                           | sed 's/mips.*/mips/' \
                           | sed 's/riscv64/riscv/' \
                           | sed 's/loongarch64/loongarch/')

# Path to locally built libbpf (v1.5.0)
LIBBPF_SRC   := ../libbpf/src
LIBBPF_A     := $(LIBBPF_SRC)/libbpf.a

# BPF compilation flags
BPF_CFLAGS := -g -O2 -target bpf \
              -D__TARGET_ARCH_$(ARCH) \
              -I. -I$(LIBBPF_SRC)

# Userspace compilation flags (use local libbpf headers + static lib)
USER_CFLAGS  := -g -O2 -Wall -I. -I$(LIBBPF_SRC)
USER_LDFLAGS := $(LIBBPF_A) -lelf -lz

# ---------------------------------------------------------------
# Phony targets
# ---------------------------------------------------------------
.PHONY: all clean vmlinux

all: open_trace

clean:
	rm -f open_trace open_trace.bpf.o open_trace.skel.h

vmlinux:
	$(BPFTOOL) btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h

# ---------------------------------------------------------------
# Step 1: Compile the BPF C program to BPF object
# ---------------------------------------------------------------
open_trace.bpf.o: open_trace.bpf.c vmlinux.h
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

# ---------------------------------------------------------------
# Step 2: Generate the BPF skeleton header from the object
# ---------------------------------------------------------------
open_trace.skel.h: open_trace.bpf.o
	$(BPFTOOL) gen skeleton $< > $@

# ---------------------------------------------------------------
# Step 3: Compile the userspace program (includes skeleton)
# ---------------------------------------------------------------
open_trace: open_trace.c open_trace.skel.h
	$(CC) $(USER_CFLAGS) -o $@ $< $(USER_LDFLAGS)
