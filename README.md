# lass - the lightweight assembler

### Unmaintained project

### lass is a lightweight two-pass x86 assembler written in C with a focus on portability

lass uses intel syntax, and currently supports nasm style labels (with $ and $$). 

Benefits: 
* bare minimum of library calls: strtok, strcmp, strcpy, etc.
* Single C file, one header (+ elf header if you want ELF executables)
* Operates on a char[] buffer, so you can implement whatever kind of input you want

Drawbacks:
* It's not complete
* Doesn't support full NASM syntax yet (macros, "equ", resb)

A majority of the intel instruction set is supported, with complete support being the goal.

### to-do:
* Fix SIB addressing mode (mov eax, [ebp + eax*2 + 8], etc etc)
