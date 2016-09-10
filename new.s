BITS 32

main:
mov eax, 1
mov edx, 0xFF
add edx, eax
int 128

cmp eax, edx
mov eax, 8


