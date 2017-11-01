#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

#include "grammar.tab.h"
#include "../../util/set.h"
#include "prog.h"

const char *argp_program_version = NULL;
const char *argp_program_bug_address = "https://github.com/weltoph/buk-tools";

typedef struct {
  bool debug;
  uint8_t arity;
  char *inputfile;
  char **inputs;
  int inputsc;
} Arguments;

struct argp_option options[] = { 
  { .name = "debug", .key = 'd', .arg = NULL, .flags = 0,
    .doc = "activates debug-mode which allows for close monitoring of execution",
    .group = 0 },
  { .name = "arity", .key = 'a', .arg = "value", .flags = 0,
    .doc = "indicates the arity of the computed function", .group = 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
  Arguments *args = (Arguments *)state->input;
  switch(key) {
    case 'd': args->debug = true;
              break;
    case 'a': args->arity = (uint8_t)atoi(arg);
              break;
    case ARGP_KEY_ARG:
              if (state->arg_num == 0) {
                args->inputfile = arg;
              } else {
                return ARGP_ERR_UNKNOWN;
              }
              break;
    case ARGP_KEY_ARGS:
              args->inputs = state->argv + state->next;
              args->inputsc = state->argc - state->next;
              break;
    default: return ARGP_ERR_UNKNOWN;
             break;
  }
  return 0;
}

struct argp argp = { options, parse_opt, "FILENAME [INPUTS]",
  "simple simulator for register machines", 0, 0, 0 };

extern FILE *yyin;
extern Prog *parse_result;

int main(int argc, char *argv[])
{
  Arguments args = { .debug = false, .arity = 1, .inputfile = NULL };

  error_t argp_res = argp_parse(&argp, argc, argv, 0, NULL, &args);
  if(argp_res != 0) {
    return argp_res;
  }
  if(!args.inputfile) {
    fprintf(stderr, "Missing inputfile\n");
    return -1;
  }

  FILE *f = fopen(args.inputfile, "r");

  if (!f) {
    fprintf(stderr, "Couldn't open file %s", *argv);
    return -2;
  }

  yyin = f;

  int result = yyparse();
  if(result != 0) {
    fprintf(stderr, "Parsing failed\n");
    return -3;
  }

  Prog *prog = parse_result;

  if(!consistency_check(prog)) {
    fprintf(stderr, "Program inconsistent\n");
    return -4;
  }
  
  for(uint8_t i = 0; i < args.inputsc; i++) {
    uint8_t value = (uint8_t)atoi(args.inputs[i]);
    set_reg(prog, i+1, value);
  }

  if(args.debug) {
    while(!is_finished(prog)) {
      cmd_fprint(stdout, prog->current);
      fprintf(stdout, "\n");
      fprintf(stdout, "options:\n\t[n] : executing next step\n\t");
      fprintf(stdout, "[r] : run program to the end\n\t");
      fprintf(stdout, "[s] n m : printing registers n to m\n\t");
      fprintf(stdout, "[a] n m : setting register n to value m\n");
      step(prog);
    }
  } else {
    exec(prog);
  }
  
  /* print result */
  print_registers(prog, 1, args.arity);
}

