#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "grammar.tab.h"
#include "../../util/set.h"
#include "prog.h"

extern FILE *yyin;
extern Prog *parse_result;

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

  Prog *prog = parse_result;

  if(!consistency_check(prog)) {
    fprintf(stderr, "Program inconsistent\n");
    return -3;
  }
  
  for(uint8_t i = 0; i < argc; i++) {
    uint8_t value = (uint8_t)atoi(argv[i]);
    set_reg(prog, i+1, value);
  }

  exec(prog);
  print_registers(prog, 1, 1);
}

