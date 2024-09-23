GLOBAL call_sys_read
GLOBAL call_sys_write
GLOBAL call_timeClock
GLOBAL call_setFontSize
GLOBAL call_printRegisters
GLOBAL call_clear
GLOBAL InvalidOpasm
GLOBAL call_getHeight
GLOBAL call_getWidth
GLOBAL call_moveCursorX
GLOBAL call_moveCursorY
GLOBAL call_drawRectangle
GLOBAL call_sleepms
GLOBAL call_beep
GLOBAL call_getTicks
GLOBAL testmm
GLOBAL mmStatus
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
call_getTicks:
    call_to_handler 6
call_getHeight:
    call_to_handler 7
call_getWidth:
    call_to_handler 8
call_moveCursorX:
    call_to_handler 9
call_moveCursorY:
    call_to_handler 10
call_drawRectangle:
    call_to_handler 11
call_sleepms:
    call_to_handler 12
call_setFontSize:
    call_to_handler 13
call_beep:
    call_to_handler 14
testmm:
    call_to_handler 15
mmStatus:
    call_to_handler 16

InvalidOpasm:
    mov rax, 0x12345678
    rdpmc
    ret