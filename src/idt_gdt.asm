[bits 32]
global gdt_flush
global idt_flush

gdt_flush:
    mov     eax, [esp + 4]      ; Pointer to GDT pointer
    lgdt    [eax]

    ; Load all data segment selectors
    mov     ax, 0x10            ; 0x10 - offset to data segment
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    jmp     0x08:.flush         ; 0x08 - Code segment
.flush:
    ret

idt_flush:
    mov     eax, [esp + 4]      ; pointer to Idt pointer
    lidt    [eax]
    ret
