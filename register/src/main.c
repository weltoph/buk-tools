#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>

#include "grammar.tab.h"
#include "../../util/set.h"
#include "prog.h"

const char *argp_program_version = "unspecified";
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
    .doc = "Enters debug-mode", .group = 0 },
  { .name = "arity", .key = 'a', .arg = "value", .flags = 0,
    .doc = "Defines arity of computed function", .group = 0 },
  { 0 }
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
  "simulator for register machines", 0, 0, 0 };

extern FILE *yyin;
extern Prog *parse_result;

static char *get_token(char **input)
{
  char *token = strsep(input, " ");
  if(!token) {
    fprintf(stderr, "DEBUG-ERROR: could not extract expected token\n");
  }
  return token;
}

static bool debug_handle_input(Prog *prog, char *input)
{
  char *input_cpy = malloc(strlen(input) + 1);
  if(!input_cpy) {
    fprintf(stderr, "DEBUG-ERROR: could not allocate memory for tokenizing input %s\n",
        input);
    return false;
  }
  strcpy(input_cpy, input);
  char *current = input_cpy;
  char *token = NULL;
  token = get_token(&current);
  if(strcmp(token, "n") == 0) {
    return step(prog);
  } else if(strcmp(token, "r") == 0) {
    return exec(prog);
  } else if(strcmp(token, "a") == 0) {
    uint8_t index = (uint8_t)atoi(get_token(&current));
    uint8_t value = (uint8_t)atoi(get_token(&current));
    set_reg(prog, index, value);
    return true;
  } else if(strcmp(token, "s") == 0) {
    for(token = get_token(&current); true; token = get_token(&current)) {
      uint8_t current_reg = (uint8_t)atoi(token);
      print_registers(prog, current_reg, current_reg);
      if(current == NULL) { break; }
    }
    return true;
  } else if(strcmp(token, "p") == 0) {
    print_prog(prog);
    return true;
  } else if(strcmp(token, "u") == 0) {
    prog->current = prog->current->prev ? prog->current->prev : prog->current;
    return true;
  } else if(strcmp(token, "d") == 0) {
    prog->current = prog->current->next ? prog->current->next : prog->current;
    return true;
  } else {
    fprintf(stdout, "unknown debug option %s\n", token);
    return true;
  }
  free(input_cpy);
}

static void debug_prog(Prog *prog)
{
  while(!is_finished(prog)) {
    cmd_fprint(stdout, prog->current);
    fprintf(stdout, "\n");

    fprintf(stdout, "options:\n");
    fprintf(stdout, "\t[n] : executing next step (default)\n");
    fprintf(stdout, "\t[r] : run program to the end\n");
    fprintf(stdout, "\t[p] : prints program\n");
    fprintf(stdout, "\t[d] : increases program counter without executing code\n");
    fprintf(stdout, "\t[u] : decreases program counter without executing code\n");
    fprintf(stdout, "\t[a] n m : setting register n to value m\n");
    fprintf(stdout, "\t[s] n1 ... nk : printing registers n1 to nk\n");

    char *input = NULL;
    size_t buf_size = 0;
    ssize_t input_len = 0;
    input_len = getline(&input, &buf_size, stdin);
    if(input_len < 0 || buf_size == 0) {
      fprintf(stderr, "DEBUG-ERROR: could not get input\n");
      return;
    }
    if(input_len == 1) {
      step(prog);
    } else {
      /* strip trailing new-line */
      input[strlen(input) - 1] = '\0';
      bool success = debug_handle_input(prog, input);
      if(!success) {
        fprintf(stderr, "DEBUG-ERROR: abort debug due to an error\n");
        break;
      }
    }
    free(input);
  }
  fprintf(stdout, "Program terminated...\n");
}

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
    debug_prog(prog);
  } else {
    exec(prog);
  }
  
  /* print result */
  print_registers(prog, 1, args.arity);
}

