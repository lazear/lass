PROG=./asm
NDI=ndisasm

all: compile test

compile:
	 gcc -w -std=gnu99 assembler.c -o asm

clean:
	rm *.o

test:
	$(PROG) new.s
	$(NDI) output