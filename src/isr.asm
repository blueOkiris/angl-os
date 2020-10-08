[bits 32]

extern _ZN4angl6kernel3Isr7handlerENS0_11RegisterSetE

isr_common_stub:
    pusha

    mov     ax, ds
    push    eax

    mov     ax, 0x10    ; Data segment
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    
    call _ZN4angl6kernel3Isr7handlerENS0_11RegisterSetE

    pop     eax
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax

    popa
    add     esp, 8
    sti
    iret

; Define macros for isrs either with/without error codes
%macro ISR_NO_CODE 1
global isr%1
isr%1:
    cli
    push byte 0
    push byte %1
    jmp isr_common_stub
%endmacro

%macro ISR_ERROR_CODE 1
global isr%1
isr%1:
    cli
    push byte %1
    jmp isr_common_stub
%endmacro

ISR_NO_CODE 0
ISR_NO_CODE 1
ISR_NO_CODE 2
ISR_NO_CODE 3
ISR_NO_CODE 4
ISR_NO_CODE 5
ISR_NO_CODE 6
ISR_NO_CODE 7
ISR_ERROR_CODE 8
ISR_NO_CODE 9
ISR_ERROR_CODE 10
ISR_ERROR_CODE 11
ISR_ERROR_CODE 12
ISR_ERROR_CODE 13
ISR_ERROR_CODE 14
ISR_NO_CODE 15
ISR_NO_CODE 16
ISR_NO_CODE 17
ISR_NO_CODE 18
ISR_NO_CODE 19
ISR_NO_CODE 20
ISR_NO_CODE 21
ISR_NO_CODE 22
ISR_NO_CODE 23
ISR_NO_CODE 24
ISR_NO_CODE 25
ISR_NO_CODE 26
ISR_NO_CODE 27
ISR_NO_CODE 28
ISR_NO_CODE 29
ISR_NO_CODE 30
ISR_NO_CODE 31
