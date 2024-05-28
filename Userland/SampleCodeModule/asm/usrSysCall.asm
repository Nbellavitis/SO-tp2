GLOBAL call_sys_read
GLOBAL call_sys_write
GLOBAL call_timeClock
GLOBAL call_setFontSize
GLOBAL call_printRegisters
GLOBAL call_clear
GLOBAL InvalidOpasm
GLOBAL call_printTitle
section .text

%macro call_to_handler 1
    push rbp
    mov rbp, rsp
    mov r9, r8      ;arg 5
    mov r8, rcx     ;arg 4
    mov rcx, rdx    ;arg 3
    mov rdx, rsi    ;arg 2
    mov rsi, rdi    ;arg 1
    mov rdi, %1     ;syscall deseada
    int 0x80      ;interrupcion 80
    mov rsp, rbp
	pop rbp
    ret
%endmacro

call_sys_write:
    call_to_handler 1
call_sys_read:
    call_to_handler 2
call_timeClock:
    call_to_handler 3
call_printRegisters:
    call_to_handler 4
call_clear:
    call_to_handler 5
call_printTitle:
    call_to_handler 6
call_setFontSize:
    call_to_handler 13
InvalidOpasm:
    mov rax, 0x12345678
    rdpmc
    ret