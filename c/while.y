%{
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;

void yyerror(const char *s);
%}

%union {
  uint8_t index;
  uint8_t value;
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

%%
prog:
    IDENTIFIER ASSIGNMENT rhs_assignment
    | IDENTIFIER ASSIGNMENT rhs_assignment DELIMITER prog
    | WHILE IDENTIFIER NOT_EQUAL_ZERO DO prog END DELIMITER prog
    | WHILE IDENTIFIER NOT_EQUAL_ZERO DO prog END
    ;

rhs_assignment:
              IDENTIFIER
              | IDENTIFIER constant_term

constant_term:
             PLUS potentially_signed
             | MINUS CONSTANT
                   ;
potentially_signed:
                  MINUS CONSTANT
                  | CONSTANT
                  ;
%%
int main(int argc, char** argv)
{
  FILE *myfile = fopen(argv[1], "r");

  if (!myfile) {
    printf("couldn't open file");
    return -1;
  }


  yyin = myfile;

  do {
    yyparse();
  } while (!feof(yyin));
}


void yyerror(const char *s)
{
  printf("An error occurred: %s", s);
}
