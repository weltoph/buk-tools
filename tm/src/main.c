#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "grammar.tab.h"
#include "tape.h"
#include "set.h"
#include "turingmachine.h"
#include "transition.h"

extern FILE *yyin;
extern Turingmachine tm;

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

  while(argc != 0) {
    char *input = value_in_set(tm.ioalphabet, *argv);
    if(!input) {
      fprintf(stderr, "RUNTIME ERROR: input %s does not fit in I/O alphabet\n",
          *argv);
      return -3;
    }
    set_content(tm.tape, input);
    right(tm.tape);
    argc--; argv++;
  }
  reset_head(tm.tape);
  run_tm();
  print_result();

  return 0;
}

