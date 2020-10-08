[bits 32]
global read_port
global write_port

read_port:
    push    ebp
    mov     ebp, esp
    sub     esp, 1
    mov     dx, [ebp + 8]
    in      al, dx
    push    eax
    leave
    ret

write_port:
    push    ebp
    mov     ebp, esp
    mov     dx, [ebp + 8]
    mov     al, [ebp + 12]
    out     dx, al
    leave
    ret
