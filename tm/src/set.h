#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "limits.h"

typedef struct {
  bool characters[MAX_TAPEALPHABET_SIZE];
} Tapechar_Set;

typedef struct {
  bool states[MAX_STATE_INDEX + 1];
} State_Set;

void add_states(State_Set *to, State_Set from);
bool add_state(State_Set *to, uint8_t index);
bool state_in_set(uint8_t index, State_Set set);
uint8_t state_set_size(State_Set set);
char *state_set_representation(State_Set set);

void add_tapechars(Tapechar_Set *to, Tapechar_Set from);
bool add_tapechar(Tapechar_Set *to, char character);
bool tapechar_in_set(char character, Tapechar_Set set);
uint8_t tapechar_set_size(Tapechar_Set set);
