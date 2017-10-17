EXENAME=whint
LEX=flex
LFLAGS=
YACC=bison
YFLAGS=-d
CC=gcc
CCFLAGS=
DEP=lex.yy.c while.tab.c while.tab.h prog.c prog.h
INTERMEDSUF=.yy.c .tab.c .tab.h

$(EXENAME) : $(DEP)
	$(CC) $(CCFLAGS) -lfl $(filter-out $(wildcard *.h),$(DEP)) -o ../$(EXENAME)
	$(MAKE) clean

.PHONY: clean

lex.yy.c : while.l
	$(LEX) $(LFLAGS) while.l

while.tab.c while.tab.h : while.y
	$(YACC) $(YFLAGS) while.y

clean :
	@rm $(addprefix *,$(INTERMEDSUF))
