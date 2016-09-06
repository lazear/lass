
all:
	 gcc -w -std=gnu99 assembler.c -o asm

clean:
	rm *.o
