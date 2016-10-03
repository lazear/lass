bits 32

; takes edx as argument
syscall_one:
	mov eax, 1
	int 0x80
	jmp $

main:
	push ebp
	mov ebp, esp
	mov edx, .string
	jmp syscall_one
	pop ebp
	retn

.string:
db "HELLO"
