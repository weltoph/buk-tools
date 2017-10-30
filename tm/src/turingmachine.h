#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "../../util/set.h"
#include "tape.h"
#include "transition.h"

typedef struct {
  String_Set *states;
  String_Set *ioalphabet;
  String_Set *tapealphabet;
  Transition_Table *delta;
  char *current_state;
  char *end_state;
  Tape *tape;
} Turingmachine;

extern Turingmachine tm;

void run_tm();
void print_result();
