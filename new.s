BITS 32
main:
mov eax, 37
mov edx, 15
sub edx, eax
and edx, eax

mov ecx, edx
dec ecx
sub eax, ecx
cmp eax, ebx
end:

mov eax, end
jmp edx

