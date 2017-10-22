#pragma once
#include <stdbool.h>
#include <stdint.h>

struct _Tapecell;

typedef struct _Tapecell Tapecell;

struct _Tapecell {
  int16_t index;
  char content;
  Tapecell *next;
  Tapecell *previous;
};

typedef struct {
  Tapecell *origin;
  Tapecell *current;
  char blank_symbol;
  uint8_t size_left;
  uint8_t size_right;
} Tape;

/* gives character of current tapecell */
char read_cell(Tape tape);
/*
 * moves head to the left if tapesize allows it
 * (success is indicated by return value)
 */
bool move_left(Tape *tape);
/*
 * moves head to the right if tapesize allows it
 * (success is indicated by return value)
 */
bool move_right(Tape *tape);
/*
 * resets tape to original state
 */
void reset(Tape *tape);
/* returns null-terminated string */
char *read_result(Tape tape);
