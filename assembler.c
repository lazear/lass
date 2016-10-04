/*
  GNU nano 2.7.0                                                                                                   File: LICENSE                                                                                                             

MIT License

Copyright (c) 2016 Michael Lazear

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/


#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <assert.h>

#include "opcodes.h"
#include "elf.h"
#include <ctype.h>
#include <limits.h>

uint32_t program_length = 0;
uint32_t current_position = 0;
uint32_t num_symbols = 0;
uint32_t unr_symbols = 0;
uint8_t* output;
uint8_t* elf_output;
uint32_t elf_offset = 0;

void make_elf() {
	printf("Making elf header\n");
	elf_offset = sizeof(elf32_ehdr) + sizeof(elf32_phdr);
	elf32_ehdr* e = malloc(sizeof(elf32_ehdr));
	e->e_ident[0] = ELF_MAGIC;
	e->e_ident[1] = 0x00010101;
	e->e_ident[2] = 0;
	e->e_ident[3] = 0;
	e->e_type = ET_EXEC;
	e->e_machine = 3;
	e->e_version = 1;
	e->e_entry = find_symbol("main");
	e->e_phoff = sizeof(elf32_ehdr);
	e->e_shoff = 0;
	e->e_flags = 0;
	
	e->e_ehsize = sizeof(elf32_ehdr);
	e->e_phentsize = sizeof(elf32_phdr) * 1;
	e->e_phnum = 1;
	e->e_shentsize = 0;
	e->e_shnum = 0;
	e->e_shstrndx = 0;
	
	elf32_phdr* p = malloc(sizeof(elf32_phdr));
	p->p_type = 1;
	p->p_offset = 0;
	p->p_vaddr = 0;
	p->p_paddr = 0;
	p->p_filesz = program_length +elf_offset;
	printf("p_filesz: %d bytes\n", p->p_filesz);
	p->p_memsz = p->p_filesz;
	p->p_flags = 0x5;
	p->p_align = 0x1000;

	elf_output = malloc(elf_offset);
	memcpy(elf_output, e, sizeof(elf32_ehdr));
	memcpy(elf_output + sizeof(elf32_ehdr), p, sizeof(elf32_phdr));

}

int add_symbol(char* label) {
	printf("%08x %s:\n", current_position, label);
	for (int i = 0; i < 32 && i < strlen(label); i++) {
		symtab[num_symbols].name[i] = label[i];
	}
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

	symtab[num_symbols++].position = current_position;
}


int find_symbol(char* label) {
	for (int i = 0; i < num_symbols; i++)
		if (strcmp(symtab[i].name, label) == 0)
			return symtab[i].position + elf_offset;
	if (strncmp(label, "$$", 2) == 0)
		return 0;
	if (strcmp(label, "$") == 0)
		return current_position + elf_offset;

	unresolved[unr_symbols].position = current_position;
	//printf("unres sym %s @ 0x%x \n", label, current_position);
	for (int i = 0; i < 32 && i < strlen(label); i++)
			unresolved[unr_symbols].name[i]  = label[i];
	unr_symbols++;
	return current_position;
}

/* functions to output in little-endian */
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

int log2(int s) {
	int i = 0;
	while (s /= 2)
		i++;
	return i;
}

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


isa* find_instruction(char* s, int op1, int op2) {
	for (int i = 0; i < sizeof(x86)/sizeof(isa); i++) {
		if (strcmp(x86[i].name, s) == 0) {
			if (op1 & sreg) {
				if (( x86[i].op1 == op1) && ((x86[i].op2 == op2) || (x86[i].op2 == none))) 
					return &x86[i];
			} else {
				if (((x86[i].op1 & op1) == x86[i].op1) && ((x86[i].op2 == op2) || (x86[i].op2 == none))) 
					return &x86[i];
				if ((x86[i].op1 == none) && (x86[i].op2 == none)) 
					return &x86[i];
			}
		}
	}
	/* Second loop - if we have an imm8 or rel8, but there is no designated
	imm8/rel8 function, use the 32 bit version. This simply saves some output space,
	and copies the format that NASM uses */
	for (int i = 0; i < sizeof(x86)/sizeof(isa); i++) 
		if (strcmp(x86[i].name, s) == 0) 
			if (((x86[i].op1 & op1) == x86[i].op1) && ((x86[i].op2 & ( (op2 & (imm8 | rel8)) ? 0x6C0 : op2)) || (x86[i].op2 == none)))
				return &x86[i];
	return NULL;
}

/* classify() returns the size of operands and immediates */
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
	for (int i = 0; i < sizeof(segments)/sizeof(syntax); i++)
		if (strcmp(segments[i].name, s) == 0) {
			return segments[i].code | sreg;
		}

	return -1;
}

typedef struct __instruction_s {
	int prefix;
	int operand;
	int modrm;
	int sib;
	int disp;
} instruction_s;



instruction_s* calc_sib(char* line, int op1) {
	int scale 	= -1;
	int index 	= -1;
	int base 	= -1;
	char* plus 	= strchr(line, '+');
	char* minus = strchr(line, '-');
	char* star 	= strchr(line, '*');

	int c 		= 0;
	int disp 	= 0;
	int mod 	= 0;

	instruction_s* ret = malloc(sizeof(instruction_s));
	memset(ret, 0, sizeof(instruction_s));
	//printf("%s\n", line);
	/* We haven't been provided the first operand by parse_line.
	This should mean that the displacement IS the first operand:
	mov [eax+8], ebx 	etc, etc
	*/
	if (op1 == 0) {
		char* op = strchr(line, ',');
		if (op != NULL) {
			printf("%s\n", op);
			while(*op == ' ' || *op == ',')
				op++;
			op1 = classify(op);
			if (op1 & (imm8|imm32))
				disp = lass_atoi(op);
		}
	}

	/* No multiplication symbol, so we assume that no SIB byte is required */
	if (star == NULL) {
		scale = 0;

		char* pch = strtok(line, " [+-],");
		while(pch) {
			if (isdigit(*pch)) {
				disp = lass_atoi(pch);
			} else {
				if (c == 0)
					index = classify(pch) & ~(rm32|rm8);
				if (c == 1)
					base = classify(pch);
				c++;
			}
			pch = strtok(NULL, " [+-],*");
		}

		if (disp) {
			disp *= (minus) ? -1 : 1;
			if (disp > UCHAR_MAX)
				ret->modrm = MODRM(MOD_FOUR, (op1 & ~(r8|r32)), index);
			else 
				ret->modrm = MODRM(MOD_ONE, (op1 & ~(r8|r32)), index);
			ret->disp = disp;
		} else {
			ret->modrm = MODRM((index == EBP) ? MOD_ONE : MOD_ZERO, (op1 & ~(r8|r32)), index);
			ret->disp = 0;
		}
		return ret;
	}

	/* We need to calculate an SIB byte 
	MOD = 00b, 01b, 10b and R/M = 100b */
	char* pch = strtok(line, " [+],*");
	int n;
	while(pch) {
		if (isdigit(*pch)) {
			n = lass_atoi(pch);

			if (plus < star & pch < plus) {
				disp  = n;
			}
			if (plus < star & pch > plus) {
				scale = n;
			}
		} else {
			if (c == 0)
				index = classify(pch);
			if (c == 1)
				base = classify(pch);
			c++;
		}
		
		pch = strtok(NULL, " [+],*");
	}

	/* R/M bits equals 4 always. Mod is determined by SIB addressing mode:
		00 = [reg32 + eax*n]
		01 = [disp + reg8 + eax*n]
		10 = [disp + reg32 + eax*n]
		00 = [disp + eax*n], base field = 101
	*/
	if (index && base) {
		if (index & r32) 
			mod = 2;
		else 
			mod = 1;
		index &= ~(r8 | r32);
		base &= ~(r8|r32);
	}
	if (base == -1) {
		base = 5;
		mod = 0;
	}

	scale = log2(scale);

	ret->modrm = MODRM(mod, op1, 4);
	ret->sib = MODRM(scale, index, base);
	ret->disp = disp;
	//printf("\nScale: %x Index: %x Base: %x disp: %x\n", scale, index, base, disp);
	//printf("%x %x %x\n", ret->modrm, ret->sib, ret->disp);
	return ret;
}

int parse_line(char* line, int pass_no) {
	if (pass_no > 1) printf("%08x %-20s\n",current_position, line);	

	char* displacement = NULL;
	char* disp = strpbrk(line, "[");

	if (disp)
		displacement = strdup(disp);

	char* pch = strtok(line, " ,\t");		// split line by spaces and commas
	char* inst;							// store instruction

	uint64_t syn[2] = { 0, 0};
	int count 		= 0;	// number of items in line
	int immediate 	= 0;	// store value of immediates
	int imm = 0;
	int mode = MOD_REG;		// default to MOD 11b
	int m = -1;
	int rev = 0;
	int sib = 0;


	if (!strcmp(pch, "db")) {
		char* pch = strtok(NULL, ", '\"");
		while(pch) {
			if (isdigit(*pch)) {
				immediate = lass_atoi(pch);
				write_byte(immediate);
			}
			else
				while(*pch) {
					write_byte(*pch++);
				}
			pch =  strtok(NULL, ", '\"");
		}
		return;
	}
	if (!strcmp(pch, "dw")) {
		char* pch = strtok(NULL, ", '\"");
		while(pch) {
			if (isdigit(*pch)) {
				immediate = lass_atoi(pch);
				write_word(immediate);
			}
			pch =  strtok(NULL, ", '\"");
		}
		return;
	}
	if (!strcmp(pch, "dd")) {
		char* pch = strtok(NULL, ", '\"");
		while(pch) {
			if (isdigit(*pch)) {
				immediate = lass_atoi(pch);
				write_dword(immediate);
			}
			pch =  strtok(NULL, ", '\"");
		}
		return;
	}

	while(pch && *pch != ';') {
		if (pch == disp && disp) {
			// Do something
			instruction_s* a = calc_sib(displacement, syn[0]);
			m = a->modrm;
			sib = a->sib;
			immediate = a->disp;
			mode = (m >> 6);
	
			//imm = (immediate != 0);
			imm = 1;

			syn[1] |= (r32);
			if (syn[0] == 0) {
				syn[0]  = (m >> 3) & 0x7 | r32;
				rev = 1;
			}
			break;
		}
		//printf("\t%s\n", pch);

		if (count == 0) {
			inst = strdup(pch);
		}
		else {
			int n = classify(pch);

			if (n & (imm8 | imm32)) {
				imm = 1;
				immediate = lass_atoi(pch);
				//printf("Immediate value: %x\n", immediate);
			} 
			if (n == -1) {	// label maybe?
				immediate = find_symbol(pch);
					imm = 1;
				//printf("Symbol? %s: 0x%x\n", pch, immediate);
				n = rel8;
			}
			syn[count - 1] = n;
		}
		count++;
		pch = strtok(NULL, " ,\t");		// get next token
	}

	isa* instruction = find_instruction(inst, syn[0], syn[1]);

	int start = current_position;

	if (instruction) {
		int o = instruction->primary;

		/* Get any prefixes taken care of */
		if (instruction->secondary)
			write_byte(instruction->secondary);
		if (instruction->prefix)
			write_byte(0x0F);

		if (syn[1]) {	// Two operands

			if ((syn[0] & (r32 | r8 | sreg)) && (syn[1]  & (r32 | r8 | sreg))) 
				m = (m == -1) ? MODRM(mode, syn[0] &= ~(r32 | r8 | sreg), syn[1] &= ~(r32 | r8 | sreg)) : m;
			else if ((syn[0] & (r32 | r8)) && (syn[1] & (imm8 | imm32 | rel8 | rel32))) 
				m = (m == -1) ? MODRM(mode, instruction->extension, syn[0] &= ~(r32 | r8)) : m;

			if ((syn[0] & (imm8 | imm32 | rel8 | rel32)))
				m = MODRM(mode, instruction->extension, 0);


			if (rev)
				o ^= 0x2;	// switch d flag

			if (instruction->extension == 0xDEAD) 	// Macro value for alternate encodings
				write_byte((o & ~0x7 | syn[0] & ~(r32|r8)));
			else
				write_word(m << 8 | o);

			//if (pass_no > 1) printf("\t%x%x\n", o << 8 | m);

		} else {	// One operand?

			if (instruction->extension == 0xDEAD && m == -1) {
			//	printf("EXTENSION\n");
				write_byte( (o & ~0x7 | syn[0] & ~(r32|r8 |sreg)));
			} else {
				if (!syn[0])
					write_byte(o);
				// Immediate, relative, none or ss/fs/gs/etc
				else if (syn[0] & 0x6C0 || (syn[0] & sreg))
					write_byte(o);
				else if (syn[0] != none) {
					m = (m == -1) ? MODRM(mode, instruction->extension, syn[0] & ~(r32 | r8 | sreg)) : m;
					write_word(m << 8 | o);
				//	if (pass_no > 1) printf("%x", MODRM(mode, instruction->extension, syn[0] &= ~(r32 | r8)) << 8 | o);
				}

			}
			//if (pass_no > 1) printf("\t%x\n", o);
		}

		/* If there's an SIB byte, write it now */
		if (sib) {
			write_byte(sib);
		}

		if (imm) {
			int sw = (instruction->op2 == none) ? instruction->op1 : instruction->op2;
			switch(sw) {
				case imm8:
					write_byte(immediate & 0xFF);
					break;
				case imm32:
					write_dword(immediate);
					break;
				case rel8:	// Calculate relative offset
					write_byte(immediate - current_position - (current_position-start) - elf_offset);
					break;
				case rel32:
					write_dword(immediate - current_position - (current_position-start) - elf_offset);
					break;
				default:	// SIB/Displacement 
					if (mode == 1)
						write_byte(immediate);
					else
						write_dword(immediate);
				}
		}
	} else {
		printf("instruction: %s not recognized?\n", inst);
	}
}

void pass(char* buffer, int sz, int pass_no) {
	char** lines = malloc(sz);

	*lines = buffer;
	uint32_t start = lines;

	for (int i = 0; i < sz; i++) {
		if (buffer[i] == '\n') {
			buffer[i] = '\0';
			char* new = (uint32_t) buffer + i + 1;
			if (*new) 
				*(lines++) = new;
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
				parse_line(*lines, pass_no);
		}
	}

	free(start);
}


int main(int argc, char* argv[]) {

	if (argc < 2) {
		printf("usage: lass input.s\n");
		return -1;
	}
	char* fname = argv[1];

	int fp = open(fname, O_RDWR, 0444);
	assert(fp);

	int sz = lseek(fp, 0, SEEK_END);		// seek to end of file
	if (sz < 1)
		return -1;

	lseek(fp, 0, SEEK_SET);		// back to beginning

	char* buffer = malloc(sz);	// File buffer
	output = malloc(sz);
	pread(fp, buffer, sz, 0);


	// First pass
	printf("lass: read %d bytes from %s\n", sz, fname);
	printf("First pass, finding labels:\n");
	pass(buffer, sz, 1);
	//Second pass
	program_length = current_position;
	current_position = 0;
	
	if (strcmp("elf", argv[2])==0) make_elf();
	pread(fp, buffer, sz, 0);

	pass(buffer, sz, 2);


	FILE* fd = fopen("output", "wb");
	assert(fd);
	fwrite(elf_output, 1, elf_offset, fd);
	fwrite(output, 1, current_position, fd);
	fflush(fd);
	free(buffer);
	printf("Wrote %d bytes to %s\n", current_position, "output" );

	return 0;
}
