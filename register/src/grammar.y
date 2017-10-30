%{
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "prog.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);

%}


%code requires {
#include "grammar.tab.h"
}

%code {
}

%union {
  Instruction instruction;
  Cmp_Type comparison_type;
  char name[30];
  uint8_t value;
}

%token <instruction> INSTRUCTION
%token <comparison_type> CMP
%token END
%token GOTO
%token IF
%token THEN
%token <value> VALUE
%token <name> LABEL
%token DELIMITER

%%

%%

void yyerror(const char *s)
{
  printf("\nAn error occurred: %s\n", s);
}

