#include "obj.h"
#include "tac.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* global var */
int tos;					/* top of static */
int tof;					/* top of frame */
int oof;					/* offset of formal */
int oon;					/* offset of next frame */
int frame_offset; // positive offset from sp(tof)
int sp;
int arg_num; /* current actual argment input num*/
int arg_stack[32];
struct rdesc rdesc[R_NUM];
char *function_name, *possible_function_name;
TAC *cur;

SYM *copy_sym(SYM *t)
{
	SYM *res = (SYM *)malloc(sizeof(SYM));
	res->address = t->address;
	res->label = t->label;
	res->name = t->name;
	res->offset = t->offset;
	res->store = t->store;
	res->type = t->type;
	res->value = t->value;
	return res;
}

void clear_desc(int r)
{
	rdesc[r].var = NULL;
}

void insert_desc(int r, SYM *n, int mod)
{
	/* Search through each register in turn looking for "n". There should be at most one of these. */
	int or ; /* Old descriptor */
	for (or = R_GEN; or < R_NUM; or ++)
	{
		if (rdesc[or].var == n)
		{
			/* Found it, clear it and break out of the loop. */
			clear_desc(or);
			break;
		}
	}

	/* Insert "n" in the new descriptor */

	rdesc[r].var = n;
	rdesc[r].modified = mod;
}

void spill_one(int r)
{
	// if ((rdesc[r].var != NULL) && rdesc[r].modified && !rdesc[r].var->temp_flag)
	if ((rdesc[r].var != NULL) && rdesc[r].modified)
	{
		if (rdesc[r].var->temp_flag && rdesc[r].var->offset == -1)
		{
			return;
		}
		if (rdesc[r].var->store || rdesc[r].var->temp_flag) /* local var */
		{
			if (rdesc[r].var->offset == 0)
				printf("	str x%u,[%s]\n", r, "sp");
			else
				printf("	str x%u,[%s, %u]\n", r, "sp", rdesc[r].var->offset);
		}
		else /* global var */
		{
			printf("	ldr x%u, %s\n", R_TP, rdesc[r].var->name);
			// printf("	LOD x%u,STATIC\n", R_TP);
			if (rdesc[r].var->type == SYM_TEXT)
				printf("	str x%u,[x%u]\n", r, R_TP);
			// printf("	STO (x%u+%u),x%u\n", R_TP, rdesc[r].var->offset, r);
		}
		rdesc[r].modified = UNMODIFIED;
	}
}

void spill_all(void)
{
	int r;
	for (r = R_GEN; r < R_NUM; r++)
		spill_one(r);
}

void flush_all(void)
{
	int r;

	spill_all();

	for (r = R_GEN; r < R_NUM; r++)
		clear_desc(r);

	clear_desc(R_TP); /* Clear result register */
}

void load_reg(int r, SYM *n)
{
	int s;

	/* Look for a register */
	for (s = 0; s < R_NUM; s++)
	{
		if (rdesc[s].var == n)
		{
			if (s == r) // do nothing.
				return;
			// printf("	LOD x%u,x%u\n", r, s);
			printf("	mov x%u,x%u\n", r, s);
			insert_desc(r, n, rdesc[s].modified);
			return;
		}
	}

	/* Not in a reg. Load appropriately */
	switch (n->type)
	{
	case SYM_INT:
		// printf("	LOD x%u,%u\n", r, n->value);
		if (n->value < 1 << 16)
		{
			printf("	ldr x%u,=%d\n", r, n->value);
			break;
		}
		else
		{
			// TODO: 64bit load
			//  printf("  ")
		}

	case SYM_VAR:
		if (n->store == 1 || n->temp_flag) /* local var */
		{
			if (n->offset == -1)
				break;
			if (n->offset == 0)
				printf("	ldr x%u,[sp]\n", r);
			else
				printf("	ldr x%u,[sp, %d]\n", r, n->offset);
			// if ((n->offset) >= 0)
			// 	// printf("	LOD x%u,(x%u+%d)\n", r, R_BP, n->offset);
			// 	printf("	ldr x%u,[sp, %d]\n", r, n->offset);
			// else
			// 	// printf("	LOD x%u,(x%u-%d)\n", r, R_BP, -(n->offset));
			// 	printf("	ldr x%u,[sp, %d]\n", r, -(n->offset));
		}
		else /* global var */
		{
			// printf("	LOD x%u,STATIC\n", R_TP);
			// printf("	LOD x%u,(x%u+%d)\n", r, R_TP, n->offset);
			// printf("	adr x%u,static\n", R_TP);
			// printf("	ldr x%u,[x%u, %d]\n", r, R_TP, n->offset);

			// reading label of n
			printf("	adrp x%u, %s\n", r, n->name);
			printf("	add x%u, x%u, :lo12:%s\n", r, r, n->name);
			printf("  ldr x%u, [x%u]\n", r, r);
		}
		break;
	case SYM_ARR:
		if (n->store == 1) /* local var */
		{
			printf("  add x%u, sp, %d\n", r, n->offset);
			// if ((n->offset) >= 0)
			// 	// printf("	LOD x%u,(x%u+%d)\n", r, R_BP, n->offset);
			// 	printf("	ldr x%u,[sp, %d]\n", r, n->offset);
			// else
			// 	// printf("	LOD x%u,(x%u-%d)\n", r, R_BP, -(n->offset));
			// 	printf("	ldr x%u,[sp, %d]\n", r, -(n->offset));
		}
		else /* global var */
		{
			// printf("	LOD x%u,STATIC\n", R_TP);
			// printf("	LOD x%u,(x%u+%d)\n", r, R_TP, n->offset);
			// printf("	adr x%u,static\n", R_TP);
			// printf("	ldr x%u,[x%u, %d]\n", r, R_TP, n->offset);

			// reading label of n
			printf("	adrp x%u, %s\n", r, n->name);
			printf("	add x%u, x%u, :lo12:%s\n", r, r, n->name);
			printf("  ldr x%u, [x%u]", r, r);
		}
		break;

	case SYM_TEXT:
		printf("	adrp x%u, str%u\n", r, n->label);
		printf("	add x%u, x%u, :lo12:str%u\n", r, r, n->label);
		// printf("	adr x%u,str%u\n", r, n->label);
		break;
	}

	insert_desc(r, n, UNMODIFIED);
}

/* Get the first reg as a destination reg. */
int get_first_reg(SYM *c)
{
	int r;
	for (r = R_GEN; r < R_NUM; r++) /* Already in a register */
	{
		if (rdesc[r].var == c)
		{
			spill_one(r);
			return r;
		}
	}

	for (r = R_GEN; r < R_NUM; r++)
	{
		if (rdesc[r].var == NULL) /* Empty register */
		{
			load_reg(r, c);
			return r;
		}
	}

	for (r = R_GEN; r < R_NUM; r++)
	{
		if (!rdesc[r].modified) /* Unmodifed register */
		{
			clear_desc(r);
			load_reg(r, c);
			return r;
		}
	}

	spill_one(R_GEN); /* Modified register */
	clear_desc(R_GEN);
	load_reg(R_GEN, c);
	return R_GEN;
}

/* Get the second reg as a source reg. Exclude the first reg. */
int get_second_reg(SYM *b, int first_reg)
{
	int r;
	for (r = R_GEN; r < R_NUM; r++)
	{
		if (rdesc[r].var == b) /* Already in register */
			return r;
	}

	for (r = R_GEN; r < R_NUM; r++)
	{
		if (rdesc[r].var == NULL) /* Empty register */
		{
			load_reg(r, b);
			return r;
		}
	}

	for (r = R_GEN; r < R_NUM; r++)
	{
		if (!rdesc[r].modified && (r != first_reg)) /* Unmodifed register */
		{
			clear_desc(r);
			load_reg(r, b);
			return r;
		}
	}

	for (r = R_GEN; r < R_NUM; r++)
	{
		if (r != first_reg) /* Modified register */
		{
			spill_one(r);
			clear_desc(r);
			load_reg(r, b);
			return r;
		}
	}
}

void asm_bin(char *op, SYM *a, SYM *b, SYM *c)
{
	int reg1 = get_first_reg(b);				/* Result register */
	int reg2 = get_second_reg(c, reg1); /* One more register */

	printf("	%s x%u,x%u,x%u\n", op, reg1, reg1, reg2);

	/* Delete c from the descriptors and insert a */

	// clear_desc(reg1);
	// ^^^^^^^^^^
	// this code seems to be USELESS,
	// because insert_desc will not check
	// whether reg1 is obtained,
	// insert_desc simply overwrites it.

	insert_desc(reg1, a, MODIFIED);
}

void asm_cmp(int op, SYM *a, SYM *b, SYM *c)
{
	int static label_cnt = 0;
	char label_cmp_a[20];
	char label_cmp_b[20];
	sprintf(label_cmp_a, "cmp_a%d", label_cnt);
	sprintf(label_cmp_b, "cmp_b%d", label_cnt);
	++label_cnt;

	int reg1 = get_first_reg(b);				/* Result register */
	int reg2 = get_second_reg(c, reg1); /* One more register */

	// printf("	SUB x%u,x%u\n", reg1, reg2);
	// printf("	TST x%u\n", reg1);
	// TST: set FLAG_REG to FLAG_EZ/LZ/GZ if reg1 is zero/postive/negative

	// jump reg <- PC+5*instruction_length, which is "LOD Rreg1, 1"
	// printf("	ldr R3,[pc+#40]\n");
	printf("	cmp x%u,x%u\n", reg1, reg2);

	// this are stable control flows of comparation
	switch (op)
	{
	case TAC_EQ:
		// if FLAG reg is FLAG_EZ, skip the remaining [LOD,LOD,JMP] instructions,
		// directly LOD reg1 True
		// otherwise, LOD reg1 False and skip the True instruction by assign PC+24.
		printf("	beq %s\n", label_cmp_a);
		printf("	mov x%u,0\n", reg1);
		printf("	b %s\n", label_cmp_b);
		printf("%s:", label_cmp_a);
		printf("	mov x%u,1\n", reg1);
		printf("%s:", label_cmp_b);
		break;

	case TAC_NE:
		// printf("	LOD R3,R1+40\n");
		// printf("	JEZ R3\n");
		// printf("	LOD x%u,1\n", reg1);
		// printf("	LOD R3,R1+24\n");
		// printf("	JMP R3\n");
		// printf("	LOD x%u,0\n", reg1);
		printf("	beq %s\n", label_cmp_a);
		printf("	mov x%u,1\n", reg1);
		printf("	b %s\n", label_cmp_b);
		printf("%s:", label_cmp_a);
		printf("	mov x%u,0\n", reg1);
		printf("%s:", label_cmp_b);
		break;

	case TAC_LT:
		// printf("	LOD R3,R1+40\n");
		// printf("	JLZ R3\n");
		// printf("	LOD x%u,0\n", reg1);
		// printf("	LOD R3,R1+24\n");
		// printf("	JMP R3\n");
		// printf("	LOD x%u,1\n", reg1);
		printf("	blt %s\n", label_cmp_a);
		printf("	mov x%u,0\n", reg1);
		printf("	b %s\n", label_cmp_b);
		printf("%s:", label_cmp_a);
		printf("	mov x%u,1\n", reg1);
		printf("%s:", label_cmp_b);
		break;

	case TAC_LE:
		// printf("	LOD R3,R1+40\n");
		// printf("	JGZ R3\n");
		// printf("	LOD x%u,1\n", reg1);
		// printf("	LOD R3,R1+24\n");
		// printf("	JMP R3\n");
		// printf("	LOD x%u,0\n", reg1);
		printf("	ble %s\n", label_cmp_a);
		printf("	mov x%u,0\n", reg1);
		printf("	b %s\n", label_cmp_b);
		printf("%s:", label_cmp_a);
		printf("	mov x%u,1\n", reg1);
		printf("%s:", label_cmp_b);
		break;

	case TAC_GT:
		// printf("	LOD R3,R1+40\n");
		// printf("	JGZ R3\n");
		// printf("	LOD x%u,0\n", reg1);
		// printf("	LOD R3,R1+24\n");
		// printf("	JMP R3\n");
		// printf("	LOD x%u,1\n", reg1);
		printf("	bgt %s\n", label_cmp_a);
		printf("	mov x%u,0\n", reg1);
		printf("	b %s\n", label_cmp_b);
		printf("%s:", label_cmp_a);
		printf("	mov x%u,1\n", reg1);
		printf("%s:", label_cmp_b);
		break;

	case TAC_GE:
		// printf("	LOD R3,R1+40\n");
		// printf("	JLZ R3\n");
		// printf("	LOD x%u,1\n", reg1);
		// printf("	LOD R3,R1+24\n");
		// printf("	JMP R3\n");
		// printf("	LOD x%u,0\n", reg1);
		printf("	bge %s\n", label_cmp_a);
		printf("	mov x%u,0\n", reg1);
		printf("	b %s\n", label_cmp_b);
		printf("%s:", label_cmp_a);
		printf("	mov x%u,1\n", reg1);
		printf("%s:", label_cmp_b);
		break;
	}

	/* Delete c from the descriptors and insert a */
	clear_desc(reg1);
	insert_desc(reg1, a, MODIFIED);
}

//
void asm_copy(SYM *a, SYM *b)
{
	int reg1 = get_first_reg(b);		/* Load b into a register */
	insert_desc(reg1, a, MODIFIED); /* Indicate a is there */
}

void asm_cond(char *op, SYM *a, char *l)
{
	spill_all();

	if (a == NULL)
		// unconditional branch

		printf("	%s %s\n", op, l);

	// condition branch
	if (a != NULL)
	{
		int r;

		for (r = R_GEN; r < R_NUM; r++) /* Is it in reg? */
		{
			if (rdesc[r].var == a)
				break;
		}

		if (r < R_NUM)
			// in the regfile
			printf("	%s x%u, %s\n", op, r, l);
		else
			printf("	%s x%u, %s\n", op, get_first_reg(a), l);

		return;
	}
}
unsigned int asm_function_var_size(TAC *code)
{
	unsigned int size = 0;
	for (TAC *current_code = code; current_code->op != TAC_ENDFUNC; current_code = current_code->next)
	{
		if ((current_code->op == TAC_ARR || current_code->op == TAC_VAR || current_code->op == TAC_FORMAL) && current_code->a->temp_flag != 1)
		{
			switch (current_code->a->type)
			{
			case SYM_VAR:
				// size += sizeof(long);
				size += 8;
				break;
			case SYM_ARR:
				// size += sizeof(long);
				size += 8 * current_code->b->value;
				break;
			case SYM_UNDEF:
				fprintf(stderr, "[ERROR] UNDEFINED SYMBOL IN FUNCTION\n");
				exit(-1);

			default:
				break;
			}
		}
	}
	return size;
}
void asm_begin_func(unsigned int size)
{
	/* We reset the top of stack, since it is currently empty apart from the link information. */
	scope_local = 1;
	// relative to base
	tof = LOCAL_OFF;
	oon = 0;
	unsigned int stack_size = stack_alloc(size + abs(tof));
	tof = -stack_size; // sp (negative)
	oof = -tof;				 // base (positive)
										 // sp+base = initialization point

	frame_offset = -LOCAL_OFF; // starting from 16, skipping x29 and x30
	printf("	stp x29, x30, [sp]");
}
void asm_return(SYM *a)
{
	// if (a != NULL) /* return value */
	// {
	// 	spill_one(R_TP);
	// 	load_reg(R_TP, a);
	// }

	// printf("	LOD R3,(R2+%u)\n", REG_WIDTH); /* return address */
	// printf("	LOD R2,(R2)\n");							 /* restore bp */
	// printf("	JMP R3\n");										 /* return */

	printf("  ldp x29, x30, [sp]\n");
	stack_free(abs(tof));
	printf("  ret\n");
}

void asm_head()
{
	// char head[] =
	// 		"	# head\n"
	// 		"	LOD R2,STACK\n"
	// 		"	STO (R2),0\n"
	// 		"	LOD R4,EXIT\n"
	// 		"	STO (R2+4),R4";
	char head[] =
			"  .arch armv8-a\n"
			"  .text\n";
	// "  .global main\n"
	// "main:";

	puts(head);
	asm_static();
}

void asm_lib()
{
	char lib[] =
			"\nPRINTN:\n"
			"	LOD R7,(R2-4) # 789\n"
			"	LOD R15,R7 # 789 \n"
			"	DIV R7,10 # 78\n"
			"	TST R7\n"
			"	JEZ PRINTDIGIT\n"
			"	LOD R8,R7 # 78\n"
			"	MUL R8,10 # 780\n"
			"	SUB R15,R8 # 9\n"
			"	STO (R2+8),R15 # local 9 store\n"
			"\n	# out 78\n"
			"	STO (R2+12),R7 # actual 78 push\n"
			"\n	# call PRINTN\n"
			"	STO (R2+16),R2\n"
			"	LOD R4,R1+32\n"
			"	STO (R2+20),R4\n"
			"	LOD R2,R2+16\n"
			"	JMP PRINTN\n"
			"\n	# out 9\n"
			"	LOD R15,(R2+8) # local 9 \n"
			"\nPRINTDIGIT:\n"
			"	ADD  R15,48\n"
			"	OUT\n"
			"\n	# ret\n"
			"	LOD R3,(R2+4)\n"
			"	LOD R2,(R2)\n"
			"	JMP R3\n"
			"\nPRINTS:\n"
			"	LOD R7,(R2-4)\n"
			"\nPRINTC:\n"
			"	LOD R15,(R7)\n"
			"	DIV R15,16777216\n"
			"	TST R15\n"
			"	JEZ PRINTSEND\n"
			"	OUT\n"
			"	ADD R7,1\n"
			"	JMP PRINTC\n"
			"\nPRINTSEND:\n"
			"	# ret\n"
			"	LOD R3,(R2+4)\n"
			"	LOD R2,(R2)\n"
			"	JMP R3\n"

			"\n"
			"EXIT:\n"
			"	END\n";

	puts(lib);
}

void asm_str(SYM *s)
{
	char *t = s->name; /* The text */
	int i;

	printf("str%u:\n", s->label); /* Label for the string */
	printf("	.asciz %s\n", t);

	// for (i = 1; t[i + 1] != 0; i++)
	// {
	// 	if (t[i] == '\\')
	// 	{
	// 		switch (t[++i])
	// 		{
	// 		case 'n':
	// 			printf("%u,", '\n');
	// 			break;

	// 		case '\"':
	// 			printf("%u,", '\"');
	// 			break;
	// 		}
	// 	}
	// 	else
	// 		printf("%c,", t[i]);
	// }

	// printf("0\n"); /* End of string */
}

void asm_rodata(SYM *rodata_sym_list)
{
	printf("	.section .rodata\n// Read-only data section\n");
	for (SYM *sl = rodata_sym_list; sl != NULL; sl = sl->next)
		asm_str(sl);
}

void asm_static(void)
{
	int i;

	SYM *sl, *rodata_sym_list_head, *next_rodata;
	rodata_sym_list_head = NULL;

	printf("	.data\n");
	for (sl = sym_tab_global; sl != NULL; sl = sl->next)
	{
		switch (sl->type)
		{
		case SYM_TEXT:
			next_rodata = copy_sym(sl);
			next_rodata->next = rodata_sym_list_head;
			rodata_sym_list_head = next_rodata;
			break;
		case SYM_VAR:
			printf("  .global %s\n", sl->name);
			printf("  .type %s, %%object\n", sl->name);
			printf("  .size %s, %u\n", sl->name, sizeof(int));
			printf("%s:\n", sl->name);
			printf("  .word %u\n", sl->value);
			break;
		case SYM_ARR:
			printf("  .global %s\n", sl->name);
			printf("  .type %s, %%object\n", sl->name);
			printf("  .size %s, %u\n", sl->name, sizeof(int) * sl->size);
			printf("%s:\n", sl->name);
			printf("  .word %u\n", sl->value);
			break;

		default:
			break;
		}
	}
	asm_rodata(rodata_sym_list_head);

	// printf("STATIC:\n");
	// printf("	DBN 0,%u\n", tos);
	// printf("STACK:\n");
}

void asm_code(TAC *code)
{
	int r;
	unsigned int size = 0;
	unsigned int formal_arg = 0;
	TAC *prev_cur;
	int single_var_reg;

	switch (code->op)
	{
	case TAC_UNDEF:
		error("cannot translate TAC_UNDEF");
		return;

	case TAC_ADD:
		// asm_bin("ADD", code->a, code->b, code->c);
		asm_bin("add", code->a, code->b, code->c);
		return;

	case TAC_SUB:
		asm_bin("sub", code->a, code->b, code->c);
		return;

	case TAC_MUL:
		asm_bin("mul", code->a, code->b, code->c);
		return;

	case TAC_DIV:
		asm_bin("sdiv", code->a, code->b, code->c);
		return;

	case TAC_NEG:
		asm_bin("sub", code->a, mk_const(0), code->b);
		return;

	case TAC_EQ:
	case TAC_NE:
	case TAC_LT:
	case TAC_LE:
	case TAC_GT:
	case TAC_GE:
		asm_cmp(code->op, code->a, code->b, code->c);
		return;

	case TAC_COPY:
		asm_copy(code->a, code->b);
		return;

	case TAC_GOTO:
		// uncondition
		asm_cond("b", NULL, code->a->name);
		return;

	case TAC_IFZ:
		// condition
		asm_cond("cbz", code->b, code->a->name);
		return;

	case TAC_LABEL:
		flush_all();
		// assume only function_end need it.
		possible_function_name = strdup(code->a->name);
		printf("	.text\n");
		printf("	.global %s\n", code->a->name);
		printf("%s:\n", code->a->name);
		return;

	case TAC_ACTUAL:
		// printf("#ACUTAL %d\t", arg_num);
		// tac_print(code);
		if (arg_num < 8)
		{
			spill_one(arg_num);
			load_reg(arg_num, code->a);
			insert_desc(arg_num, code->a, UNMODIFIED);
		}
		// store in stack
		else if (arg_num + 1 < R_NUM)
		{
			// skip the reg8
			spill_one(arg_num + 1);
			load_reg(arg_num + 1, code->a);
			oon += REG_WIDTH;
			arg_stack[arg_num - 8] = arg_num + 1;
		}
		arg_num++;
		return;

	case TAC_CALL:
		// printf("CALL %s\n", code->id);
		// tac_print(code);
		flush_all();

		if (arg_num > 8)
		{
			unsigned int space = stack_alloc(oon);
			// read from stack
			while (arg_num > 8)
			{
				printf("  str x%u, [sp, #%u]", arg_stack[(--arg_num) - 8], /*offset*/ oon - space);
			}
		}

		printf("  bl %s", (char *)code->b->name);

		// return val
		if (code->a != NULL)
		{
			insert_desc(R_TP, code->a, MODIFIED);
		}

		oon = 0;
		arg_num = 0;
		return;

	case TAC_BEGINFUNC:
		function_name = possible_function_name;
		size = asm_function_var_size(code);
		asm_begin_func(size);
		return;

	case TAC_FORMAL:
		while (cur->op == TAC_FORMAL && cur->next != NULL)
		{
			if ((++formal_arg) <= 8)
			{
				insert_desc(formal_arg - 1, cur->a, MODIFIED);
			}
			else
			{
				cur->a->store = 1; /* parameter is special local var */
				cur->a->offset = oof;
				oof += REG_WIDTH;
			}
			// save in stack
			cur->a->store = 1; /* local var */
			cur->a->offset = frame_offset;
			frame_offset += REG_WIDTH;

			prev_cur = cur;
			cur = cur->next;
			printf("  # ");
			tac_print(cur);
			putchar('\n');
		}
		// rollback 1 code, because main loop increments that.
		cur = prev_cur;
		return;

	case TAC_VAR:
		if (code->a->temp_flag)
		{
			return;
		}
		if (scope_local)
		{
			code->a->store = 1; /* local var */
			code->a->offset = frame_offset;
			frame_offset += REG_WIDTH;
		}
		else
		// global var is marked with label.
		{
			code->a->store = 0; /* global var */
													// code->a->offset = tos;
													// tos += REG_OFFSET_STEP;
		}
		return;

	case TAC_ARR:
		if (scope_local)
		{
			code->a->store = 1; /* local var */
			code->a->offset = frame_offset;
			load_reg(R_TP, code->a);
			spill_one(R_TP);
			frame_offset += REG_WIDTH * code->b->value;
		}
		else
		// global var is marked with label.
		{
			code->a->store = 0; /* global var */
													// code->a->offset = tos;
													// tos += REG_OFFSET_STEP;
		}
		return;

	case TAC_COPY_LARR:
		// base address of arr
		load_reg(R_BP, code->a);
		single_var_reg = get_second_reg(code->c, R_BP);
		printf("  str x%u, [x%u, #%d]", single_var_reg, R_BP, code->b->value * 8);
		return;

	case TAC_COPY_RARR:
		load_reg(R_BP, code->b);
		code->a->offset = code->b->offset + code->c->value * 8;
		single_var_reg = get_second_reg(code->a, R_BP);
		printf("  ldr x%u, [x%u, #%d]", single_var_reg, R_BP, code->c->value * 8);
		return;

	case TAC_RETURN:
		spill_one(R_TP);
		load_reg(R_TP, code->a);
		return;

	case TAC_ENDFUNC:
		asm_return(NULL);
		scope_local = 0;
		printf(".size %s, .-%s\n", function_name, function_name);
		return;

	default:
		/* Don't know what this one is */
		error("unknown TAC opcode to translate");
		return;
	}
}

// return actually allocated bytes that are aligned to 16bytes
unsigned int stack_alloc(unsigned int bytes)
{
	if ((bytes & 0b1111) != 0)
	{
		// not aligned to 16bytes, do the alignment
		bytes >>= 4;
		bytes += 1;
		bytes <<= 4;
	}
	printf("  add sp, sp, #-%u\n", bytes);
	return bytes;
}

// unaligned data will not be accept
void stack_free(unsigned int bytes)
{
	if ((bytes & 0b1111) != 0)
	{
		printf("ERROR! Invalid free bytes!\nDO THE ALIGNMENT FIRST");
	}
	printf("  add sp, sp, #%u\n", bytes);
}

void tac_obj()
{
	tof = LOCAL_OFF; /* TOS allows space for link info */
	oof = FORMAL_OFF;
	oon = 0;

	int r;
	for (r = 0; r < R_NUM; r++)
		rdesc[r].var = NULL;
	insert_desc(0, mk_const(0), UNMODIFIED); /* R0 holds 0 */

	asm_head();

	for (cur = tac_first; cur != NULL; cur = cur->next)
	{
		printf("\n	# ");
		tac_print(cur);
		printf("\n");
		asm_code(cur);
	}
	// asm_lib();
	// asm_static();
}
