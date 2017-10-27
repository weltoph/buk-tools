#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "map.h"

typedef enum {
  LEFT,
  NEUTRAL,
  RIGHT
} MOVEMENT_DIRECTION;

/* (q, a) -> (p, b, X) */
typedef struct {
  char *p;
  char *b;
  MOVEMENT_DIRECTION X;
} Action;

typedef struct {
  char *q;
  char *a;
  Action rhs;
} Transition;

typedef struct {
  Map delta;
} Transition_Table;

Transition_Table new_trans_table();

/* creates new Action but copies p and b */
Action new_action(char *p, char *b, MOVEMENT_DIRECTION X);

/* creates new Transition but copies q and a */
Transition new_trans(char *q, char *a, Action rhs);

/* adds trans to table (copies necessary information, thus
 * trans can be discarded afterwards
 */
void add_trans(Transition_Table *table, Transition trans);
Action get_action(Transition_Table *table, char *q, char *a);

void free_table(Transition_Table *table);
