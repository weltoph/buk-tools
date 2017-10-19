#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "statement.h"
#include "prog.h"
#include "while.tab.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern Statement *last_parsed_statement;

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
  execute_from_top(&prog);
  free_statements(&prog);

  /* output */
  printf("%u\n", prog.variables[0]);
}


