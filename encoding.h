
// Mod/RM register codes
#define EAX 0
#define ECX 1
#define EDX 2
#define EBX 3
#define ESP 4
#define EBP 5
#define ESI 6
#define EDI 7
#define sreg	0x1000
#define ss 		sreg | 2
#define cs 		sreg | 1
#define ds 		sreg | 3
#define es 		sreg | 0
#define fs 		sreg | 4
#define gs 		sreg | 5

/*
Zero: [eax]
One: [eax+ disp8]
Four: [eax+ disp32]
Reg: eax */
#define MOD_ZERO	0
#define MOD_ONE		1
#define MOD_FOUR	2
#define MOD_REG		3

#define IMM_OP		-2
#define ONE_OP		-1
#define COND(a)	(COND_##a)

#define OPCODE(code, sign, width) (code | ((sign & 1)<<1) | (width & 1))
#define MODRM(mod, reg, rm)	( ((mod & 0x3) << 6)| ((reg & 0x7) << 3) | (rm & 0x7) )
#define COMBINE(o, m) ( (o<<8) | m)
#define ishex(c)	(((tolower(c)) <= 'f') & ((tolower(c)) >= '0'))
#define ENDIAN(x) ( ((x&0xFF) << 24) | ((x&0x0000FF00) << 8) | ((x&0x00FF0000) >> 8) | ((x&0xFF000000) >> 24))

struct symbol {
	char name[32];
	uint32_t position;
} symtab[256], unresolved[256];

typedef struct syntax {
	char name[10];
	int code;

} syntax;

syntax registers[] = {
	{ "eax", EAX },
	{ "ecx", ECX },
	{ "edx", EDX },
	{ "ebx", EBX },
	{ "esp", ESP },
	{ "ebp", EBP },
	{ "esi", ESI },
	{ "edi", EDI },
	{ "ax", EAX },
	{ "cx", ECX },
	{ "dx", EDX },
	{ "bx", EBX },
	{ "sp", ESP },
	{ "bp", EBP },
	{ "si", ESI },
	{ "di", EDI },
}, half[] = {
	{ "al", EAX },
	{ "cl", ECX },
	{ "dl", EDX },
	{ "bl", EBX },
	{ "ah", EAX },
	{ "ch", ECX },
	{ "dh", EDX },
	{ "bh", EBX },
}, segments[] = {
	{ "ss", ss },
	{ "cs", cs },
	{ "es", es },
	{ "fs", fs },
	{ "gs", gs },
	{ "ds", ds },
};