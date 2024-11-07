GLOBAL callSysRead
GLOBAL callSysWrite
GLOBAL callTimeClock
GLOBAL callSetFontSize
GLOBAL callPrintRegisters
GLOBAL callClear
GLOBAL invalidOpAsm
GLOBAL callGetHeight
GLOBAL callGetWidth
GLOBAL callMoveCursorX
GLOBAL callMoveCursorY
GLOBAL callDrawRectangle
GLOBAL callSleepMs
GLOBAL callBeep
GLOBAL callGetTicks
GLOBAL testMM
GLOBAL mmStatus
GLOBAL createProcess
GLOBAL killProcess
GLOBAL blockProcess
GLOBAL unblockProcess
GLOBAL allocMemory
GLOBAL freeMemory
GLOBAL changePrio
GLOBAL getMyPid
GLOBAL yield
GLOBAL semOpen
GLOBAL semWait
GLOBAL semPost
GLOBAL semClose
GLOBAL testeando
GLOBAL ps
GLOBAL waitpid
GLOBAL pipeOpen
GLOBAL pipeClose
GLOBAL pipeRead
GLOBAL pipeWrite
GLOBAL pipeOpenAnon
GLOBAL getMyFds
section .text

%macro callToHandler 1
    push rbp
    mov rbp, rsp
    mov r9, r8       ; arg 5
    mov r8, rcx      ; arg 4
    mov rcx, rdx     ; arg 3
    mov rdx, rsi     ; arg 2
    mov rsi, rdi     ; arg 1
    mov rdi, %1      ; desired syscall
    int 0x80         ; interrupt 80
    mov rsp, rbp
	pop rbp
    ret
%endmacro

callSysWrite:
    callToHandler 1
callSysRead:
    callToHandler 2
callTimeClock:
    callToHandler 3
callPrintRegisters:
    callToHandler 4
callClear:
    callToHandler 5
callGetTicks:
    callToHandler 6
callGetHeight:
    callToHandler 7
callGetWidth:
    callToHandler 8
callMoveCursorX:
    callToHandler 9
callMoveCursorY:
    callToHandler 10
callDrawRectangle:
    callToHandler 11
callSleepMs:
    callToHandler 12
callSetFontSize:
    callToHandler 13
callBeep:
    callToHandler 14
testMM:
    callToHandler 15
mmStatus:
    callToHandler 16
createProcess:
    callToHandler 18
killProcess:
    callToHandler 19
blockProcess:
     callToHandler 20
unblockProcess:
    callToHandler 21
allocMemory:
    callToHandler 22
freeMemory:
    callToHandler 23
changePrio:
    callToHandler 24
getMyPid:
    callToHandler 25
testeando:
    callToHandler 26
ps:
    callToHandler 27
waitpid:
    callToHandler 28
yield:
    callToHandler 29
semOpen:
    callToHandler 30
semWait:
    callToHandler 31
semPost:
    callToHandler 32
semClose:
    callToHandler 33
pipeOpen:
    callToHandler 34
pipeClose:
    callToHandler 35
pipeRead:
    callToHandler 36
pipeWrite:
    callToHandler 37
pipeOpenAnon:
    callToHandler 38
getMyFds:
    callToHandler 39
invalidOpAsm:
    mov rax, 0x12345678
    rdpmc
    ret
