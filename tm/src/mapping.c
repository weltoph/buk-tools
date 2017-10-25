#include <stdbool.h>
#include <stdint.h>
#include "mapping.h"
#include "limits.h"
#include "set.h"

char movement_character(Movement_Direction dir)
{
  switch(dir) {
    case LEFT:    return 'L';
    case NEUTRAL: return 'N';
    case RIGHT:   return 'R';
    default:      return '~';
  }
}

static bool is_valid_lookup(char tapecharacter, uint8_t index)
{
  return is_valid_tapealphabet_character(tapecharacter) &&
    is_valid_state_index(index);
}

Action_Status check_entry(Mapping map,
    char tapecharacter, uint8_t index)
{
  if(!is_valid_lookup(tapecharacter, index)) {
    return UNDEFINED;
  }
  return map.entries[tapechar_to_index(tapecharacter)][index];
}

bool set_action(Mapping *map,
    char tapecharacter, uint8_t index,
    char new_character, uint8_t new_state_index,
      Movement_Direction movement_direction)
{
  if(!is_valid_lookup(tapecharacter, index) ||
      !is_valid_lookup(new_character, new_state_index)) {
    return false;
  }
  const Action new_action = {
    .character = new_character,
    .next_state_index = new_state_index,
    .direction = movement_direction
  };
  map->actions[tapechar_to_index(tapecharacter)][index] = new_action;
  return true;
}

bool add_assignment(Mapping *map, Assignment assign)
{
  return set_action(map, assign.character, assign.state,
      assign.result.character, assign.result.next_state_index,
        assign.result.direction);
}

Action get_action(Mapping map,
    char tapecharacter, uint8_t index)
{
  /* we default for VOIDEDDEF/UNDEFINED entries to a no-op */
  const Action no_op = {
    .character = tapecharacter,
    .next_state_index = index,
    .direction = NEUTRAL
  };
  if(!is_valid_lookup(tapecharacter, index)) {
    return no_op;
  }
  const Action_Status status = check_entry(map, tapecharacter, index);
  if(status == UNDEFINED || status== VOIDEDDEF) {
    return no_op;
  }

  return map.actions[tapechar_to_index(tapecharacter)][index];
}
