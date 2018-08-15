bits 32

; takes edx as argument
syscall_one:
	mov eax, 1
	int 0x80
	pop ebp
	retn

main:
	push ebp
	mov ebp, esp
	mov edx, .string
	jmp syscall_one

	mov eax, [ebp+4]

.string:
db "HELLO"
