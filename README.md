# lass - the lightweight assembler

### lass is a lightweight x86 assembler written in C with a focus on portability

lass uses intel syntax, and currently supports nasm style labels (with $ and $$). 

Benefits: bare minimum of library calls: strtok, strcmp, strcpy, etc.

Only a small number of instructions are supported, but with the framework in place, it will be easier to add more.

### to-do:
* Implement second-pass parsing to resolve labels
* Add in more op-codes (write a python script to generate opcode table header files)
