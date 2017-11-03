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
#include "prog.h"
}

%code {
Prog *parse_result = NULL;
}
%union {
  Command *cmd;
  Instruction instruction;
  Cmp_Type comparison_type;
  char name[30];
  uint8_t value;
}

%token <instruction> INSTRUCTION_TOK
%token <comparison_type> CMP_TOK
%token END_TOK
%token GOTO_TOK
%token IF_TOK
%token ACC_TOK
%token THEN_TOK
%token <value> VALUE_TOK
%token <name> LABEL_TOK
%token DELIMITER_TOK

%type <cmd> prog
%type <cmd> cmd

%%
prog: cmd {
      if(!parse_result) { parse_result = new_prog(); }
      const bool success = append_command(parse_result, $1);
      if(!success) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", yylineno);
        YYABORT;
      }
      char *repr = cmd_to_string($1);
      free(repr);
      $$ = $1;
    }
    | prog DELIMITER_TOK cmd {
      if(!parse_result) { parse_result = new_prog(); }
      const bool success = append_command(parse_result, $3);
      if(!success) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", yylineno);
        YYABORT;
      }
      char *repr = cmd_to_string($3);
      free(repr);
      $$ = $1;
    }
    | prog DELIMITER_TOK {
      $$ = $1;
    }
    ;
cmd: INSTRUCTION_TOK VALUE_TOK {
      Command *value = new_instruction($1, $2);
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", yylineno);
        YYABORT;
      }
      $$ = value;
   }
   | GOTO_TOK LABEL_TOK {
      Command *value = new_jmp($2);
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", yylineno);
        YYABORT;
      }
      $$ = value;
    }
    | IF_TOK ACC_TOK CMP_TOK VALUE_TOK THEN_TOK GOTO_TOK LABEL_TOK {
      Command *value = new_cond($3, $4, $7);
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", yylineno);
        YYABORT;
      }
      $$ = value;
    }
    | END_TOK {
      Command *value = new_end();
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", yylineno);
        YYABORT;
      }
      $$ = value;
    }
    | LABEL_TOK {
      Command *value = new_label($1);
      if(!value) {
        fprintf(stderr, "PARSE-ERROR: could not parse line #%i\n", yylineno);
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

