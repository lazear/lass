PROG=./lass
NDI=ndisasm

all: compile test

compile:
	 gcc -Wall -std=gnu99 assembler.c -o $(PROG)

clean:
	rm *.o

test:
	$(PROG) new.s bin
	$(NDI) -b 32 output
