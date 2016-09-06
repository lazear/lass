#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <assert.h>

typedef struct _reg {
	char name[10];
	uint8_t opcode;
} reg;

typedef struct opcode {

}opcode;

#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7

/* 
Zero: [eax]
One: [eax+ disp8]
Four: [eax+ disp32]
Reg: eax */
#define MOD_ZERO	0
#define MOD_ONE		1
#define MOD_FOUR	2
#define MOD_REG		3

#define ADD 		0

#define IMM 		(1<<7)

/*
x86 opcode structure

opcode:
Bit 0 (1<<0) == 1 specific 16/32 bit. Set to zero is 8 bit
Bit 1 (1<<1) == 2, is direction flag
d = 0(dest = memory location) add [ebx], al d= 1 add al, [ebx]

d = 0, REG is source 		RM is dest
d = 1, REG is destination 	RM is source

|	1-2		0-1		0-1		0-4		0-4			Bytes
|Opcode|Mod-R/M| SIB   | Displace|Immediate|



*/

#define OPCODE(code, d, s) (code | ((d & 1)<<1) | (s & 1))
#define MODRM(mod, reg, rm)	(((mod & 0x3) << 6)| ((reg & 0x7) << 3) | (rm & 0x7))

int main(int argc, char* argv[]) {
	if (argc < 2)
		return -1;
	char* fname = argv[1];

	int fp = open(fname, O_RDWR, 0444);
	assert(fp);

	int sz = lseek(fp, 0, SEEK_END);		// seek to end of file
	if (sz < 1)
		return -1;

	lseek(fp, 0, SEEK_SET);		// back to beginning

	char* buffer = malloc(sz);	// File buffer
	char** lines = malloc(sz);

	pread(fp, buffer, sz, 0);
	printf("%s %d\n", fname, sz);
//	puts(buffer);

	*lines = buffer;
	uint32_t start = lines;
	for (int i = 0; i < sz; i++) {
		if (buffer[i] == '\n') {
			buffer[i] = '\0';
			*(lines++) = (uint32_t) buffer + i + 1;
		}
	}

	for(lines = start; *lines; lines++) {
		if (**lines != ';')	{	// Remove comment-only lines
			if (strchr(*lines, ':'))
				printf("LABEL: %s\n", *(lines));
			else if ( strchr(*lines, ','))
				printf("COMMA: %s\n",*lines);
			else
				printf("OTHER: %s\n", *(lines));
		}
	}

// Adding immediates: d=0 means constant is same size as reg \
	d = 1, means sign extension \
	s =0, 8 bit/ s=1, 32 bit \
	REG set to 000, opcode extension for add 
	printf("%x %x\n", OPCODE(ADD | IMM, 1, 1), MODRM(MOD_REG, EAX, EDX));
	printf("%x %x\n", OPCODE(ADD | IMM, 1, 1), MODRM(MOD_REG, 0, EDX));

	return 1;
}