bits 32
main:
	mov ecx, [ebp]
	mov eax, [ebp + eax * 8]
	push gs
	push fs
	push es
	pop es
	pop fs
	pop gs