#include <stdbool.h>
#include <stdint.h>
#include "mapping.h"
#include "limits.h"

char movement_character(Movement_Direction dir)
{
  switch(dir) {
    case LEFT:    return 'L';
    case NEUTRAL: return 'N';
    case RIGHT:   return 'R';
    default:      return '~';
  }
}

/*
 * Likewise to the checks in limits we take the verbose approach here
 * rather than computing the index to avoid encoding problems
 * (this might be silly, but better safe than sorry...)
 */
uint8_t tapechar_to_index(char character)
{
  switch(character) {
    case 'a':
      return 0;
      break;
    case 'b':
      return 1;
      break;
    case 'c':
      return 2;
      break;
    case 'd':
      return 3;
      break;
    case 'e':
      return 4;
      break;
    case 'f':
      return 5;
      break;
    case 'g':
      return 6;
      break;
    case 'h':
      return 7;
      break;
    case 'i':
      return 8;
      break;
    case 'j':
      return 9;
      break;
    case 'k':
      return 10;
      break;
    case 'l':
      return 11;
      break;
    case 'm':
      return 12;
      break;
    case 'n':
      return 13;
      break;
    case 'o':
      return 14;
      break;
    case 'p':
      return 15;
      break;
    case 'q':
      return 16;
      break;
    case 'r':
      return 17;
      break;
    case 's':
      return 18;
      break;
    case 't':
      return 19;
      break;
    case 'u':
      return 20;
      break;
    case 'v':
      return 21;
      break;
    case 'w':
      return 22;
      break;
    case 'x':
      return 23;
      break;
    case 'y':
      return 24;
      break;
    case 'z':
      return 25;
      break;
    case 'A':
      return 26;
      break;
    case 'B':
      return 27;
      break;
    case 'C':
      return 28;
      break;
    case 'D':
      return 29;
      break;
    case 'E':
      return 30;
      break;
    case 'F':
      return 31;
      break;
    case 'G':
      return 32;
      break;
    case 'H':
      return 33;
      break;
    case 'I':
      return 34;
      break;
    case 'J':
      return 35;
      break;
    case 'K':
      return 36;
      break;
    case 'L':
      return 37;
      break;
    case 'M':
      return 38;
      break;
    case 'N':
      return 39;
      break;
    case 'O':
      return 40;
      break;
    case 'P':
      return 41;
      break;
    case 'Q':
      return 42;
      break;
    case 'R':
      return 43;
      break;
    case 'S':
      return 44;
      break;
    case 'T':
      return 45;
      break;
    case 'U':
      return 46;
      break;
    case 'V':
      return 47;
      break;
    case 'W':
      return 48;
      break;
    case 'X':
      return 49;
      break;
    case 'Y':
      return 50;
      break;
    case 'Z':
      return 51;
      break;
    case '0':
      return 52;
      break;
    case '1':
      return 53;
      break;
    case '2':
      return 54;
      break;
    case '3':
      return 55;
      break;
    case '4':
      return 56;
      break;
    case '5':
      return 57;
      break;
    case '6':
      return 58;
      break;
    case '7':
      return 58;
      break;
    case '8':
      return 60;
      break;
    case '9':
      return 61;
      break;
    default:
      return MAX_TAPEALPHABET_SIZE;
      break;
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
