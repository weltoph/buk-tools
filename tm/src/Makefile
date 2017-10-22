EXENAME=tmint
EXEPATH=../$(EXENAME)

LEX=flex
LFLAGS=

YACC=bison
YFLAGS=-d

CC=gcc
CCFLAGS=

LEXFILES=$(wildcard *.l)
YACCFILES=$(wildcard *.y)

LEXSRC=$(patsubst %.l,%.yy.c,$(LEXFILES))
YACCSRC=$(patsubst %.y,%.tab.c, $(YACCFILES)) $(patsubst %.y,%.tab.h, $(YACCFILES))

DEP=$(wildcard *.h *.c) $(LEXSRC) $(YACCSRC)
INTERMEDSUF=.yy.c .tab.c .tab.h


$(EXEPATH) : $(DEP)
	$(CC) $(CCFLAGS) -lfl *.c -o $(EXEPATH)

.PHONY: clean

%.yy.c : %.l
	$(LEX) $(LFLAGS) -o $@ $<

%.tab.c %.tab.h : %.y
	$(YACC) $(YFLAGS) $<


clean :
	@rm $(addprefix *,$(INTERMEDSUF))
	@rm $(EXEPATH)

