BITS 32
main:
mov eax, 1
mov edx, 15
sub edx, eax
and edx, eax

mov ecx, edx
inc ecx

sub eax, ecx
cmp eax, eax
end:
mov eax, end
jmp eax