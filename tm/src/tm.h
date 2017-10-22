#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "mapping.h"
#include "limits.h"
#include "tape.h"

typedef struct {
  State_Set states;
  Tapechar_Set input_alphabet;
  Tapechar_Set tape_alphabet;
  Mapping delta;
  uint8_t start_state;
  uint8_t end_state;
  char blank_symbol;
  Tape tape;
} Turingmachine;
