#include <stdio.h>
#include <stdlib.h>
#include "tape.h"

static void free_left(Tapecell *cell)
{
  for(Tapecell *c = cell->left; c != NULL; c = c->left) { free(c); }
}

static void free_right(Tapecell *cell)
{
  for(Tapecell *c = cell->right; c != NULL; c = c->right) { free(c); }
}

/* creates new Tape */
Tape new_tape(char *blank_sym)
{
  Tapecell *origin = malloc(sizeof(*origin));
  if(!origin) {
    fprintf(stderr, "ERROR: memory allocation for a new Tape\n");
  }
  return (Tape){ .current = origin, .origin = origin, .blank_symbol = blank_sym};
}

/* moves left */
void left(Tape *tape)
{
  Tapecell *current = tape->current;
  if(!current->left) {
    current->left = malloc(sizeof(*(current->left)));
    if(!current->left) {
      fprintf(stderr, "ERROR: could not allocate tape for left movement");
    }
    current->left->right = current;
    current->left->content = tape->blank_symbol;
  }
  tape->current = current->left;
}

/* moves right */
void right(Tape *tape)
{
  Tapecell *current = tape->current;
  if(!current->right) {
    current->right = malloc(sizeof(*(current->right)));
    if(!current->right) {
      fprintf(stderr, "ERROR: could not allocate tape for right movement");
    }
    current->right->left = current;
    current->right->content = tape->blank_symbol;
  }
  tape->current = current->right;
}

/* get content */
char *get_content(Tape *tape)
{
  return tape->current->content;
}

/* set content */
void set_content(Tape *tape, char *content)
{
  tape->current->content = content;
}

/* frees complete tape (but not its content!) */
void free_tape(Tape *tape)
{
  if(!tape->current) { return; }
  free_left(tape->current);
  free_right(tape->current);
  free(tape->current);
}

/* sets head position back to origin */
void reset_head(Tape *tape)
{
  tape->current = tape->origin;
}
