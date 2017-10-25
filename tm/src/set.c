#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "limits.h"
#include "set.h"


void add_states(State_Set *to, State_Set from)
{
  for(uint8_t i = 0; i <= MAX_STATE_INDEX; i++) {
    (to->states)[i] = (to->states)[i] || from.states[i];
  }
}

bool add_state(State_Set *to, uint8_t index)
{
  if(!is_valid_state_index(index)) return false;
  to->states[index] = true;
  return true;
}

bool state_in_set(uint8_t index, State_Set set)
{
  if(!is_valid_state_index(index)) return false;
  return set.states[index];
}

uint8_t state_set_size(State_Set set)
{
  uint8_t result = 0;
  for(uint8_t i = 0; i <= MAX_STATE_INDEX; i++) {
    if(state_in_set(i, set)) {
      result += 1;
    }
  }
  return result;
}

char *state_set_representation(State_Set set)
{
  /*
   * 3 characters per element (qXX) + separation symbols
   * (one less than elements) + set brackets {} + NULL-termination
   */
  uint8_t state_size = state_set_size(set);
  uint8_t state_representation_size = 3 * state_size
    + (state_size > 0 ? (state_size-1) : 0)
    + 2
    + 1;
  char *representation = malloc(sizeof(char)*state_representation_size);
  representation[state_representation_size-1] = 0;
  representation[0] = '{';
  char *current_position = representation + 1;
  char state_rep[5];
  for(uint8_t state_index = 0; state_index <= MAX_STATE_INDEX; state_index++) {
    if(state_in_set(state_index, set)) {
      sprintf(state_rep, "q%02u,", state_index);
      memcpy(current_position, state_rep, strlen(state_rep));
      current_position += strlen(state_rep);
    }
  }
  representation[state_representation_size-2] = '}';
  return representation;
}

void add_tapechars(Tapechar_Set *to, Tapechar_Set from)
{
  for(uint8_t i = 0; i < MAX_TAPEALPHABET_SIZE; i++) {
    (to->characters)[i] = (to->characters)[i] || from.characters[i];
  }
}

bool add_tapechar(Tapechar_Set *to, char character)
{
  if(!is_valid_tapealphabet_character(character)) return false;
  to->characters[tapechar_to_index(character)] = true;
  return true;
}

bool tapechar_in_set(char character, Tapechar_Set set)
{
  if(!is_valid_tapealphabet_character(character)) return false;
  return set.characters[tapechar_to_index(character)];
}

uint8_t tapechar_set_size(Tapechar_Set set)
{
  uint8_t result = 0;
  for(char *current = valid_characters; current - valid_characters < MAX_TAPEALPHABET_SIZE; current++) {
      if(tapechar_in_set(*current, set)) {
        result += 1;
      }
  }
  return result;
}
