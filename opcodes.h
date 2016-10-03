 /* This file was generated by encoder.py as part of lass - the lightweight assembler
(C) 2016 Michael Lazear*/
#ifndef __LASS_ISA__
#define __LASS_ISA__

typedef struct _instruction_set {
	char name[10];
	int op1;
	int op2;
	int primary;
	int extension;
	int secondary;
	int prefix;
} isa;

#define none	0
#define r32 	0x10
#define rm32 	r32
#define r8 		0x20
#define rm8		r8
#define imm8 	0x40
#define imm32	0x80
#define crn 	0x100
#define rel8 	0x200
#define rel32 	0x400
#define eax 	r32 | EAX | 0x800 	// So this value doesn't get used
#define edx 	r32 | EDX
#define ecx		r32 | ECX
#define ebx		r32 | EBX
#define sreg	0x1000
#define ss 		sreg | 2
#define cs 		sreg | 1
#define ds 		sreg | 3
#define es 		sreg | 0
#define fs 		sreg | 4
#define gs 		sreg | 5

isa x86[] = {
	{"adc", r8, rm8, 0x12, 0x0, 0x0, 0x0},
	{"adc", r32, rm32, 0x13, 0x0, 0x0, 0x0},
	{"adc", eax, imm32, 0x15, 0x0, 0x0, 0x0},
	{"adc", rm8, imm8, 0x80, 0x2, 0x0, 0x0},
	{"adc", rm32, imm32, 0x81, 0x2, 0x0, 0x0},
	{"adc", rm8, imm8, 0x82, 0x2, 0x0, 0x0},
	{"adc", rm32, imm8, 0x83, 0x2, 0x0, 0x0},
	{"add", rm32, imm8, 0x83, 0x2, 0x0, 0x0},
	{"add", r8, rm8, 0x2, 0x0, 0x0, 0x0},
	{"add", r32, rm32, 0x3, 0x0, 0x0, 0x0},
	{"add", eax, imm32, 0x5, 0x0, 0x0, 0x0},
	{"add", rm8, imm8, 0x80, 0x0, 0x0, 0x0},
	{"add", rm32, imm32, 0x81, 0x0, 0x0, 0x0},
	{"add", rm8, imm8, 0x82, 0x0, 0x0, 0x0},
	{"add", rm32, imm8, 0x83, 0x0, 0x0, 0x0},
	{"and", r8, rm8, 0x22, 0x0, 0x0, 0x0},
	{"and", r32, rm32, 0x23, 0x0, 0x0, 0x0},
	{"and", eax, imm32, 0x25, 0x0, 0x0, 0x0},
	{"and", rm8, imm8, 0x80, 0x4, 0x0, 0x0},
	{"and", rm32, imm32, 0x81, 0x4, 0x0, 0x0},
	{"and", rm8, imm8, 0x82, 0x4, 0x0, 0x0},
	{"and", rm32, imm8, 0x83, 0x4, 0x0, 0x0},
	{"bsf", r32, rm32, 0xbc, 0x0, 0x0, 0x0f},
	{"bsr", r32, rm32, 0xbd, 0x0, 0x0, 0x0f},
	{"bswap", r32, none, 0xc8, 0xDEAD, 0x0, 0x0f},
	{"bt", rm32, imm8, 0xba, 0x4, 0x0, 0x0f},
	{"btc", rm32, imm8, 0xba, 0x7, 0x0, 0x0f},
	{"btr", rm32, imm8, 0xba, 0x6, 0x0, 0x0f},
	{"bts", rm32, imm8, 0xba, 0x5, 0x0, 0x0f},
	{"call", rel32, none, 0xe8, 0x0, 0x0, 0x0},
	{"call", rm32, none, 0xff, 0x2, 0x0, 0x0},
	{"cdq", edx, eax, 0x99, 0x0, 0x0, 0x0},
	{"clc", none, none, 0xf8, 0x0, 0x0, 0x0},
	{"cld", none, none, 0xfc, 0x0, 0x0, 0x0},
	{"cli", none, none, 0xfa, 0x0, 0x0, 0x0},
	{"cmc", none, none, 0xf5, 0x0, 0x0, 0x0},
	{"cmovb", r32, rm32, 0x42, 0x0, 0x0, 0x0f},
	{"cmovnae", r32, rm32, 0x42, 0x0, 0x0, 0x0f},
	{"cmovc", r32, rm32, 0x42, 0x0, 0x0, 0x0f},
	{"cmovbe", r32, rm32, 0x46, 0x0, 0x0, 0x0f},
	{"cmovna", r32, rm32, 0x46, 0x0, 0x0, 0x0f},
	{"cmovl", r32, rm32, 0x4c, 0x0, 0x0, 0x0f},
	{"cmovnge", r32, rm32, 0x4c, 0x0, 0x0, 0x0f},
	{"cmovle", r32, rm32, 0x4e, 0x0, 0x0, 0x0f},
	{"cmovng", r32, rm32, 0x4e, 0x0, 0x0, 0x0f},
	{"cmovnb", r32, rm32, 0x43, 0x0, 0x0, 0x0f},
	{"cmovae", r32, rm32, 0x43, 0x0, 0x0, 0x0f},
	{"cmovnc", r32, rm32, 0x43, 0x0, 0x0, 0x0f},
	{"cmovnbe", r32, rm32, 0x47, 0x0, 0x0, 0x0f},
	{"cmova", r32, rm32, 0x47, 0x0, 0x0, 0x0f},
	{"cmovnl", r32, rm32, 0x4d, 0x0, 0x0, 0x0f},
	{"cmovge", r32, rm32, 0x4d, 0x0, 0x0, 0x0f},
	{"cmovnle", r32, rm32, 0x4f, 0x0, 0x0, 0x0f},
	{"cmovg", r32, rm32, 0x4f, 0x0, 0x0, 0x0f},
	{"cmovno", r32, rm32, 0x41, 0x0, 0x0, 0x0f},
	{"cmovnp", r32, rm32, 0x4b, 0x0, 0x0, 0x0f},
	{"cmovpo", r32, rm32, 0x4b, 0x0, 0x0, 0x0f},
	{"cmovns", r32, rm32, 0x49, 0x0, 0x0, 0x0f},
	{"cmovnz", r32, rm32, 0x45, 0x0, 0x0, 0x0f},
	{"cmovne", r32, rm32, 0x45, 0x0, 0x0, 0x0f},
	{"cmovo", r32, rm32, 0x40, 0x0, 0x0, 0x0f},
	{"cmovp", r32, rm32, 0x4a, 0x0, 0x0, 0x0f},
	{"cmovpe", r32, rm32, 0x4a, 0x0, 0x0, 0x0f},
	{"cmovs", r32, rm32, 0x48, 0x0, 0x0, 0x0f},
	{"cmovz", r32, rm32, 0x44, 0x0, 0x0, 0x0f},
	{"cmove", r32, rm32, 0x44, 0x0, 0x0, 0x0f},
	{"cmp", r8, rm8, 0x3a, 0x0, 0x0, 0x0},
	{"cmp", r32, rm32, 0x3b, 0x0, 0x0, 0x0},
	{"cmp", eax, imm32, 0x3d, 0x0, 0x0, 0x0},
	{"cmp", rm8, imm8, 0x80, 0x7, 0x0, 0x0},
	{"cmp", rm32, imm32, 0x81, 0x7, 0x0, 0x0},
	{"cmp", rm8, imm8, 0x82, 0x7, 0x0, 0x0},
	{"cmp", rm32, imm8, 0x83, 0x7, 0x0, 0x0},
	{"cmpsb", rm32, imm8, 0x83, 0x7, 0x0, 0x0},
	{"cmpsw", rm32, imm8, 0x83, 0x7, 0x0, 0x0},
	{"cmpsd", rm32, imm8, 0x83, 0x7, 0x0, 0x0},
	{"cmpxchg", rm32, eax, 0xb1, 0x0, 0x0, 0x0f},
	{"crc32", r32, rm8, 0x38, 0x0, 0xf2, 0x0f},
	{"crc32", r32, rm32, 0x38, 0x0, 0xf2, 0x0f},
	{"cs", r32, rm32, 0x38, 0x0, 0xf2, 0x0f},
	{"dec", r32, none, 0x48, 0xDEAD, 0x0, 0x0},
	{"dec", rm8, none, 0xfe, 0x0, 0x0, 0x0},
	{"dec", rm32, none, 0xff, 0x0, 0x0, 0x0},
	{"div", rm8, none, 0xf6, 0x6, 0x0, 0x0},
	{"div", rm32, none, 0xf7, 0x6, 0x0, 0x0},
	{"ds", rm32, none, 0xf7, 0x6, 0x0, 0x0},
	{"emms", none, none, 0x77, 0x0, 0x0, 0x0f},
	{"es", none, none, 0x77, 0x0, 0x0, 0x0f},
	{"fadd", none, none, 0x77, 0x0, 0x0, 0x0f},
	{"fclex", none, none, 0xdb, 0x4, 0x9b, 0x0},
	{"fdecstp", none, none, 0xd9, 0x6, 0x0, 0x0},
	{"fdiv", none, none, 0xd9, 0x6, 0x0, 0x0},
	{"fdivr", none, none, 0xd9, 0x6, 0x0, 0x0},
	{"fincstp", none, none, 0xd9, 0x6, 0x0, 0x0},
	{"finit", none, none, 0xdb, 0x4, 0x9b, 0x0},
	{"fmul", none, none, 0xdb, 0x4, 0x9b, 0x0},
	{"fnclex", none, none, 0xdb, 0x4, 0x0, 0x0},
	{"fndisi nop", none, none, 0xdb, 0x4, 0x0, 0x0},
	{"fneni nop", none, none, 0xdb, 0x4, 0x0, 0x0},
	{"fninit", none, none, 0xdb, 0x4, 0x0, 0x0},
	{"fnop", none, none, 0xd9, 0x2, 0x0, 0x0},
	{"fnsetpm nop", none, none, 0xdb, 0x4, 0x0, 0x0},
	{"fs", none, none, 0xdb, 0x4, 0x0, 0x0},
	{"fsub", none, none, 0xdb, 0x4, 0x0, 0x0},
	{"fsubr", none, none, 0xdb, 0x4, 0x0, 0x0},
	{"fwait", none, none, 0x9b, 0x0, 0x0, 0x0},
	{"wait", none, none, 0x9b, 0x0, 0x0, 0x0},
	{"getsec", eax, none, 0x37, 0x0, 0x0, 0x0f},
	{"gs", eax, none, 0x37, 0x0, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x18, 0x4, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x18, 0x5, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x18, 0x6, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x18, 0x7, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x19, 0x0, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1a, 0x0, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1b, 0x0, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1c, 0x0, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1d, 0x0, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1e, 0x0, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1f, 0x0, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1f, 0x2, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1f, 0x3, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1f, 0x4, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1f, 0x5, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1f, 0x6, 0x0, 0x0f},
	{"hint_nop", rm32, none, 0x1f, 0x7, 0x0, 0x0f},
	{"hlt", none, none, 0xf4, 0x0, 0x0, 0x0},
	{"idiv", edx, eax, 0xf7, 0x7, 0x0, 0x0},
	{"imul", r32, rm32, 0x69, 0x0, 0x0, 0x0},
	{"imul", r32, rm32, 0x6b, 0x0, 0x0, 0x0},
	{"imul", edx, eax, 0xf7, 0x5, 0x0, 0x0},
	{"imul", r32, rm32, 0xaf, 0x0, 0x0, 0x0f},
	{"in", eax, imm8, 0xe5, 0x0, 0x0, 0x0},
	{"inc", r32, none, 0x40, 0xDEAD, 0x0, 0x0},
	{"inc", rm8, none, 0xfe, 0x0, 0x0, 0x0},
	{"inc", rm32, none, 0xff, 0x0, 0x0, 0x0},
	{"insb", rm32, none, 0xff, 0x0, 0x0, 0x0},
	{"insw", rm32, none, 0xff, 0x0, 0x0, 0x0},
	{"insd", rm32, none, 0xff, 0x0, 0x0, 0x0},
	{"insertps", rm32, none, 0xff, 0x0, 0x0, 0x0},
	{"int", imm8, none, 0xcd, 0x0, 0x0, 0x0},
	{"int1", none, none, 0xf1, 0x0, 0x0, 0x0},
	{"icebp", none, none, 0xf1, 0x0, 0x0, 0x0},
	{"into", none, none, 0xce, 0x0, 0x0, 0x0},
	{"invd", none, none, 0x8, 0x0, 0x0, 0x0f},
	{"invlpg", none, none, 0x8, 0x0, 0x0, 0x0f},
	{"iret", none, none, 0xcf, 0x0, 0x0, 0x0},
	{"iretd", none, none, 0xcf, 0x0, 0x0, 0x0},
	{"jb", rel8, none, 0x72, 0x0, 0x0, 0x0},
	{"jnae", rel8, none, 0x72, 0x0, 0x0, 0x0},
	{"jc", rel8, none, 0x72, 0x0, 0x0, 0x0},
	{"jb", rel32, none, 0x82, 0x0, 0x0, 0x0f},
	{"jnae", rel32, none, 0x82, 0x0, 0x0, 0x0f},
	{"jc", rel32, none, 0x82, 0x0, 0x0, 0x0f},
	{"jbe", rel8, none, 0x76, 0x0, 0x0, 0x0},
	{"jna", rel8, none, 0x76, 0x0, 0x0, 0x0},
	{"jbe", rel32, none, 0x86, 0x0, 0x0, 0x0f},
	{"jna", rel32, none, 0x86, 0x0, 0x0, 0x0f},
	{"jecxz", rel32, none, 0x86, 0x0, 0x0, 0x0f},
	{"jl", rel8, none, 0x7c, 0x0, 0x0, 0x0},
	{"jnge", rel8, none, 0x7c, 0x0, 0x0, 0x0},
	{"jl", rel32, none, 0x8c, 0x0, 0x0, 0x0f},
	{"jnge", rel32, none, 0x8c, 0x0, 0x0, 0x0f},
	{"jle", rel8, none, 0x7e, 0x0, 0x0, 0x0},
	{"jng", rel8, none, 0x7e, 0x0, 0x0, 0x0},
	{"jle", rel32, none, 0x8e, 0x0, 0x0, 0x0f},
	{"jng", rel32, none, 0x8e, 0x0, 0x0, 0x0f},
	{"jmp", rel32, none, 0xe9, 0x0, 0x0, 0x0},
	{"jmp", rel8, none, 0xeb, 0x0, 0x0, 0x0},
	{"jmp", rm32, none, 0xff, 0x4, 0x0, 0x0},
	{"jnb", rel8, none, 0x73, 0x0, 0x0, 0x0},
	{"jae", rel8, none, 0x73, 0x0, 0x0, 0x0},
	{"jnc", rel8, none, 0x73, 0x0, 0x0, 0x0},
	{"jnb", rel32, none, 0x83, 0x0, 0x0, 0x0f},
	{"jae", rel32, none, 0x83, 0x0, 0x0, 0x0f},
	{"jnc", rel32, none, 0x83, 0x0, 0x0, 0x0f},
	{"jnbe", rel8, none, 0x77, 0x0, 0x0, 0x0},
	{"ja", rel8, none, 0x77, 0x0, 0x0, 0x0},
	{"jnbe", rel32, none, 0x87, 0x0, 0x0, 0x0f},
	{"ja", rel32, none, 0x87, 0x0, 0x0, 0x0f},
	{"jnl", rel8, none, 0x7d, 0x0, 0x0, 0x0},
	{"jge", rel8, none, 0x7d, 0x0, 0x0, 0x0},
	{"jnl", rel32, none, 0x8d, 0x0, 0x0, 0x0f},
	{"jge", rel32, none, 0x8d, 0x0, 0x0, 0x0f},
	{"jnle", rel8, none, 0x7f, 0x0, 0x0, 0x0},
	{"jg", rel8, none, 0x7f, 0x0, 0x0, 0x0},
	{"jnle", rel32, none, 0x8f, 0x0, 0x0, 0x0f},
	{"jg", rel32, none, 0x8f, 0x0, 0x0, 0x0f},
	{"jno", rel8, none, 0x71, 0x0, 0x0, 0x0},
	{"jno", rel32, none, 0x81, 0x0, 0x0, 0x0f},
	{"jnp", rel8, none, 0x7b, 0x0, 0x0, 0x0},
	{"jpo", rel8, none, 0x7b, 0x0, 0x0, 0x0},
	{"jnp", rel32, none, 0x8b, 0x0, 0x0, 0x0f},
	{"jpo", rel32, none, 0x8b, 0x0, 0x0, 0x0f},
	{"jns", rel8, none, 0x79, 0x0, 0x0, 0x0},
	{"jns", rel32, none, 0x89, 0x0, 0x0, 0x0f},
	{"jnz", rel8, none, 0x75, 0x0, 0x0, 0x0},
	{"jne", rel8, none, 0x75, 0x0, 0x0, 0x0},
	{"jnz", rel32, none, 0x85, 0x0, 0x0, 0x0f},
	{"jne", rel32, none, 0x85, 0x0, 0x0, 0x0f},
	{"jo", rel8, none, 0x70, 0x0, 0x0, 0x0},
	{"jo", rel32, none, 0x80, 0x0, 0x0, 0x0f},
	{"jp", rel8, none, 0x7a, 0x0, 0x0, 0x0},
	{"jpe", rel8, none, 0x7a, 0x0, 0x0, 0x0},
	{"jp", rel32, none, 0x8a, 0x0, 0x0, 0x0f},
	{"jpe", rel32, none, 0x8a, 0x0, 0x0, 0x0f},
	{"js", rel8, none, 0x78, 0x0, 0x0, 0x0},
	{"js", rel32, none, 0x88, 0x0, 0x0, 0x0f},
	{"jz", rel8, none, 0x74, 0x0, 0x0, 0x0},
	{"je", rel8, none, 0x74, 0x0, 0x0, 0x0},
	{"jz", rel32, none, 0x84, 0x0, 0x0, 0x0f},
	{"je", rel32, none, 0x84, 0x0, 0x0, 0x0f},
	{"lar", rel32, none, 0x84, 0x0, 0x0, 0x0f},
	{"lds", ds, r32, 0xc5, 0x0, 0x0, 0x0},
	{"lea", r32, rel32, 0x8d, 0x0, 0x0, 0x0},
	{"les", es, r32, 0xc4, 0x0, 0x0, 0x0},
	{"lfence", none, none, 0xae, 0x5, 0x0, 0x0f},
	{"lfs", fs, r32, 0xb4, 0x0, 0x0, 0x0f},
	{"lgdt", r32, none, 0x1, 0x0, 0x0, 0x0f},
	{"lgs", gs, r32, 0xb5, 0x0, 0x0, 0x0f},
	{"lidt", rel32, none, 0x1, 0x0, 0x0, 0x0f},
	{"lmsw", gs, r32, 0xb5, 0x0, 0x0, 0x0f},
	{"lock", gs, r32, 0xb5, 0x0, 0x0, 0x0f},
	{"lodsb", gs, r32, 0xb5, 0x0, 0x0, 0x0f},
	{"lodsw", gs, r32, 0xb5, 0x0, 0x0, 0x0f},
	{"lodsd", gs, r32, 0xb5, 0x0, 0x0, 0x0f},
	{"loop", ecx, rel8, 0xe2, 0x0, 0x0, 0x0},
	{"loopnz", ecx, rel8, 0xe0, 0x0, 0x0, 0x0},
	{"loopne", ecx, rel8, 0xe0, 0x0, 0x0, 0x0},
	{"loopz", ecx, rel8, 0xe1, 0x0, 0x0, 0x0},
	{"loope", ecx, rel8, 0xe1, 0x0, 0x0, 0x0},
	{"lsl", ecx, rel8, 0xe1, 0x0, 0x0, 0x0},
	{"lss", ss, r32, 0xb2, 0x0, 0x0, 0x0f},
	{"mfence", none, none, 0xae, 0x6, 0x0, 0x0f},
	{"monitor", none, none, 0xae, 0x6, 0x0, 0x0f},
	{"mov", r8, rm8, 0x8a, 0x0, 0x0, 0x0},
	{"mov", r32, rm32, 0x8b, 0x0, 0x0, 0x0},
	{"mov", r32, rm32, 0x8b, 0x0, 0x0, 0x0},
	{"mov", sreg, rm32, 0x8e, 0x0, 0x0, 0x0},
	{"mov", r8, imm8, 0xb0, 0xDEAD, 0x0, 0x0},
	{"mov", r32, imm32, 0xb8, 0xDEAD, 0x0, 0x0},
	{"mov", rm8, imm8, 0xc6, 0x0, 0x0, 0x0},
	{"mov", rm32, imm32, 0xc7, 0x0, 0x0, 0x0},
	{"mov", r32, crn, 0x20, 0x0, 0x0, 0x0f},
	{"mov", r32, crn, 0x20, 0x0, 0x0, 0x0f},
	{"mov", crn, r32, 0x22, 0x0, 0x0, 0x0f},
	{"mov", crn, r32, 0x22, 0x0, 0x0, 0x0f},
	{"movsb", crn, r32, 0x22, 0x0, 0x0, 0x0f},
	{"movsw", crn, r32, 0x22, 0x0, 0x0, 0x0f},
	{"movsd", crn, r32, 0x22, 0x0, 0x0, 0x0f},
	{"movsx", r32, rm8, 0xbe, 0x0, 0x0, 0x0f},
	{"movsx", r32, rm32, 0xbf, 0x0, 0x0, 0x0f},
	{"movzx", r32, rm8, 0xb6, 0x0, 0x0, 0x0f},
	{"movzx", r32, rm32, 0xb7, 0x0, 0x0, 0x0f},
	{"mul", rm8, none, 0xf6, 0x4, 0x0, 0x0},
	{"mul", rm32, none, 0xf7, 0x4, 0x0, 0x0},
	{"mwait", rm32, none, 0xf7, 0x4, 0x0, 0x0},
	{"neg", rm8, none, 0xf6, 0x3, 0x0, 0x0},
	{"neg", rm32, none, 0xf7, 0x3, 0x0, 0x0},
	{"nop", none, none, 0x90, 0x0, 0x0, 0x0},
	{"nop", rm32, none, 0x0d, 0x0, 0x0, 0x0f},
	{"nop", rm32, none, 0x1f, 0x0, 0x0, 0x0f},
	{"not", rm8, none, 0xf6, 0x2, 0x0, 0x0},
	{"not", rm32, none, 0xf7, 0x2, 0x0, 0x0},
	{"or", r8, rm8, 0x0a, 0x0, 0x0, 0x0},
	{"or", r32, rm32, 0x0b, 0x0, 0x0, 0x0},
	{"or", eax, imm32, 0x0d, 0x0, 0x0, 0x0},
	{"or", rm8, imm8, 0x80, 0x0, 0x0, 0x0},
	{"or", rm32, imm32, 0x81, 0x0, 0x0, 0x0},
	{"or", rm8, imm8, 0x82, 0x0, 0x0, 0x0},
	{"or", rm32, imm8, 0x83, 0x0, 0x0, 0x0},
	{"out", imm8, eax, 0xe7, 0x0, 0x0, 0x0},
	{"outsb", imm8, eax, 0xe7, 0x0, 0x0, 0x0},
	{"outsw", imm8, eax, 0xe7, 0x0, 0x0, 0x0},
	{"outsd", imm8, eax, 0xe7, 0x0, 0x0, 0x0},
	{"pause", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pextrb", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pextrq", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pextrw", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pinsrb", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pinsrq", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pinsrw", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pinsrw", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovsxbd", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovsxbq", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovsxbw", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovsxdq", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovsxwd", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovsxwq", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovzxbd", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovzxbq", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovzxbw", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovzxdq", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovzxwd", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pmovzxwq", none, none, 0x90, 0x0, 0xf3, 0x0},
	{"pop", es, none, 0x7, 0x0, 0x0, 0x0},
	{"pop", ss, none, 0x17, 0x0, 0x0, 0x0},
	{"pop", ds, none, 0x1f, 0x0, 0x0, 0x0},
	{"pop", r32, none, 0x58, 0xDEAD, 0x0, 0x0},
	{"pop", rm32, none, 0x8f, 0x0, 0x0, 0x0},
	{"pop", fs, none, 0xa1, 0x0, 0x0, 0x0f},
	{"pop", gs, none, 0xa9, 0x0, 0x0, 0x0f},
	{"popa", none, none, 0x61, 0x0, 0x0, 0x0},
	{"popad", none, none, 0x61, 0x0, 0x0, 0x0},
	{"popcnt", r32, rm32, 0xb8, 0x0, 0xf3, 0x0f},
	{"popf", none, none, 0x9d, 0x0, 0x0, 0x0},
	{"popfd", none, none, 0x9d, 0x0, 0x0, 0x0},
	{"push", es, none, 0x6, 0x0, 0x0, 0x0},
	{"push", cs, none, 0x0e, 0x0, 0x0, 0x0},
	{"push", ss, none, 0x32, 0x0, 0x0, 0x0},
	{"push", ds, none, 0x1e, 0x0, 0x0, 0x0},
	{"push", r32, none, 0x50, 0xDEAD, 0x0, 0x0},
	{"push", imm32, none, 0x68, 0x0, 0x0, 0x0},
	{"push", imm8, none, 0x6a, 0x0, 0x0, 0x0},
	{"push", rm32, none, 0xff, 0x6, 0x0, 0x0},
	{"push", fs, none, 0xa0, 0x0, 0x0, 0x0f},
	{"push", gs, none, 0xa8, 0x0, 0x0, 0x0f},
	{"pusha", none, none, 0x60, 0x0, 0x0, 0x0},
	{"pushad", none, none, 0x60, 0x0, 0x0, 0x0},
	{"pushf", none, none, 0x9c, 0x0, 0x0, 0x0},
	{"pushfd", none, none, 0x9c, 0x0, 0x0, 0x0},
	{"rcl", rm8, imm8, 0xc0, 0x2, 0x0, 0x0},
	{"rcl", rm32, imm8, 0xc1, 0x2, 0x0, 0x0},
	{"rcl", rm8, none, 0xd0, 0x2, 0x0, 0x0},
	{"rcl", rm32, none, 0xd1, 0x2, 0x0, 0x0},
	{"rcr", rm8, imm8, 0xc0, 0x3, 0x0, 0x0},
	{"rcr", rm32, imm8, 0xc1, 0x3, 0x0, 0x0},
	{"rcr", rm8, none, 0xd0, 0x3, 0x0, 0x0},
	{"rcr", rm32, none, 0xd1, 0x3, 0x0, 0x0},
	{"rdmsr", eax, edx, 0x32, 0x0, 0x0, 0x0f},
	{"rdpmc", eax, edx, 0x33, 0x0, 0x0, 0x0f},
	{"rdtsc", eax, edx, 0x31, 0x0, 0x0, 0x0f},
	{"rdtscp", eax, edx, 0x31, 0x0, 0x0, 0x0f},
	{"rep", eax, edx, 0x31, 0x0, 0x0, 0x0f},
	{"rep", eax, edx, 0x31, 0x0, 0x0, 0x0f},
	{"repnz", eax, edx, 0x31, 0x0, 0x0, 0x0f},
	{"repne", eax, edx, 0x31, 0x0, 0x0, 0x0f},
	{"repz", eax, edx, 0x31, 0x0, 0x0, 0x0f},
	{"repe", eax, edx, 0x31, 0x0, 0x0, 0x0f},
	{"retf", imm32, none, 0xca, 0x0, 0x0, 0x0},
	{"retf", none, none, 0xcb, 0x0, 0x0, 0x0},
	{"retn", imm32, none, 0xc2, 0x0, 0x0, 0x0},
	{"retn", none, none, 0xc3, 0x0, 0x0, 0x0},
	{"rol", rm8, imm8, 0xc0, 0x0, 0x0, 0x0},
	{"rol", rm32, imm8, 0xc1, 0x0, 0x0, 0x0},
	{"rol", rm8, none, 0xd0, 0x0, 0x0, 0x0},
	{"rol", rm32, none, 0xd1, 0x0, 0x0, 0x0},
	{"ror", rm8, imm8, 0xc0, 0x0, 0x0, 0x0},
	{"ror", rm32, imm8, 0xc1, 0x0, 0x0, 0x0},
	{"ror", rm8, none, 0xd0, 0x0, 0x0, 0x0},
	{"ror", rm32, none, 0xd1, 0x0, 0x0, 0x0},
	{"rsm", none, none, 0xaa, 0x0, 0x0, 0x0f},
	{"sal", rm8, imm8, 0xc0, 0x6, 0x0, 0x0},
	{"shl", rm8, imm8, 0xc0, 0x6, 0x0, 0x0},
	{"sal", rm32, imm8, 0xc1, 0x6, 0x0, 0x0},
	{"shl", rm32, imm8, 0xc1, 0x6, 0x0, 0x0},
	{"sal", rm8, none, 0xd0, 0x6, 0x0, 0x0},
	{"shl", rm8, none, 0xd0, 0x6, 0x0, 0x0},
	{"sal", rm32, none, 0xd1, 0x6, 0x0, 0x0},
	{"shl", rm32, none, 0xd1, 0x6, 0x0, 0x0},
	{"shl", rm32, none, 0xd1, 0x6, 0x0, 0x0},
	{"shl", rm32, none, 0xd1, 0x6, 0x0, 0x0},
	{"setalc", rm32, none, 0xd1, 0x6, 0x0, 0x0},
	{"sar", rm8, imm8, 0xc0, 0x7, 0x0, 0x0},
	{"sar", rm32, imm8, 0xc1, 0x7, 0x0, 0x0},
	{"sar", rm8, none, 0xd0, 0x7, 0x0, 0x0},
	{"sar", rm32, none, 0xd1, 0x7, 0x0, 0x0},
	{"sbb", r8, rm8, 0x1a, 0x0, 0x0, 0x0},
	{"sbb", r32, rm32, 0x1b, 0x0, 0x0, 0x0},
	{"sbb", eax, imm32, 0x1d, 0x0, 0x0, 0x0},
	{"sbb", rm8, imm8, 0x80, 0x3, 0x0, 0x0},
	{"sbb", rm32, imm32, 0x81, 0x3, 0x0, 0x0},
	{"sbb", rm8, imm8, 0x82, 0x3, 0x0, 0x0},
	{"sbb", rm32, imm8, 0x83, 0x3, 0x0, 0x0},
	{"scasb", rm32, imm8, 0x83, 0x3, 0x0, 0x0},
	{"scasw", rm32, imm8, 0x83, 0x3, 0x0, 0x0},
	{"scasd", rm32, imm8, 0x83, 0x3, 0x0, 0x0},
	{"setb", rm8, none, 0x92, 0x0, 0x0, 0x0f},
	{"setnae", rm8, none, 0x92, 0x0, 0x0, 0x0f},
	{"setc", rm8, none, 0x92, 0x0, 0x0, 0x0f},
	{"setbe", rm8, none, 0x96, 0x0, 0x0, 0x0f},
	{"setna", rm8, none, 0x96, 0x0, 0x0, 0x0f},
	{"setl", rm8, none, 0x9c, 0x0, 0x0, 0x0f},
	{"setnge", rm8, none, 0x9c, 0x0, 0x0, 0x0f},
	{"setle", rm8, none, 0x9e, 0x0, 0x0, 0x0f},
	{"setng", rm8, none, 0x9e, 0x0, 0x0, 0x0f},
	{"setnb", rm8, none, 0x93, 0x0, 0x0, 0x0f},
	{"setae", rm8, none, 0x93, 0x0, 0x0, 0x0f},
	{"setnc", rm8, none, 0x93, 0x0, 0x0, 0x0f},
	{"setnbe", rm8, none, 0x97, 0x0, 0x0, 0x0f},
	{"seta", rm8, none, 0x97, 0x0, 0x0, 0x0f},
	{"setnl", rm8, none, 0x9d, 0x0, 0x0, 0x0f},
	{"setge", rm8, none, 0x9d, 0x0, 0x0, 0x0f},
	{"setnle", rm8, none, 0x9f, 0x0, 0x0, 0x0f},
	{"setg", rm8, none, 0x9f, 0x0, 0x0, 0x0f},
	{"setno", rm8, none, 0x91, 0x0, 0x0, 0x0f},
	{"setnp", rm8, none, 0x9b, 0x0, 0x0, 0x0f},
	{"setpo", rm8, none, 0x9b, 0x0, 0x0, 0x0f},
	{"setns", rm8, none, 0x99, 0x0, 0x0, 0x0f},
	{"setnz", rm8, none, 0x95, 0x0, 0x0, 0x0f},
	{"setne", rm8, none, 0x95, 0x0, 0x0, 0x0f},
	{"seto", rm8, none, 0x90, 0x0, 0x0, 0x0f},
	{"setp", rm8, none, 0x9a, 0x0, 0x0, 0x0f},
	{"setpe", rm8, none, 0x9a, 0x0, 0x0, 0x0f},
	{"sets", rm8, none, 0x98, 0x0, 0x0, 0x0f},
	{"setz", rm8, none, 0x94, 0x0, 0x0, 0x0f},
	{"sete", rm8, none, 0x94, 0x0, 0x0, 0x0f},
	{"sfence", none, none, 0xae, 0x7, 0x0, 0x0f},
	{"sgdt", none, none, 0xae, 0x7, 0x0, 0x0f},
	{"shl", rm8, imm8, 0xc0, 0x4, 0x0, 0x0},
	{"sal", rm8, imm8, 0xc0, 0x4, 0x0, 0x0},
	{"shl", rm32, imm8, 0xc1, 0x4, 0x0, 0x0},
	{"sal", rm32, imm8, 0xc1, 0x4, 0x0, 0x0},
	{"shl", rm8, none, 0xd0, 0x4, 0x0, 0x0},
	{"sal", rm8, none, 0xd0, 0x4, 0x0, 0x0},
	{"shl", rm32, none, 0xd1, 0x4, 0x0, 0x0},
	{"sal", rm32, none, 0xd1, 0x4, 0x0, 0x0},
	{"sal", rm32, none, 0xd1, 0x4, 0x0, 0x0},
	{"sal", rm32, none, 0xd1, 0x4, 0x0, 0x0},
	{"shr", rm8, imm8, 0xc0, 0x5, 0x0, 0x0},
	{"shr", rm32, imm8, 0xc1, 0x5, 0x0, 0x0},
	{"shr", rm8, none, 0xd0, 0x5, 0x0, 0x0},
	{"shr", rm32, none, 0xd1, 0x5, 0x0, 0x0},
	{"sidt", rm32, none, 0xd1, 0x5, 0x0, 0x0},
	{"sldt", rm32, none, 0xd1, 0x5, 0x0, 0x0},
	{"smsw", rm32, none, 0xd1, 0x5, 0x0, 0x0},
	{"smsw", rm32, none, 0xd1, 0x5, 0x0, 0x0},
	{"ss", rm32, none, 0xd1, 0x5, 0x0, 0x0},
	{"stc", none, none, 0xf9, 0x0, 0x0, 0x0},
	{"std", none, none, 0xfd, 0x0, 0x0, 0x0},
	{"sti", none, none, 0xfb, 0x0, 0x0, 0x0},
	{"stos", none, none, 0xaa, 0x0, 0x0, 0x0},
	{"stosb", none, none, 0xaa, 0x0, 0x0, 0x0},
	{"stos", none, none, 0xab, 0x0, 0x0, 0x0},
	{"stosw", none, none, 0xab, 0x0, 0x0, 0x0},
	{"stos", none, none, 0xab, 0x0, 0x0, 0x0},
	{"stosd", none, none, 0xab, 0x0, 0x0, 0x0},
	{"str", none, none, 0xab, 0x0, 0x0, 0x0},
	{"sub", r8, rm8, 0x2a, 0x0, 0x0, 0x0},
	{"sub", r32, rm32, 0x2b, 0x0, 0x0, 0x0},
	{"sub", eax, imm32, 0x2d, 0x0, 0x0, 0x0},
	{"sub", rm8, imm8, 0x80, 0x5, 0x0, 0x0},
	{"sub", rm32, imm32, 0x81, 0x5, 0x0, 0x0},
	{"sub", rm8, imm8, 0x82, 0x5, 0x0, 0x0},
	{"sub", rm32, imm8, 0x83, 0x5, 0x0, 0x0},
	{"sysenter", none, none, 0x34, 0x0, 0x0, 0x0f},
	{"sysexit", none, none, 0x35, 0x0, 0x0, 0x0f},
	{"test", rm8, r8, 0x84, 0x0, 0x0, 0x0},
	{"test", rm32, r32, 0x85, 0x0, 0x0, 0x0},
	{"test", eax, imm32, 0xa9, 0x0, 0x0, 0x0},
	{"test", rm8, imm8, 0xf6, 0x0, 0x0, 0x0},
	{"test", rm8, imm8, 0xf6, 0x0, 0x0, 0x0},
	{"test", rm32, imm32, 0xf7, 0x0, 0x0, 0x0},
	{"test", rm32, imm32, 0xf7, 0x0, 0x0, 0x0},
	{"ud2", none, none, 0x0b, 0x0, 0x0, 0x0f},
	{"verr", rm32, none, 0x0, 0x4, 0x0, 0x0f},
	{"verw", rm32, none, 0x0, 0x5, 0x0, 0x0f},
	{"vmcall", rm32, none, 0x0, 0x5, 0x0, 0x0f},
	{"vmlaunch", rm32, none, 0x0, 0x5, 0x0, 0x0f},
	{"vmresume", rm32, none, 0x0, 0x5, 0x0, 0x0f},
	{"vmwrite", r32, rm32, 0x79, 0x0, 0x0, 0x0f},
	{"vmxoff", r32, rm32, 0x79, 0x0, 0x0, 0x0f},
	{"wbinvd", none, none, 0x9, 0x0, 0x0, 0x0f},
	{"xchg", r8, rm8, 0x86, 0x0, 0x0, 0x0},
	{"xchg", r32, rm32, 0x87, 0x0, 0x0, 0x0},
	{"xchg", r32, eax, 0x90, 0xDEAD, 0x0, 0x0},
	{"xgetbv", r32, eax, 0x90, 0xDEAD, 0x0, 0x0},
	{"xlatb", r32, eax, 0x90, 0xDEAD, 0x0, 0x0},
	{"xor", r8, rm8, 0x32, 0x0, 0x0, 0x0},
	{"xor", r32, rm32, 0x33, 0x0, 0x0, 0x0},
	{"xor", eax, imm32, 0x35, 0x0, 0x0, 0x0},
	{"xor", rm8, imm8, 0x80, 0x6, 0x0, 0x0},
	{"xor", rm32, imm32, 0x81, 0x6, 0x0, 0x0},
	{"xor", rm8, imm8, 0x82, 0x6, 0x0, 0x0},
	{"xor", rm32, imm8, 0x83, 0x6, 0x0, 0x0},
//	{"xsave", m, edx, 0xae, 0x4, 0x0, 0x0f},
//	{"xsetbv", m, edx, 0xae, 0x4, 0x0, 0x0f},
};
#endif
