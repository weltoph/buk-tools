#pragma once
#include <stdint.h>
#include <stdbool.h>


/*
 * accepted states range from
 *  q0 to q30
 */
#define MAX_STATE_INDEX 30

/*
 * accepted tapealphabet is:
 *  a-zA-Z0-9 (thus 2x26 + 10)
 *    (note that strlen(valid_characters) == MAX_TAPE_ALPHABET_SIZE should
 *      always be true)
 */
#define MAX_TAPEALPHABET_SIZE 62

/* NULL-terminated string of all useable characters */
extern char *valid_characters;

bool is_valid_state_index(uint8_t index);
bool is_valid_tapealphabet_character(char character);

uint8_t tapechar_to_index(char character);

