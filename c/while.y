%{
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);

%}


%code requires {
#include "prog.h"
}

%code {
Statement *last_parsed_statement;
}

%union {
  uint8_t index;
  int8_t value;
  Assignment ass;
  Statement *stmt;
}


%token <index> IDENTIFIER
%token <value> CONSTANT
%token DELIMITER
%token ASSIGNMENT
%token PLUS
%token MINUS
%token NOT_EQUAL_ZERO
%token WHILE
%token DO
%token END

%type <value> potentially_signed
%type <value> constant_term
%type <ass> rhs_assignment
%type <stmt> prog

%%
prog:
    IDENTIFIER ASSIGNMENT rhs_assignment {
        Assignment const rhs_ass = $3;
        uint8_t const lhs_index  = $1;
        uint8_t const rhs_index  = rhs_ass.rhs_index;
        int8_t const constant    = rhs_ass.constant;
        Statement *this_statement = gen_assignment(lhs_index, rhs_index,
          constant, NULL);
        if (!this_statement)
          ; /* TODO: error */
        last_parsed_statement = this_statement;
        $$ = this_statement;
      }
    | IDENTIFIER ASSIGNMENT rhs_assignment DELIMITER prog {
        Assignment const rhs_ass = $3;
        uint8_t const lhs_index  = $1;
        uint8_t const rhs_index  = rhs_ass.rhs_index;
        int8_t const constant    = rhs_ass.constant;
        Statement *next          = $5;
        Statement *this_statement = gen_assignment(lhs_index, rhs_index,
          constant, next);
        if (!this_statement)
          ; /* TODO: error */
        last_parsed_statement = this_statement;
        $$ = this_statement;
      }
    | WHILE IDENTIFIER NOT_EQUAL_ZERO DO prog END DELIMITER prog{
        Statement *cond_branch = $5;
        uint8_t const cond_ind = $2;
        Statement *next        = $8;
        Statement *this_statement = gen_while(cond_ind, cond_branch, next);
        if (!this_statement)
          ; /* TODO: error */
        last_parsed_statement = this_statement;
        $$ = this_statement;
      }
    | WHILE IDENTIFIER NOT_EQUAL_ZERO DO prog END {
        Statement *cond_branch = $5;
        uint8_t const cond_ind = $2;
        Statement *this_statement = gen_while(cond_ind, cond_branch, NULL);
        if (!this_statement)
          ; /* TODO: error */
        last_parsed_statement = this_statement;
        $$ = this_statement;
      }
    ;

rhs_assignment:
              IDENTIFIER {
                  const Assignment ass = {.rhs_index = $1, .constant = 0};
                  $$ = ass;
                }
              | IDENTIFIER constant_term {
                  const Assignment ass = {.rhs_index = $1, .constant = $2};
                  $$ = ass;
                }

constant_term:
             PLUS potentially_signed { $$ = $2; }
             | MINUS CONSTANT { $$ = -$2; }
                   ;
potentially_signed:
                  MINUS CONSTANT { $$ = -$2; }
                  | CONSTANT  { $$ = $1; }
                  ;
%%
int main(int argc, char** argv)
{
  /* consume executable name */
  argc--; argv++;
  FILE *myfile = fopen(*argv, "r");

  if (!myfile) {
    printf("Couldn't open file %s", *argv);
    return -1;
  }

  /* consume file name */
  argc--; argv++;

  yyin = myfile;

  do {
    yyparse();
  } while (!feof(yyin));


  /* generating program */
  Program prog = {.current_statement = last_parsed_statement,
    .program_begin = last_parsed_statement};

  /* reading input variables */ 
  for(uint8_t i = 0; i < argc; i++)
  {
    prog.variables[i+1] = atoi(argv[i]);
  }
  /* execute program */
  execute(&prog);

  /* output */
  printf("The result of the computation is %d", prog.variables[0]);
}

void yyerror(const char *s)
{
  printf("An error occurred: %s", s);
}
