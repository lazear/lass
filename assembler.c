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
	{ "jmp", JMP },
	{ "cmp", CMP }
}, extensions[] = {
	{ "add", ADD_EXT },
	{ "and", AND_EXT },
	{ "inc", INC_EXT },
	{ "dec", DEC_EXT },
	{ "sub", SUB_EXT },
	{ "ret", RET_EXT },
	{ "int", INT_EXT },
	{ "mov", MOV_EXT }, 
	{ "jmp", 4 },
	{ "cmp", CMP_EXT }
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
uint8_t output[1024];

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
		//	printf("\tMATCH: %s %x\n", inst, instructions[i].code);
			return instructions[i].code;
		}
	}
	return -1;
}

int wregister(char* r) {
	for (int i = 0; i < 8; i++) {
		if (strcmp(r, registers[i].name) == 0) {
		//	printf("\tMATCH: %s %x\n", r, registers[i].code);
			return registers[i].code;
		}
	}
	return -1;
}

int wextension(char* r) {
	for (int i = 0; i < 8; i++) {
		if (strcmp(r, extensions[i].name) == 0) {
		//	printf("\tMATCH: %s %x\n", r, extensions[i].code);
			return extensions[i].code;
		}
	}
	return -1;
}

int add_symbol(char* label) {
	for (int i = 0; i < 32 && i < strlen(label)-1; i++) {
		symtab[num_symbols].name[i] = label[i];
	}
	symtab[num_symbols].position = current_position;

	printf("New symbol %s @ 0x%x\n", symtab[num_symbols].name, current_position);
		num_symbols++;
}

int find_symbol(char* label) {
	for (int i = 0; i < num_symbols; i++)
		if (strcmp(symtab[i].name, label) == 0)
			return symtab[i].position;
	return -1;
}

uint32_t mov_immediate(char reg, uint32_t n) {
	int w = (n > 0xFFFF) ? 1 : 0;
	w = 1;	// force 32 bit
	uint32_t out = (0xB0 | (w<<3) | reg) << 16 | n << 8;	// alternate encoding
	uint32_t op = OP(MOV_IMM, 1, 1, MOD_REG, ONE_OP, reg) << 16 | n << 8;
	printf("%x:%x\n", out, op);
}


int parse_line(char* line) {
	printf("NEW LINE: %s\n", line);						// how many items in line?
	char * pch = strtok(line, " ,");	// split line by spaces and commas
	char* inst;
	uint8_t syn[3];
	int count = 0;
	int valid = 0;
	int immediate = 0;
	int jmp_to = -1;
	while(pch) {
		int q = winstruction(pch);

		if (q < 0) {
			switch(*pch) {
				case 'e': {	// is this a register maybe?
		 			q = wregister(pch);
					break;
				}
				case '[':	// memory location

					break;
				default:
					
					break;

			}
		} else {
			inst = pch;
		}

		if (q >= 0) 		// This is an instruction or a register
			syn[valid++] = q;
		else {			// This is an immediate? or Unknown
			if (isdigit(*pch))
				immediate = atoi(pch, 16);
			else
				jmp_to = find_symbol(pch);
		}
		
		count++;
	//	printf("%s INSTRUCTION %x REGISTER %x SYMBOL %x\n", pch, winstruction(pch), wregister(pch), find_symbol(pch) );
		pch = strtok(NULL, " ,");		// get next token
	}

	if (jmp_to != -1) printf("Jmp to %x\n", jmp_to);
	if (valid < count || valid == 2) {
		printf("ONE OPERAND ONLY\n");
		switch(syn[0]) {
			int opcode = syn[0];
			int ext = wextension(inst);

			case MOV: {
				output[current_position++] = OPCODE(MOV_IMM, 1, 1);
				output[current_position++] = MODRM(MOD_REG, MOV_IMM_EXT, syn[1]);
				if (jmp_to != -1)
					immediate = jmp_to;
				output[current_position++] = immediate & 0xFF;
				output[current_position++] = (immediate >> 8)& 0xFF;
				output[current_position++] = (immediate >> 16)& 0xFF;
				output[current_position++] = (immediate >> 24)& 0xFF;
				break;
			} case INC: {
				if (strcmp(inst, "inc") == 0) {
					output[current_position++] = OPCODE(INC, 1, 1);
					output[current_position++] = MODRM(MOD_REG, ext, syn[1]);
				} else {
					output[current_position++] = OPCODE(INC, 1, 1);
					output[current_position++] = MODRM(MOD_REG, ext, syn[1]);
				}
				break;
			} case JMP :{
					if (jmp_to >= 0) {
					output[current_position++] = OPCODE(JMP, 1, 1);
					printf("Jmp to %x\n", jmp_to);
				} else 
					output[current_position++] = OPCODE(0xFF, 1, 1);
					output[current_position++] = MODRM(MOD_REG, 4, syn[1]);
				break;
			}
			default: {
				output[current_position++] = OPCODE(syn[0], 1, 1);
				output[current_position++] = MODRM(MOD_REG, wextension(inst), syn[1]);
				output[current_position++] = immediate & 0xFF;
				output[current_position++] = (immediate >> 8)& 0xFF;
				output[current_position++] = (immediate >> 16)& 0xFF;
				output[current_position++] = (immediate >> 24)& 0xFF;
				break;
			}
		}

	} else {
		//printf("%x", OP1(syn[0], 1, 1, MOD_REG, syn[1], syn[2]));
		output[current_position++] = OPCODE(syn[0], 1, 1) & 0xFF;
		output[current_position++] = MODRM(MOD_REG, syn[1], syn[2]) & 0xFF;
		printf("%x %x\n", OPCODE(syn[0], 1, 1), MODRM(MOD_REG, syn[1], syn[2]) );
	}

	return 1;
}

int main(int argc, char* argv[]) {
	if (argc < 2)
		return -1;
	char* fname = argv[1];
//	buffer = malloc(1024);
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
		if (**lines != ';' && **lines)	{	// Remove comment-only lines
			if (strchr(*lines, ':'))
				add_symbol(*lines);
			else if ( strchr(*lines, ',')) {
				parse_line(*lines);
			}
			else 
				parse_line(*lines);
		}
	}
	FILE* fd = fopen("output", "wb");
	puts(output);
	fwrite(output, 1, current_position, fd);
	fflush(fd);
	fclose(fd);
	printf("find end: %x\n", find_symbol("main"));
//	free(buffer);
	return 1;
}