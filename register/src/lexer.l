/* Scanner for a Turingmachine specification language */
%{
#include <stdlib.h> /* atoi for identifier index */
#include <stdint.h>
#include <stdio.h>
#include "grammar.tab.h"
%}

%option noinput
%option nounput
%option yylineno

VALUE [0-9]|[1-9][0-9]+
LABELCHAR [0-9a-z]

%%

"#".*
[ \t\n]

{VALUE} {
  yylval.value = (uint8_t)(atoi(yytext));
  return VALUE_TOK;
}

${LABELCHAR}{1,29} {
  strcpy(yylval.name, yytext + 1);
  return LABEL_TOK;
}

"LOAD" {
  yylval.instruction = (Instruction){ .type = LOAD, .variant = STANDARD };
  return INSTRUCTION_TOK;
}

"STORE" {
  yylval.instruction = (Instruction){ .type = STORE, .variant = STANDARD };
  return INSTRUCTION_TOK;
}

"ADD" {
  yylval.instruction = (Instruction){ .type = ADD, .variant = STANDARD };
  return INSTRUCTION_TOK;
}

"SUB" {
  yylval.instruction = (Instruction){ .type = SUB, .variant = STANDARD };
  return INSTRUCTION_TOK;
}

"MULT" {
  yylval.instruction = (Instruction){ .type = MULT, .variant = STANDARD };
  return INSTRUCTION_TOK;
}


"DIV" {
  yylval.instruction = (Instruction){ .type = DIV, .variant = STANDARD };
  return INSTRUCTION_TOK;
}

"INDLOAD" {
  yylval.instruction = (Instruction){ .type = LOAD, .variant = INDIRECT };
  return INSTRUCTION_TOK;
}

"INDSTORE" {
  yylval.instruction = (Instruction){ .type = STORE, .variant = INDIRECT };
  return INSTRUCTION_TOK;
}

"INDADD" {
  yylval.instruction = (Instruction){ .type = ADD, .variant = INDIRECT };
  return INSTRUCTION_TOK;
}

"INDSUB" {
  yylval.instruction = (Instruction){ .type = SUB, .variant = INDIRECT };
  return INSTRUCTION_TOK;
}

"INDMULT" {
  yylval.instruction = (Instruction){ .type = MULT, .variant = INDIRECT };
  return INSTRUCTION_TOK;
}

"INDDIV" {
  yylval.instruction = (Instruction){ .type = DIV, .variant = INDIRECT };
  return INSTRUCTION_TOK;
}

"CLOAD" {
  yylval.instruction = (Instruction){ .type = LOAD, .variant = CONSTANT };
  return INSTRUCTION_TOK;
}

"CADD" {
  yylval.instruction = (Instruction){ .type = ADD, .variant = CONSTANT };
  return INSTRUCTION_TOK;
}

"CSUB" {
  yylval.instruction = (Instruction){ .type = SUB, .variant = CONSTANT };
  return INSTRUCTION_TOK;
}

"CMULT" {
  yylval.instruction = (Instruction){ .type = MULT, .variant = CONSTANT };
  return INSTRUCTION_TOK;
}

"CDIV" {
  yylval.instruction = (Instruction){ .type = DIV, .variant = CONSTANT };
  return INSTRUCTION_TOK;
}

"<=" {
  yylval.comparison_type = LEQ;
  return CMP_TOK;
}

"<" {
  yylval.comparison_type = LE;
  return CMP_TOK;
}

"=" {
  yylval.comparison_type = EQ;
  return CMP_TOK;
}

">=" {
  yylval.comparison_type = GEQ;
  return CMP_TOK;
}

">" {
  yylval.comparison_type = GR;
  return CMP_TOK;
}

"END" {
  return END_TOK;
}

"GOTO" {
  return GOTO_TOK;
}

"IF" {
  return IF_TOK;
}

"c(0)" {
  return ACC_TOK;
}

"THEN" {
  return THEN_TOK;
}

";" {
  return DELIMITER_TOK;
}

%%
