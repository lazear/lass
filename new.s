bits 32
main:
push ebp
mov ebp, esp

mov ecx, [ebx+4]

push ds
push es 
push fs
push gs

xor eax, eax
mov gs, ax
mov fs, ax
mov es, ax
mov ds, ax

sub esp, 12
mov [ebx + 8], eax
mov ebx, [8 + ebp * 4]