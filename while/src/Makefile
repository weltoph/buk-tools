EXENAME=whint
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
DEP=$(wildcard *.c *.h) $(LEXSRC) $(YACCSRC)
INTERMEDSUF=.yy.c .tab.c .tab.h

$(EXENAME) : $(DEP)
	$(CC) $(CCFLAGS) -lfl $(filter-out $(wildcard *.h),$(DEP)) -o ../$(EXENAME)

.PHONY: clean

%.yy.c : %.l
	$(LEX) $(LFLAGS) -o $@ $<

%.tab.c %.tab.h : %.y
	$(YACC) $(YFLAGS) $<

clean :
	@rm $(addprefix *,$(INTERMEDSUF))
