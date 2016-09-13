bits 32
main:
	mov ecx, [ebp]
	mov eax, [ebp + eax * 8]
	mov [ebp-8], eax
	mov eax, 0x10
	dec ecx
