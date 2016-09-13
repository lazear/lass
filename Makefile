PROG=./lass
NDI=ndisasm

all: compile test

compile:
	 gcc -w -std=gnu99 assembler.c -o $(PROG)

clean:
	rm *.o

test:
	$(PROG) new.s
	$(NDI) -b 32 output
