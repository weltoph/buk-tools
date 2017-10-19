EXENAME=whint
LEX=flex
LFLAGS=
YACC=bison
YFLAGS=-d
CC=gcc
CCFLAGS=
DEP=lex.yy.c while.tab.c while.tab.h prog.c prog.h statement.c statement.h
INTERMEDSUF=.yy.c .tab.c .tab.h

$(EXENAME) : $(DEP)
	$(CC) $(CCFLAGS) -lfl $(filter-out $(wildcard *.h),$(DEP)) -o ../$(EXENAME)

.PHONY: clean

lex.yy.c : while.l
	$(LEX) $(LFLAGS) while.l

%.tab.c %.tab.h : %.y
	$(YACC) $(YFLAGS) $<

clean :
	@rm $(addprefix *,$(INTERMEDSUF))
