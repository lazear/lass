#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <assert.h>
#include "encoding.h"
#include "opcodes.h"
#include <ctype.h>
#include <limits.h>

uint32_t program_length = 0;
uint32_t current_position = 0;
uint32_t num_symbols = 0;
uint32_t unr_symbols = 0;
uint8_t output[1024];


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

#define ishex(c)	(((tolower(c)) <= 'f') & ((tolower(c)) >= '0'))


int err = 0;

long long lass_atoi(char* s) {
	long long num = 0;
	int sign = 1;
	int base = 10;
	char* d = strpbrk(s, "x");
	if (d != NULL) {
		base = 16;
		s = d + 1;
	}

	if (s[0] == '-')
		sign = -1;

	for (int i = 0; i < strlen(s) && s[i] != '\0'; i++) {
		if (isdigit(s[i]))
			num = num*base + (s[i] - '0');
		else if (ishex(s[i]))
			num = num*base + (tolower(s[i]) - 'a') + 10;
	}
	return sign*num;
}

#define r32 	0x10
#define r8 		0x20
#define imm8 	0x40
#define imm32	0x80
#define crn 	0x100
#define rel8 	0x200
#define rel32 	0x400


isa* find_instruction(char* s, int op1, int op2) {
	for (int i = 0; i < sizeof(x86)/sizeof(isa); i++) {
		if (strcmp(x86[i].name, s) == 0) {
			if ((x86[i].op1 & op1) && ((x86[i].op2 && op2) || (x86[i].op2 == none))) {
				printf("found %s 0x%x\n", s, x86[i].primary);
				return &x86[i];
			}
			if ((x86[i].op1 == none) && (x86[i].op2 == none)) {
				printf("found %s 0x%x\n", s, x86[i].primary);
				return &x86[i];
			}
		}
	}
	return NULL;
}

int classify(char* s) {
	if (isdigit(*s)) {
		long long num = lass_atoi(s);
		if ( abs(num) > UCHAR_MAX) 
			return imm32;
		else
			return imm8;
	}
	for (int i = 0; i < sizeof(registers)/sizeof(syntax); i++)
		if (strcmp(registers[i].name, s) == 0)
			return registers[i].code | r32;
	for (int i = 0; i < sizeof(half)/sizeof(syntax); i++)
		if (strcmp(half[i].name, s) == 0)
			return half[i].code | r8;

	return -1;
}

int parse_line(char* line) {
	//printf("%08x %-20s\n",current_position, line);	

	char* displacement = NULL;
	char* disp = strpbrk(line, "[");

	if (disp)
		displacement = strdup(disp);

	char* pch = strtok(line, " ,");		// split line by spaces and commas
	char* inst;							// store instruction

	uint64_t syn[3] = { 0, 0, 0};
	int count = 0;
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
		if (pch == disp && disp) {
			// Do something
			printf("%s\n", displacement);
			classify(displacement);
			break;
		}
		printf("\t%s\n", pch);

		if (count == 0) {
			inst = strdup(pch);
			syn[count] = 0;
		}
		else {
			syn[count] = classify(pch);
		}
		count++;
		pch = strtok(NULL, " ,");		// get next token
	}
	printf("%s %x %x\n", inst, syn[1], syn[2]);
	find_instruction(inst, syn[1], syn[2]);
	free(displacement);


}

// int parse_line(char* line) {
// 	printf("%08x %-20s\t",current_position, line);	
// 	char* pch = strtok(line, " ,");		// split line by spaces and commas
// 	char* inst;							// store instruction
// 	uint8_t syn[3];	
// 	int count, valid = 0;
// 	int immediate, jmp_to = -1;
// 	int mode = MOD_REG;		// default to MOD 11b


// 	if (!strcmp(pch, "db")) {

// 		char* pch = strtok(NULL, " ");
// 		char b = *pch;
// 		b = (isdigit(b) == 1) ? atoi(b) : b;
// 		//printf("db: %d val\n", b);
// 		output[current_position++] = b;
// 		return;
// 	}

// 	while(pch) {
// 		int q = winstruction(pch);
// 		if (q <0 && count == 0) {
// 			printf("INSTRUCTION NOT SUPPORTED: %s\n", pch);
// 			return;
// 		}
// 		if (q < 0) {	/* Not a valid instruction */
// 			switch(*pch) {
// 				case 'e': {	// is this a register maybe?
// 		 			q = wregister(pch);
// 					break;
// 				}
// 				case '[':	// memory location
// 					mode = MOD_ZERO;
// 					break;
// 				default:
// 					break;
// 			}
// 		} else {		/* This is an instruciton */
// 			inst = pch;
// 		}

// 		if (q >= 0) {	/* This is an instruction or a register */
// 			syn[valid++] = q;
// 		} else {		/* Immediate, label? */

// 			if (isdigit(*pch))
// 				immediate = atoi(pch, 16);		// Immediate
// 			else {
// 				jmp_to = find_symbol(pch);	// Label
// 				immediate = jmp_to;
// 				//printf("Label @ 0x%x\t", jmp_to);
// 			}
// 		}
		
// 		count++;
// 		pch = strtok(NULL, " ,");		// get next token
// 	}

// 	/* All tokens have been removed from string now */
// 	int opcode 	= syn[0];
// 	int operand = syn[1];
// 	int s = 1;
// 	int w = 1;
// 	int m = 0;				// mod-rm

// 	if (valid == 0) {
// 		printf("NO VALID INSTRUCTION FOUND\n");
// 		return;
// 	}
// 	if (valid < count || valid == 2) {
// 		int ext = wextension(inst);
// 		switch(opcode) {
// 			case ADD: case OR: case ADC: case SBB: case AND: case SUB: case XOR: case CMP:
// 				opcode = 0x80;
// 				s = 1;			// add byte (signed?)	
// 				break;
// 			case MOV: {
// 				// Move is unique because it has a bunch of disparate opcodes
// 				opcode = MOV_IMM;
// 				s = 0;
// 				break;
// 			} 
// 			case JMP :{
// 				if (jmp_to >= 0) {	/* Jump SHORT, 8 bits displacement only*/
// 					int offset = (immediate - current_position - 2);
// 					opcode = 0xE9;
// 					m = (offset & 0xFF);
// 					immediate = -1;
// 					jmp_to = -1;
// 				} else {	/* Jump ABSOLUTE */
// 					opcode = 0xFF;
// 				}
// 				break;
// 			}
// 			case INT:
// 				m = immediate;
// 				s = 0; 
// 				w = 0;
// 				immediate = -1;
// 				break;
// 			default: 
// 				break;
// 		}
// 		write_word(OPCODE(opcode, s, w) | \
// 			((m > 0) ? m : MODRM(mode, ext, operand)) << 8);


// 		if (immediate >= 0 || jmp_to >= 0) {
// 			(s == 1) ? \
// 			((opcode == 0x80) ? write_byte(immediate) : write_dword(immediate))\
// 			: write_word(immediate);
// 			printf("%x %04x\n", OPCODE(opcode, s, w) << 8 | \
// 			((m > 0) ? m : MODRM(mode, ext, operand)), ENDIAN(immediate)>>16);
// 		} else
// 			printf("%x\n", OPCODE(opcode, s, w) << 8 | \
// 			((m > 0) ? m : MODRM(mode, ext, operand)));

// 	} 	else {
// 		/* Everything was recognized as instructions or registers */
// 		write_word(OPCODE(opcode, s, w) | \
// 		((m > 0) ? m : MODRM(mode, operand, syn[2])) << 8);
// 		printf("%x %x\n", OPCODE(opcode, s, w), MODRM(mode, operand, syn[2]));
// 	}
// 	return 1;
// }

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