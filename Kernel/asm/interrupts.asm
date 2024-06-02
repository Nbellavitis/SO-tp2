
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq80Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL getRegisters
GLOBAL printRegistersAsm
GLOBAL getFlag
GLOBAL saveRegisters

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN retUserland
EXTERN getStackBase
EXTERN clear
EXTERN printRegisters
SECTION .text


%macro saveRegistersState 0
	push rax
	mov rax,[rsp + 8]
	mov [registers],rax
	pop rax
	mov [registers+8],rax
	mov [registers+16],rbx
	mov [registers+24],rcx
	mov [registers+32],rdx
	mov [registers+40],rsi
	mov [registers+48],rdi
	mov [registers+56],rbp
	mov [registers+64],rsp
	mov [registers+72],r8
	mov [registers+80],r9
	mov [registers+88],r10
	mov [registers+96],r11
	mov [registers+104],r12
	mov [registers+112],r13
	mov [registers+120],r14
	mov [registers+128],r15

%endmacro

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

saveRegisters:
	mov byte[flag],1
	saveRegistersState
	ret


getRegisters:
    mov rax, registers
    mov byte[flag],0

    ret

getFlag:
	movzx rax, byte[flag]
	ret

%macro exceptionHandler 1
	;pushState
	;Guardo el estado del registros
	saveRegistersState

	;llamo a funcion en c que imprime los registros
	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher
	;popState

	call getStackBase
	mov [rsp+24], rax
    mov rax, userland
    mov [rsp], rax

   	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4

;USB
_irq05Handler:
	irqHandlerMaster 5


_irq80Handler:

 	push rbx
	push r12
	push r13
	push r14
	push r15
    push rbp
    mov rbp,rsp

	push r9

    mov r9, r8
    mov r8, rcx
    mov rcx, rdx
    mov rdx, rsi
    mov rsi, rdi
    mov rdi, 0x80

    call irqDispatcher

    pop r9
    mov rsp, rbp
    pop rbp
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx

    iretq


;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

_exception6Handler:
	exceptionHandler 6



SECTION .data
userland equ 0x400000

SECTION .bss
   	aux resq 1
	registers resq 20
	flag resq 1

