# SM-F966B / F966BXXS8AZC2 — kernel provenance

Firmware:  SM-F966B (Galaxy Z Fold7), region XSA
Build:    BP2A.250605.031.A3 / F966BXXS8AZC2 / F966BOXM8AZB6
Android:  16 (SDK 36)
SoC:      Qualcomm Snapdragon 8 Gen 3 "for Galaxy" (SM8650, Kera)
Kernel:   Linux 6.6.57-android15-8-31566393-abogkiF966BXXS8AZC2-4k
          compiled 2026-03-06 with Clang 18 (PGO+BOLT+LTO+MLGO), LLD 18

Source AP:  AP_F966BXXS8AZC2_F966BXXS8AZC2_MQB107195377_REV00_user_low_ship_MULTI_CERT_meta_OS16.tar.md5
Source BL:  BL_F966BXXS8AZC2_F966BXXS8AZC2_MQB107195377_REV00_user_low_ship_MULTI_CERT.tar.md5
Source CSC: CSC_OXM_F966BOXM8AZB6_MQB106570015_REV00_user_low_ship_MULTI_CERT.tar.md5

boot.img size:      101_122_048
boot.img SHA-256:   8A28857C4717DF39FB645D3B8EF94E2E75A5683D99BC865D8DF2735713BB4CBB
kernel size:           37_440_000
kernel SHA-256:      479C088FCA4CAA0DFC0F799F166E3E8470C46CBA167C709548CBB6D0A1217FDC
ARM64 Image:         text_offset=0x0  image_size=0x2610000  flags=0xa
KIMAGE_TEXT_BASE:    0xffffffc080000000
P0_PHYS_OFFSET:      0x80000000
P0_KERNEL_PHYS_LOAD: 0x80000000 (text_offset + P0_PHYS_OFFSET)

Recovered vmlinux.elf via vmlinux-to-elf.
Symbols extracted with NDK r29 llvm-nm.
BTF extracted from raw Image at [0x179e990, 0x1d7f99d) (6_164_493 bytes).
Kernel config recovered from embedded IKCONFIG.

Qualcomm ABL/XBL bootloader — no Samsung sboot present.
P0_PHYS_OFFSET confirmed via ARM64 Image header text_offset=0
and Qualcomm Snapdragon convention (DRAM base at 0x80000000).
