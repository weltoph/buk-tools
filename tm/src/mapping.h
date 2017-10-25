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

char movement_character(Movement_Direction dir);

Action_Status check_entry(Mapping map,
    char tapecharacter, uint8_t index);

bool set_action(Mapping *map,
    char tapecharacter, uint8_t index,
    char new_character, uint8_t new_state_index,
      Movement_Direction movement_direction);

bool add_assignment(Mapping *map, Assignment assign);

Action get_action(Mapping map,
    char tapecharacter, uint8_t index);
