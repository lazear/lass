#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <assert.h>
#include "encoding.h"
#include <ctype.h>



uint32_t program_length = 0;
uint32_t current_position = 0;
uint32_t num_symbols = 0;
uint8_t output[1024];

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
		int nelem = sizeof(extensions)/sizeof(struct syntax);
	for (int i = 0; i < nelem; i++) {
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

	//printf("New symbol %s @ 0x%x\n", symtab[num_symbols].name, current_position);
		num_symbols++;
}

int find_symbol(char* label) {
	for (int i = 0; i < num_symbols; i++)
		if (strcmp(symtab[i].name, label) == 0)
			return symtab[i].position;
	if (strcmp(label, "$$") == 0)
		return 0;
	if (strcmp(label, "$") == 0)
		return current_position;
	return -1;
}

void write_byte(uint32_t x) {
	output[current_position++] = x & 0xFF;
}

void write_word(uint32_t x) {
	output[current_position++] = x & 0xFF;
	output[current_position++] = (x >> 8)& 0xFF;
}

void write_dword(uint32_t x) {
	output[current_position++] = x & 0xFF;
	output[current_position++] = (x >> 8)& 0xFF;
	output[current_position++] = (x >> 16)& 0xFF;
	output[current_position++] = (x >> 24)& 0xFF;
}

int parse_line(char* line) {
	printf("%-20s\t\t", line);	
	char* dup = malloc(strlen(line));
	strcpy(dup, line);
	char* pch = strtok(line, " ,");		// split line by spaces and commas
	char* inst;							// store instruction
	uint8_t syn[3];	
	int count = 0;
	int valid = 0;
	int immediate = -1;
	int jmp_to = -1;
	int mode = MOD_REG;					// default to MOD 11b

	if (!strcmp(pch, "db")) {

		char* pch = strtok(NULL, " ");
		char b = *pch;
		b = (isdigit(b) == 1) ? atoi(b) : b;
		//printf("db: %d val\n", b);
		output[current_position++] = b;
		return;
	}

	while(pch) {
		int q = winstruction(pch);

		if (q < 0) {	/* Not a valid instruction */
			switch(*pch) {
				case 'e': {	// is this a register maybe?
		 			q = wregister(pch);
					break;
				}
				case '[':	// memory location
					mode = MOD_ZERO;
					break;
				default:
					break;
			}
		} else {		/* This is an instruciton */
			inst = pch;
		}

		if (q >= 0) {	/* This is an instruction or a register */
			syn[valid++] = q;
		} else {		/* Immediate, label? */

			if (isdigit(*pch))
				immediate = atoi(pch, 16);		// Immediate
			else {
				jmp_to = find_symbol(pch);	// Label
				immediate = jmp_to;
						printf("Label @ 0x%x\t", jmp_to);
			}
		}
		
		count++;
		pch = strtok(NULL, " ,");		// get next token
	}

	/* All tokens have been removed from string now */
	if (valid < count || valid == 2) {
		//printf("ONE OPERAND ONLY\n");
		int opcode 	= syn[0];
		int operand = syn[1];
		int ext = wextension(inst);
		printf("inst %s %d %d\n", inst, ext, immediate);

		int s = 1;
		int w = 1;
		int m = 0;

		switch(opcode) {
			case ADD: case OR: case ADC: case SBB: case AND: case SUB: case XOR: case CMP:
				opcode = 0x80;
				s = 1;			// add byte (signed?)	
				break;
			case MOV: {
				// Move is unique because it has a bunch of disparate opcodes
				opcode = MOV_IMM;
				s = 0;
				break;
			} 
			case JMP :{
				if (jmp_to >= 0) {	/* Jump SHORT */
					int offset = (immediate - current_position - 2);
					opcode = 0xE9;
					m = (offset & 0xFF);
					immediate = -1;
					jmp_to = -1;
				} else {	/* Jump ABSOLUTE */
					opcode = 0xFF;
				}
				break;
			}
			default: 
				break;
		}
		write_word(OPCODE(opcode, s, w) | ((m > 0) ? m : MODRM(mode, ext, operand)) << 8);
		if (immediate >= 0 || jmp_to >= 0) {
			(s == 1) ? ((opcode == 0x80) ? write_byte(immediate) : write_dword(immediate)) : write_word(immediate);
		}

	} else {
		//printf("%x", OP1(syn[0], 1, 1, mode, syn[1], syn[2]));
		output[current_position++] = OPCODE(syn[0], 1, 1) & 0xFF;
		output[current_position++] = MODRM(mode, syn[1], syn[2]) & 0xFF;
		printf("%x %x\n", OPCODE(syn[0], 1, 1), MODRM(MOD_REG, syn[1], syn[2]) );
	}
	return 1;
}

int main(int argc, char* argv[]) {
	syntax s[4] = {GENSYN(2)};
	printf("%s %d\n",s[0].name, s[0].code );
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
	fwrite(output, 1, current_position, fd);
	fflush(fd);
	fclose(fd);
	free(buffer);
	free(start);
	return 0;
}