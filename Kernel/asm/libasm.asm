GLOBAL cpuVendor
GLOBAL getKey

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
	mov rbp,rsp 
	;Armado del stack frame

	in al, 0x60;Lee la tecla presionada
	movzx eax, al; guarda en eax el byte, y extiende con ceros

	;Desarmado del stack frame
	mov rsp,rbp
	pop rbp
	ret