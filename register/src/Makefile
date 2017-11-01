EXENAME=reint
EXEPATH=../$(EXENAME)

LEX=flex
LFLAGS=

YACC=bison
YFLAGS=-d

CC=gcc
CCFLAGS=-Wall -Wpedantic -Werror -Wextra -g

UTILPATH=../../util/

LEXFILES=$(wildcard *.l)
YACCFILES=$(wildcard *.y)
UTILSRC=$(wildcard $(UTILPATH)*.c $(UTILPATH)*.h)

LEXSRC=$(patsubst %.l,%.yy.c,$(LEXFILES))
YACCSRC=$(patsubst %.y,%.tab.c, $(YACCFILES)) $(patsubst %.y,%.tab.h, $(YACCFILES))

DEP=$(wildcard *.h *.c) $(LEXSRC) $(YACCSRC) $(UTILSRC)
INTERMEDSUF=.yy.c .tab.c .tab.h


$(EXEPATH) : $(DEP)
	$(CC) $(CCFLAGS) -lfl *.c $(UTILPATH)*.c -o $(EXEPATH)

.PHONY: clean rebuild

%.yy.c : %.l
	$(LEX) $(LFLAGS) -o $@ $<

%.tab.c %.tab.h : %.y
	$(YACC) $(YFLAGS) $<


clean :
	@rm -f $(addprefix *,$(INTERMEDSUF))
	@rm -f vgcore*
	@rm -f $(EXEPATH)

rebuild :
	$(MAKE) clean
	$(MAKE)
