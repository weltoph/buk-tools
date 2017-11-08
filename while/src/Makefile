EXENAME=whint
EXEPATH=../$(EXENAME)

LEX=flex
LFLAGS=

YACC=bison
YFLAGS=-d

CC=gcc
CCFLAGS=-Wall -Wpedantic -Wextra

LEXFILES=$(wildcard *.l)
YACCFILES=$(wildcard *.y)

LEXSRC=$(patsubst %.l,%.yy.c,$(LEXFILES))
YACCSRC=$(patsubst %.y,%.tab.c, $(YACCFILES)) $(patsubst %.y,%.tab.h, $(YACCFILES))

DEP=$(wildcard *.h *.c) $(LEXSRC) $(YACCSRC)
INTERMEDSUF=.yy.c .tab.c .tab.h


$(EXEPATH) : $(DEP)
	$(CC) $(CCFLAGS) -Werror *.c -o $(EXEPATH)

.PHONY: clean rebuild dirty

%.yy.c : %.l
	$(LEX) $(LFLAGS) -o $@ $<

%.tab.c %.tab.h : %.y
	$(YACC) $(YFLAGS) $<

clean : $(DEP)
	@rm $(addprefix *,$(INTERMEDSUF))
	@rm $(EXEPATH)

rebuild : $(DEP)
	$(MAKE) clean
	$(MAKE)

dirty : $(DEP)
	$(CC) $(CCFLAGS) *.c -o $(EXEPATH)

