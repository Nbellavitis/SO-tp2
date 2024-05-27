GLOBAL cpuVendor
GLOBAL getKey
GLOBAL getHours
GLOBAL getMinutes
GLOBAL getSeconds
GLOBAL getRegisters

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

getKey:
  push rbp
  mov rbp, rsp
  mov rax, 0
  in al, 0x60       ; lee la TECLA PRESIONADA desde el puerto 60h
_good:  
  mov rsp, rbp 
  pop rbp
  ret

  getHours:
  	push rbp
  	mov rbp, rsp

  	mov al, 4
  	out 0x70, al
  	in al, 0x71

  	mov rsp, rbp
  	pop rbp
  	ret

  getMinutes:
  	push rbp
  	mov rbp, rsp

  	mov al, 2
  	out 0x70, al
  	in al, 0x71

  	mov rsp, rbp
  	pop rbp
  	ret

  getSeconds:
  	push rbp
  	mov rbp, rsp

  	mov al, 0
  	out 0x70, al
  	in al, 0x71

  	mov rsp, rbp
  	pop rbp
  	ret



getRegisters:
	push rbp
	mov rbp,rsp

	mov [rdi], rax
	mov [rdi+8],rbx
	mov [rdi+16],rcx

	push rax
	mov rax,[rbp+8]
	mov [rdi+24],rax
	pop rax
	mov [rdi+32],rsi
	mov [rdi+40],rdi
	mov [rdi+48],rsp
	push rax
	mov rax,[rbp-8]
	mov [rdi+56],rax
	pop rax
	mov [rdi+64],r8
	mov [rdi+72],r9
	mov [rdi+80],r10
	mov [rdi+88],r11
	mov [rdi+96],r12
	mov [rdi+104],r13
	mov [rdi+112],r14
	mov [rdi+120],r15
	pop rdi
	leave
	ret








