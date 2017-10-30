%{
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "prog.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int yylineno;

extern Prog *parse_result;

void yyerror(const char *s);

%}


%code requires {
#include "grammar.tab.h"
}

%code {
Prog *parse_result = new_prog();
}
%union {
  Command *cmd;
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

%type prog
%type <cmd> cmd

%%
prog: cmd {
      const bool success = append_command(parse_result, $1);
      if(!success) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", lineno);
        YYABORT;
      }
    }
    | prog DELIMITER cmd {
      const bool success = append_command(parse_result, $3);
      if(!success) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", lineno);
        YYABORT;
      }
    }
    ;
cmd: INSTRUCTION VALUE {
      Command *value = new_instruction($1, $2);
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", lineno);
        YYABORT;
      }
      $$ = value;
   }
   | GOTO LABEL {
      Command *value = new_jmp($2);
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", lineno);
        YYABORT;
      }
      $$ = value;
    }
    | IF CMP VALUE THEN GOTO LABEL {
      Command *value = new_cond($2, $3, $6);
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", lineno);
        YYABORT;
      }
      $$ = value;
    }
    | END {
      Command *value = new_end();
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", lineno);
        YYABORT;
      }
      $$ = value;
    }
    | LABEL {
      Command *value = new_label($1);
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", lineno);
        YYABORT;
      }
      $$ = value;
    }
    ;

%%

void yyerror(const char *s)
{
  printf("\nAn error occurred: %s\n", s);
}

