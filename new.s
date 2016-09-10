BITS 32

main:
mov eax, 1
mov edx, 0x10
add edx, eax
and ecx, edx
mov al, cl
int 0x80 
iret
end:

test eax, ebx
cmp edx, ecx