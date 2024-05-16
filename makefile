SAMPLE_DIR = /home/test/CrazyDriver/mini_complete/mini/sample
MINI = /home/test/CrazyDriver/mini_complete/mini/mini
FUNC_M = $(SAMPLE_DIR)/func.m
FUNC_M_S =$(SAMPLE_DIR)/func.m.s
FUNC_M_S_O = $(SAMPLE_DIR)/func.m.s.o

ASSIGN_M = $(SAMPLE_DIR)/assign.m
ASSIGN_M_S =$(SAMPLE_DIR)/assign.m.s
ASSIGN_M_S_O = $(SAMPLE_DIR)/assign.m.s.o

all: mini asm machine

mini: mini.l mini.y tac.c tac.h obj.c obj.h
	lex -o mini.l.c mini.l
	yacc -d -o mini.y.c mini.y
	gcc -g3 mini.l.c mini.y.c tac.c obj.c opt.c opt.h DFA.c DFA.h -o mini

asm: asm.l asm.y opcode.h
	lex -o asm.l.c asm.l
	yacc -d -o asm.y.c asm.y
	gcc -g3 asm.l.c asm.y.c -o asm

machine: machine.c opcode.h
	gcc -g3 machine.c -o machine

clean:
	rm -fr *.l.* *.y.* mini asm machine

func.m: all
	$(MINI) $(FUNC_M) > $(FUNC_M_S)
	gcc -g $(FUNC_M_S) -o $(FUNC_M_S_O)
	chmod +x $(FUNC_M_S_O)

assign.m: all
	$(MINI) $(ASSIGN_M) > $(ASSIGN_M_S)
	gcc -g $(ASSIGN_M_S) -o $(ASSIGN_M_S_O)
	chmod +x $(ASSIGN_M_S_O)

remake: clean all
