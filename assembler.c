#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <assert.h>
#include "encoding.h"
#include <ctype.h>

uint32_t program_length = 0;
uint32_t current_position = 0;
uint32_t num_symbols = 0;
uint32_t unr_symbols = 0;
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
	int nelem = sizeof(registers)/sizeof(struct syntax);
	for (int i = 0; i < nelem; i++) {
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

#define G(a, b) a[b], a[b+1], a[b+2], a[b+3]

int add_symbol(char* label) {
//	printf("add symbol: %s @ 0x%x\n", label, current_position);
	printf("%08x %s:\n", current_position, label);
	for (int i = 0; i < 32 && i < strlen(label); i++) {
		symtab[num_symbols].name[i] = label[i];
	}
	symtab[num_symbols].position = current_position;

	for (int i = 0; i < unr_symbols; i++) {
		if (strncmp(label, unresolved[i].name, strlen(unresolved[i].name)) == 0) {
			/* Added a previously unresolved symbol, so we need to go back and 
			replace the dummy value with the real label position */
			int dummy = unresolved[i].position;
			for (int i = dummy; i < dummy + 4; i+=2) {
				uint16_t word = (output[i+1] << 8 | output[i]);
				if (word == dummy) {
					output[i] = current_position & 0xFF;
					output[i+1] = (current_position >> 8)& 0xFF;
				}
			}
		}
	}
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

	/* At this point, the symbol is unresolved, so we add it to the unresolved
	symbol table. We're going to handle this by */
	unresolved[unr_symbols].position = current_position;
	for (int i = 0; i < 32 && i < strlen(label); i++)
			unresolved[unr_symbols].name[i]  = label[i];
	unr_symbols++;

	return current_position;
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
	printf("%08x %-20s\t",current_position, line);	
	char* pch = strtok(line, " ,");		// split line by spaces and commas
	char* inst;							// store instruction
	uint8_t syn[3];	
	int count, valid = 0;
	int immediate, jmp_to = -1;
	int mode = MOD_REG;		// default to MOD 11b


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
		if (q <0 && count == 0) {
			printf("INSTRUCTION NOT SUPPORTED: %s\n", pch);
			return;
		}
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
				//printf("Label @ 0x%x\t", jmp_to);
			}
		}
		
		count++;
		pch = strtok(NULL, " ,");		// get next token
	}

	/* All tokens have been removed from string now */
	int opcode 	= syn[0];
	int operand = syn[1];
	int s = 1;
	int w = 1;
	int m = 0;				// mod-rm

	if (valid == 0) {
		printf("NO VALID INSTRUCTION FOUND\n");
		return;
	}
	if (valid < count || valid == 2) {
		int ext = wextension(inst);
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
				if (jmp_to >= 0) {	/* Jump SHORT, 8 bits displacement only*/
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
			case INT:
				m = immediate;
				s = 0; 
				w = 0;
				immediate = -1;
				break;
			default: 
				break;
		}
		write_word(OPCODE(opcode, s, w) | \
			((m > 0) ? m : MODRM(mode, ext, operand)) << 8);


		if (immediate >= 0 || jmp_to >= 0) {
			(s == 1) ? \
			((opcode == 0x80) ? write_byte(immediate) : write_dword(immediate))\
			: write_word(immediate);
			printf("%x %04x\n", OPCODE(opcode, s, w) << 8 | \
			((m > 0) ? m : MODRM(mode, ext, operand)), ENDIAN(immediate)>>16);
		} else
			printf("%x\n", OPCODE(opcode, s, w) << 8 | \
			((m > 0) ? m : MODRM(mode, ext, operand)));

	} 	else {
		/* Everything was recognized as instructions or registers */
		write_word(OPCODE(opcode, s, w) | \
		((m > 0) ? m : MODRM(mode, operand, syn[2])) << 8);
		printf("%x %x\n", OPCODE(opcode, s, w), MODRM(mode, operand, syn[2]));
	}
	return 1;
}

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
	printf("Read %d bytes from %s\n", sz, fname);


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
			if (strchr(*lines, ':')) {
				char* c = strchr(*lines, ':');
				*c = '\0';					// remove the semi colon
				add_symbol(*lines);
			}
			else 
				parse_line(*lines);
		}
	}

	FILE* fd = fopen("output", "wb");
	assert(fd);
	fwrite(output, 1, current_position, fd);
	fflush(fd);
	free(buffer);
	free(start);
	printf("Wrote %d bytes to %s\n", current_position, "output" );
	return 0;
}