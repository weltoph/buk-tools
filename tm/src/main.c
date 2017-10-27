#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "grammar.tab.h"

extern FILE *yyin;

int main(int argc, char *argv[])
{
  /* consume executable name */
  argc--; argv++;
  FILE *myfile = fopen(*argv, "r");

  if (!myfile) {
    fprintf(stderr, "Couldn't open file %s", *argv);
    return -1;
  }

  /* consume file name */
  argc--; argv++;

  yyin = myfile;

  int result = yyparse();
  if(result != 0) {
    fprintf(stderr, "Parsing failed\n");
    return -2;
  }

  return 0;
}

