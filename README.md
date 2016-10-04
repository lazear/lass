# lass - the lightweight assembler

## USE AT YOUR OWN RISK - THIS IS A WORK IN PROGRESS

### lass is a lightweight two-pass x86 assembler written in C with a focus on portability

lass uses intel syntax, and currently supports nasm style labels (with $ and $$). 

Benefits: bare minimum of library calls: strtok, strcmp, strcpy, etc.

A majority of the intel instruction set is supported, with complete support being the goal.

### to-do:
* Fix SIB addressing mode (mov eax, [ebp + eax*2 + 8], etc etc)
