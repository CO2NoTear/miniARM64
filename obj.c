#include "obj.h"
#include "tac.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* global var */
int tos; /* top of static */
int tof; /* top of frame */
int oof; /* offset of formal */
int oon; /* offset of next frame */
struct rdesc rdesc[R_NUM];

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
	if ((rdesc[r].var != NULL) && rdesc[r].modified)
	{
		if (rdesc[r].var->store == 1) /* local var */
		{
			printf("	STO (R%u+%u),R%u\n", R_BP, rdesc[r].var->offset, r);
		}
		else /* global var */
		{
			printf("	LOD R%u,STATIC\n", R_TP);
			printf("	STO (R%u+%u),R%u\n", R_TP, rdesc[r].var->offset, r);
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
			printf("	LOD R%u,R%u\n", r, s);
			insert_desc(r, n, rdesc[s].modified);
			return;
		}
	}

	/* Not in a reg. Load appropriately */
	switch (n->type)
	{
	case SYM_INT:
		printf("	LOD R%u,%u\n", r, n->value);
		break;

	case SYM_VAR:
		if (n->store == 1) /* local var */
		{
			if ((n->offset) >= 0)
				printf("	LOD R%u,(R%u+%d)\n", r, R_BP, n->offset);
			else
				printf("	LOD R%u,(R%u-%d)\n", r, R_BP, -(n->offset));
		}
		else /* global var */
		{
			printf("	LOD R%u,STATIC\n", R_TP);
			printf("	LOD R%u,(R%u+%d)\n", r, R_TP, n->offset);
		}
		break;

	case SYM_TEXT:
		printf("	LOD R%u,L%u\n", r, n->label);
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

	printf("	%s R%u,R%u\n", op, reg1, reg2);

	/* Delete c from the descriptors and insert a */
	// clear_desc(reg1);
	// this code seems to be USELESS,
	// because insert_desc will not check
	// whether reg1 is obtained,
	// insert_desc simply overwrites it.

	insert_desc(reg1, a, MODIFIED);
}

void asm_cmp(int op, SYM *a, SYM *b, SYM *c)
{
	int reg1 = get_first_reg(b);				/* Result register */
	int reg2 = get_second_reg(c, reg1); /* One more register */

	printf("	SUB R%u,R%u\n", reg1, reg2);
	printf("	TST R%u\n", reg1);
	// TST: set FLAG_REG to FLAG_EZ/LZ/GZ if reg1 is zero/postive/negative

	// this are stable control flows of comparation
	switch (op)
	{
	case TAC_EQ:
		// jump reg <- PC+5*instruction_length, which is "LOD Rreg1, 1"
		printf("	LOD R3,R1+40\n");
		// if FLAG reg is FLAG_EZ, skip the remaining [LOD,LOD,JMP] instructions,
		// directly LOD reg1 True
		// otherwise, LOD reg1 False and skip the True instruction by assign PC+24.
		printf("	JEZ R3\n");
		printf("	LOD R%u,0\n", reg1);
		printf("	LOD R3,R1+24\n");
		printf("	JMP R3\n");
		printf("	LOD R%u,1\n", reg1);
		break;

	case TAC_NE:
		printf("	LOD R3,R1+40\n");
		printf("	JEZ R3\n");
		printf("	LOD R%u,1\n", reg1);
		printf("	LOD R3,R1+24\n");
		printf("	JMP R3\n");
		printf("	LOD R%u,0\n", reg1);
		break;

	case TAC_LT:
		printf("	LOD R3,R1+40\n");
		printf("	JLZ R3\n");
		printf("	LOD R%u,0\n", reg1);
		printf("	LOD R3,R1+24\n");
		printf("	JMP R3\n");
		printf("	LOD R%u,1\n", reg1);
		break;

	case TAC_LE:
		printf("	LOD R3,R1+40\n");
		printf("	JGZ R3\n");
		printf("	LOD R%u,1\n", reg1);
		printf("	LOD R3,R1+24\n");
		printf("	JMP R3\n");
		printf("	LOD R%u,0\n", reg1);
		break;

	case TAC_GT:
		printf("	LOD R3,R1+40\n");
		printf("	JGZ R3\n");
		printf("	LOD R%u,0\n", reg1);
		printf("	LOD R3,R1+24\n");
		printf("	JMP R3\n");
		printf("	LOD R%u,1\n", reg1);
		break;

	case TAC_GE:
		printf("	LOD R3,R1+40\n");
		printf("	JLZ R3\n");
		printf("	LOD R%u,1\n", reg1);
		printf("	LOD R3,R1+24\n");
		printf("	JMP R3\n");
		printf("	LOD R%u,0\n", reg1);
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

	if (a != NULL)
	{
		int r;

		for (r = R_GEN; r < R_NUM; r++) /* Is it in reg? */
		{
			if (rdesc[r].var == a)
				break;
		}

		if (r < R_NUM)
			printf("	TST R%u\n", r);
		else
			printf("	TST R%u\n", get_first_reg(a)); /* Load into new register */
	}

	printf("	%s %s\n", op, l);
}

void asm_return(SYM *a)
{
	if (a != NULL) /* return value */
	{
		spill_one(R_TP);
		load_reg(R_TP, a);
	}

	printf("	LOD R3,(R2+4)\n"); /* return address */
	printf("	LOD R2,(R2)\n");	 /* restore bp */
	printf("	JMP R3\n");				 /* return */
}

void asm_head()
{
	char head[] =
			"	# head\n"
			"	LOD R2,STACK\n"
			"	STO (R2),0\n"
			"	LOD R4,EXIT\n"
			"	STO (R2+4),R4";

	puts(head);
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

	printf("L%u:\n", s->label); /* Label for the string */
	printf("	DBS ");						/* Label for the string */

	for (i = 1; t[i + 1] != 0; i++)
	{
		if (t[i] == '\\')
		{
			switch (t[++i])
			{
			case 'n':
				printf("%u,", '\n');
				break;

			case '\"':
				printf("%u,", '\"');
				break;
			}
		}
		else
			printf("%u,", t[i]);
	}

	printf("0\n"); /* End of string */
}

void asm_static(void)
{
	int i;

	SYM *sl;

	for (sl = sym_tab_global; sl != NULL; sl = sl->next)
	{
		if (sl->type == SYM_TEXT)
			asm_str(sl);
	}

	printf("STATIC:\n");
	printf("	DBN 0,%u\n", tos);
	printf("STACK:\n");
}

void asm_code(TAC *c)
{
	int r;

	switch (c->op)
	{
	case TAC_UNDEF:
		error("cannot translate TAC_UNDEF");
		return;

	case TAC_ADD:
		asm_bin("ADD", c->a, c->b, c->c);
		return;

	case TAC_SUB:
		asm_bin("SUB", c->a, c->b, c->c);
		return;

	case TAC_MUL:
		asm_bin("MUL", c->a, c->b, c->c);
		return;

	case TAC_DIV:
		asm_bin("DIV", c->a, c->b, c->c);
		return;

	case TAC_NEG:
		asm_bin("SUB", c->a, mk_const(0), c->b);
		return;

	case TAC_EQ:
	case TAC_NE:
	case TAC_LT:
	case TAC_LE:
	case TAC_GT:
	case TAC_GE:
		asm_cmp(c->op, c->a, c->b, c->c);
		return;

	case TAC_COPY:
		asm_copy(c->a, c->b);
		return;

	case TAC_GOTO:
		asm_cond("JMP", NULL, c->a->name);
		return;

	case TAC_IFZ:
		asm_cond("JEZ", c->b, c->a->name);
		return;

	case TAC_LABEL:
		flush_all();
		printf("%s:\n", c->a->name);
		return;

	case TAC_ACTUAL:
		r = get_first_reg(c->a);
		printf("	STO (R2+%d),R%u\n", tof + oon, r);
		oon += 4;
		return;

	case TAC_CALL:
		flush_all();
		printf("	STO (R2+%d),R2\n", tof + oon); /* store old bp */
		oon += 4;
		printf("	LOD R4,R1+32\n");							 /* return addr: 4*8=32 */
		printf("	STO (R2+%d),R4\n", tof + oon); /* store return addr */
		oon += 4;
		printf("	LOD R2,R2+%d\n", tof + oon - 8); /* load new bp */
		printf("	JMP %s\n", (char *)c->b);				 /* jump to new func */
		if (c->a != NULL)
			insert_desc(R_TP, c->a, MODIFIED);
		oon = 0;
		return;

	case TAC_BEGINFUNC:
		/* We reset the top of stack, since it is currently empty apart from the link information. */
		scope_local = 1;
		tof = LOCAL_OFF;
		oof = FORMAL_OFF;
		oon = 0;
		return;

	case TAC_FORMAL:
		c->a->store = 1; /* parameter is special local var */
		c->a->offset = oof;
		oof -= 4;
		return;

	case TAC_VAR:
		if (scope_local)
		{
			c->a->store = 1; /* local var */
			c->a->offset = tof;
			tof += 4;
		}
		else
		{
			c->a->store = 0; /* global var */
			c->a->offset = tos;
			tos += 4;
		}
		return;

	case TAC_RETURN:
		asm_return(c->a);
		return;

	case TAC_ENDFUNC:
		asm_return(NULL);
		scope_local = 0;
		return;

	default:
		/* Don't know what this one is */
		error("unknown TAC opcode to translate");
		return;
	}
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

	TAC *cur;
	for (cur = tac_first; cur != NULL; cur = cur->next)
	{
		printf("\n	# ");
		tac_print(cur);
		printf("\n");
		asm_code(cur);
	}
	asm_lib();
	asm_static();
}
