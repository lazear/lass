BITS 32

main:
mov eax, 1

int 128

cmp eax, end
mov eax, 8
end:
mov eax, edx

