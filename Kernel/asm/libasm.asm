GLOBAL cpuVendor
GLOBAL getKey
GLOBAL getHours
GLOBAL getMinutes
GLOBAL getSeconds

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



