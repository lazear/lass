#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <assert.h>
#include "encoding.h"
#include <ctype.h>


#define M(e) (#e)

char* upper(char* s) {
	for (int i = 0; i < strlen(s); i++)
		s[i] = toupper(s[i]);
	return s;
}

struct symbol {
	char name[32];
	uint32_t position;
} symtab[256];

typedef struct syntax {
	char name[10];
	int code;

} syntax;
syntax instructions[] = {
	{ "add", ADD },
	{ "and", AND },
	{ "inc", INC },
	{ "dec", DEC },
	{ "sub", SUB },
	{ "ret", RET },
	{ "int", INT },
	{ "mov", MOV }, 
	{ "jmp", JMP }
}, registers[] = {
	{ "eax", EAX },
	{ "ecx", ECX },
	{ "edx", EDX },
	{ "ebx", EBX },
	{ "esp", ESP },
	{ "ebp", EBP },
	{ "esi", ESI },
	{ "edi", EDI },
};

uint32_t program_length = 0;
uint32_t current_position = 0;
uint32_t num_symbols = 0;
uint8_t* buffer = NULL;

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

int winstruction(char* inst) {
	int nelem = sizeof(instructions)/sizeof(struct syntax);
	for (int i = 0; i < nelem; i++) {
		if (strcmp(inst, instructions[i].name) == 0) {
			printf("\tMATCH: %s %x\n", inst, instructions[i].code);
			return instructions[i].code;
		}
	}
	return -1;
}

int wregister(char* r) {
	for (int i = 0; i < 8; i++) {
		if (strcmp(r, registers[i].name) == 0) {
			printf("\tMATCH: %s %x\n", r, registers[i].code);
			return registers[i].code;
		}
	}
	return -1;
}

int add_symbol(char* label) {
	for (int i = 0; i < 32 && i < strlen(label); i++) {
		symtab[num_symbols].name[i] = label[i];
	}
	symtab[num_symbols].position = current_position;

	printf("New symbol %s @ 0x%x\n", symtab[num_symbols].name, current_position);
		num_symbols++;
}


int parse_line(char* line) {
	printf("NEW LINE: %s\n", line);						// how many items in line?
	char * pch = strtok(line, " ,");	// split line by spaces and commas

	uint8_t syn[3];
	int count = 0;
	int valid = 0;
	int immediate = 0;

	while(pch) {
		int q = winstruction(pch);

		if (q < 0)
			switch(*pch) {
				case 'e': {	// is this a register maybe?
		 			q = wregister(pch);
					break;
				}
				case '[':	// memory location

					break;

			}				

		if (q >= 0) 		// This is an instruction or a register
			syn[valid++] = q;
		else 			// This is an immediate? or Unknown
			immediate = atoi(pch, 16);
		
		count++;
		pch = strtok(NULL, " ,");		// get next token
	}
	uint32_t dword = ENDIAN(*(uint32_t*) &syn);

	if (valid < count) {
		printf("%x\n", immediate);
	} else {

	}
	//free(s);
	return 1;
}

int main(int argc, char* argv[]) {
	if (argc < 2)
		return -1;
	char* fname = argv[1];
	buffer = malloc(1024);
	int fp = open(fname, O_RDWR, 0444);
	assert(fp);

	int sz = lseek(fp, 0, SEEK_END);		// seek to end of file
	if (sz < 1)
		return -1;

	lseek(fp, 0, SEEK_SET);		// back to beginning

	char* buffer = malloc(sz);	// File buffer
	char** lines = malloc(sz);

	pread(fp, buffer, sz, 0);
	printf("lass: %s %d bytes\n", fname, sz);


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
				add_symbol(*lines);
			else if ( strchr(*lines, ',')) {
				parse_line(*lines);
			}
			else 
				parse_line(*lines);
		}
	}

	free(buffer);
	return 1;
}