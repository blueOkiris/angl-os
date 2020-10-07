; Stage 2 - Bootloader
[bits 32]
global start
extern _ZN4angl6kernel4mainEv

; This is needed to use grub
section .mbHeader
align 0x4
MODULEALIGN     equ 1 << 0
MEMINFO         equ 1 << 1
FLAGS           equ MODULEALIGN | MEMINFO
MAGIC           equ 0x1BADB002
CHECKSUM        equ -(MAGIC + FLAGS)
MultiBootHeader:
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; Actual loader
start:
    push    ebx                     ; Load grub's structure
    call    _ZN4angl6kernel4mainEv  ; C++'s angl::kernel::main
