#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "limits.h"

typedef enum {
  LEFT,
  NEUTRAL,
  RIGHT
} Movement_Direction;

typedef struct {
  char character;
  uint8_t next_state_index;
  Movement_Direction direction;
} Action;

/*
 * Distinguish three states for Mappings:
 *  there is a definition given: DEFINED
 *  there is no definition provided: UNDEFINED
 *  the given definition is voided on purpose: VOIDEDDEF
 */
typedef enum {
  DEFINED,
  UNDEFINED,
  VOIDEDDEF
} Action_Status;

typedef struct {
  uint8_t state;
  char character;
  Action result;
} Assignment;

/*
 * Keep record of status of an action and of added actions
 */
typedef struct {
  Action_Status entries[MAX_TAPEALPHABET_SIZE][MAX_STATE_INDEX + 1];
  Action        actions[MAX_TAPEALPHABET_SIZE][MAX_STATE_INDEX + 1];
} Mapping;

typedef struct {
  bool characters[MAX_TAPEALPHABET_SIZE];
} Tapechar_Set;

typedef struct {
  bool states[MAX_STATE_INDEX + 1];
} State_Set;

char movement_character(Movement_Direction dir);

void add_states(State_Set *to, State_Set from);
bool add_state(State_Set *to, uint8_t index);
bool state_in_set(uint8_t index, State_Set set);

void add_tapechars(Tapechar_Set *to, Tapechar_Set from);
bool add_tapechar(Tapechar_Set *to, char character);
bool tapechar_in_set(char character, Tapechar_Set set);

uint8_t tapechar_to_index(char character);

Action_Status check_entry(Mapping map,
    char tapecharacter, uint8_t index);

bool set_action(Mapping *map,
    char tapecharacter, uint8_t index,
    char new_character, uint8_t new_state_index,
      Movement_Direction movement_direction);

bool add_assignment(Mapping *map, Assignment assign);

Action get_action(Mapping map,
    char tapecharacter, uint8_t index);
