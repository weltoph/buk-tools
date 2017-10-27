#pragma once

struct Cell;

typedef struct Cell Tapecell;

struct Cell {
  Tapecell *left;
  Tapecell *right;
  char *content;
};

typedef struct {
  Tapecell *current;
  Tapecell *origin;
  char *blank_symbol;
} Tape;

/* moves left */
void left(Tape *tape);

/* moves right */
void right(Tape *tape);

/* get content */
char *get_content(Tape *tape);

/* set content */
void set_content(Tape *tape, char *content);

/* frees complete tape (but not its content!) */
void free_tape(Tape *tape);

/* sets head position back to origin */
void reset_head(Tape *tape);
