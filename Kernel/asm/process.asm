section .text
global createProcess

callWrapper:
    call rdx
    mov rdi,0x11 ;exit
    int 0x80
createProcess:
    push rbp
    mov rbp, rsp
    mov rsp, rdi ; stack base
    and rsp, -16
    push 0x0
    push rdi
    push 0x202
    push 0x8
    push callWrapper
    push rbp
    push rax
    push rbx
    push rdi
    push rsi
    push rdx
    push rcx
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    mov rax, rsp
    mov rsp, rbp
    pop rbp
    ret