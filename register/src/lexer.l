/* Scanner for a Turingmachine specification language */
%{
#include <stdlib.h> /* atoi for identifier index */
#include <stdint.h>
#include <stdio.h>
#include "grammar.tab.h"
%}

%option noinput
%option nounput
%option lineno

VALUE [0-9]|[1-9][0-9]+
LABELCHAR [0-9a-z]

%%

[ \t]

{VALUE} {
  yylval.value = (uint8_t)(atoi(yytext));
  return VALUE;
}

${LABELCHAR}{1,29} {
  strcpy(yylval.name, yytext);
  return LABEL;
}

LOAD {
  yylval.instruction = { .type = LOAD, .variant = STANDARD }
  return INSTRUCTION;
}

STORE {
  yylval.instruction = { .type = STORE, .variant = STANDARD }
  return INSTRUCTION;
}

ADD {
  yylval.instruction = { .type = ADD, .variant = STANDARD }
  return INSTRUCTION;
}

SUB {
  yylval.instruction = { .type = SUB, .variant = STANDARD }
  return INSTRUCTION;
}

MULT {
  yylval.instruction = { .type = MULT, .variant = STANDARD }
  return INSTRUCTION;
}


DIV {
  yylval.instruction = { .type = DIV, .variant = STANDARD }
  return INSTRUCTION;
}

INDLOAD {
  yylval.instruction = { .type = LOAD, .variant = INDIRECT }
  return INSTRUCTION;
}

INDSTORE {
  yylval.instruction = { .type = STORE, .variant = INDIRECT }
  return INSTRUCTION;
}

INDADD {
  yylval.instruction = { .type = ADD, .variant = INDIRECT }
  return INSTRUCTION;
}

INDSUB {
  yylval.instruction = { .type = SUB, .variant = INDIRECT }
  return INSTRUCTION;
}

INDMULT {
  yylval.instruction = { .type = MULT, .variant = INDIRECT }
  return INSTRUCTION;
}

INDDIV {
  yylval.instruction = { .type = DIV, .variant = INDIRECT }
  return INSTRUCTION;
}

CLOAD {
  yylval.instruction = { .type = LOAD, .variant = CONSTANT }
  return INSTRUCTION;
}

CADD {
  yylval.instruction = { .type = ADD, .variant = CONSTANT }
  return INSTRUCTION;
}

CSUB {
  yylval.instruction = { .type = SUB, .variant = CONSTANT }
  return INSTRUCTION;
}

CMULT {
  yylval.instruction = { .type = MULT, .variant = CONSTANT }
  return INSTRUCTION;
}

CDIV {
  yylval.instruction = { .type = DIV, .variant = CONSTANT }
  return INSTRUCTION;
}

< {
  yylval.comparison_type = LE;
  return CMP;
}

<= {
  yylval.comparison_type = LEQ;
  return CMP;
}

= {
  yylval.comparison_type = EQ;
  return CMP;
}

>= {
  yylval.comparison_type = GEQ;
  return CMP;
}

> {
  yylval.comparison_type = GR;
  return CMP;
}

END {
  return END;
}

GOTO {
  return GOTO;
}

IFc(0) {
  return IF;
}

THEN {
  return THEN;
}

\n|; {
  return DELIMITER;
}

%%
