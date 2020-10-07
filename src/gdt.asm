section .text
global flushGdt

flushGdt:
    push    ebp         ; Save old base pointer
    mov     ebp, esp    ; Move the stack pointer into it

    ; Load the gdt with the special pointer
    lgdt    [ebp + 6]   ; The "pointer" is a struct of size 2*8 + 4*8 = 6 * 8
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    jmp     0x08:flush2

flush2:
    pop     ebp         ; restore base pointer for return
    ret