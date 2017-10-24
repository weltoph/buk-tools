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
 */
#define MAX_TAPEALPHABET_SIZE 62

bool is_valid_state_index(uint8_t index);
bool is_valid_tapealphabet_character(char character);

