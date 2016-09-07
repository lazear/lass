
// Mod/RM register codes
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

/* Conditional Test field (tttn)
1 byte opcodes: tttn field is bits 3-0 in the opcode byte
2 byte opcodes: tttn field is bits 3-0 in second byte
*/
#define COND_O		0	// Overflow
#define COND_NO		1	// No overflow
#define COND_B 		2	// Below, Not above or equal
#define COND_NAE 	(COND_B)
#define COND_NB 	3	// Not below, Above or equal
#define COND_AE 	(COND_NB) 
#define COND_Z		4	// Equal, Zero
#define COND_E 		(COND_Z)
#define COND_NZ	 	5	// Not zero, not equal
#define COND_NE 	(COND_NZ)
#define COND_BE 	6 	// Below or Equal
#define COND_NA 	(COND_BE)
#define COND_NBE 	7	// Not below or equal (Above)
#define COND_A 		(COND_NBE)
#define COND_S		8	// Sign
#define COND_NS		9 	// Not Sign
#define COND_P 		10	// Parity
#define COND_NP		11	// Non-parity
#define COND_L		12	//Less than <
#define COND_NGE	(COND_L)
#define COND_NL		13	// Not less than, >=
#define COND_GE 	(COND_NL)
#define COND_LE		14	// Less than, equal <=
#define COND_NG 	(COND_LE)	
#define COND_NLE	15	// Great than >
#define COND_G 		(COND_NLE)

#define ADD 0x00
#define ADD_EXT		0

#define AND			0x20	// AND with immediate is same as ADD with immediate
#define AND_EXT		4

#define CMP			0x38 	
#define CMP_EXT		7		// Reg field extension

#define DEC			0xFE
#define INC 		0xFE
#define DEC_EXT		1
#define INC_EXT		0

#define INT 		0xCD
#define INT_EXT		
#define RET 		0xC3
#define RET_EXT

#define SUB			0x28
#define SUB_EXT		5

#define JMP			0xEB
#define MOV			0x88
#define MOV_EXT
#define JMP_EXT		4

#define MOV_IMM		0xC6
#define MOV_IMM_EXT	0

#define IMM_OP		-2
#define ONE_OP		-1

#define COND(a)	(COND_##a)

#define IMM_BIT 		(1<<7)
#define OPCODE(code, sign, width) (code | ((sign & 1)<<1) | (width & 1))
#define MODRM(mod, reg, rm)	( ((mod & 0x3) << 6)| ((reg & 0x7) << 3) | (rm & 0x7) )
#define COMBINE(o, m) ( (o<<8) | m)

/* Preproccer magic */
/*
opcode: operation to be performed
s: sign bit, or direction bit. d=1 reg is destination, rm is source
w: width bit. w=1, 32 bit operands. w=0, 8 bit operands
mod: addressing moder
reg: register. set to -1 for immediate
rm: register/memory
*/

#define ENDIAN(x) ( ((x&0xFF) << 24) | ((x&0x0000FF00) << 8) | ((x&0x00FF0000) >> 8) | ((x&0xFF000000) >> 24))

#define OP1(opcode, s, w ,mod, reg, rm) \
	COMBINE(OPCODE(opcode, s, w), (MODRM(mod, reg, rm)))
#define OP(opcode, s, w, mod, reg, rm) \
	COMBINE(OPCODE(((reg == -2) ? IMM_BIT : opcode), s, w), (MODRM(mod, ((reg < 0) ? opcode ## _EXT : reg), rm)))

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
	{ "jmp", JMP_EXT },
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