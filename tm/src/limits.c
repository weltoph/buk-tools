#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "limits.h"

char *valid_characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

bool is_valid_state_index(uint8_t index)
{
  return index <= MAX_STATE_INDEX;
}

bool is_valid_tapealphabet_character(char character)
{
  return strchr(valid_characters, character) != NULL;
}

uint8_t tapechar_to_index(char character)
{
  const char *position = strchr(valid_characters, character);
  if(!position) { return MAX_TAPEALPHABET_SIZE; }
  uint8_t index = position - valid_characters;
  return index;
}
