#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "map.h"
#include "transition.h"

static char dir_to_char(MOVEMENT_DIRECTION X)
{
  switch(X) {
    case LEFT:
      return 'L'; break;
    case NEUTRAL:
      return 'N'; break;
    case RIGHT:
      return 'R'; break;
    default:
      return '~'; break;
  }
}

Action new_action(char *p, char *b, MOVEMENT_DIRECTION X)
{
  char *intern_p = malloc(strlen(p) + 1);
  char *intern_b = malloc(strlen(b) + 1);
  if(!intern_p || !intern_b) {
    fprintf(stderr, "ERROR: memory allocation failed for creating a new action\
        of the form (%s, %s, %c)\n", p, b, dir_to_char(X));
    return (Action){ .p = NULL, .b = NULL, .X = NEUTRAL};
  }
  strcpy(intern_p, p);
  strcpy(intern_b, b);
  return (Action){ .p = intern_p, .b = intern_b, .X = X};
}

Transition new_trans(char *q, char *a, Action rhs)
{
  char *intern_q = malloc(strlen(q) + 1);
  char *intern_a = malloc(strlen(a) + 1);
  if(!intern_q || !intern_a) {
    fprintf(stderr, "ERROR: memory allocation failed for creating new transition\
        of the form (%s, %s) -> (%s, %s, %c)\n",
        q, a, rhs.p, rhs.b, dir_to_char(rhs.X));
    return (Transition){ .q = NULL, .a = NULL, .rhs = rhs};
  }
  strcpy(intern_q, q);
  strcpy(intern_a, a);
  return (Transition) {
    .q = intern_q,
    .a = intern_a,
    .rhs = rhs
  };
}

Transition_Table new_trans_table()
{
  return (Transition_Table){ .delta = new_map() };
}

void add_trans(Transition_Table *table, Transition trans)
{
  char *q = trans.q;
  char *a = trans.a;
  Map *delta = &(table->delta);
  Action *act_ptr = malloc(sizeof(*act_ptr));
  if(!act_ptr) {
    fprintf(stderr, "ERROR: memory allocation for inserting Action in\
        Transition_Table for pair (%s, %s)\n", q, a);
    return;
  }
  *act_ptr = trans.rhs;
  Map *q_row = NULL;
  if(contains_key(delta, q)) {
    q_row = (Map*) get(delta, q);
  } else {
    q_row = malloc(sizeof(*q_row));
    *q_row = new_map();
    insert(delta, q, q_row);
  }
  if(contains_key(q_row, a)) {
    fprintf(stderr, "WARNING: pair (%s, %s) already present in Transition_Table\n",
        q, a);
  }
  bool success = insert(q_row, a, act_ptr);
  if(!success) {
    fprintf(stderr, "ERROR: failed to insert Action in Transition_Table for\
        pair (%s, %s)\n", q, a);
  }
}

Action get_action(Transition_Table *table, char *q, char *a)
{
  Action noop = (Action) {
    .p = q,
    .b = a,
    .X = NEUTRAL
  };
  if(!table) {
    fprintf(stderr, "ERROR: trying to access a NULL Transition_Table at pair\
        (%s, %s)\n", q, a);
    return noop;
  }
  Map *delta = &(table->delta);
  if(!contains_key(delta, q)) {
    fprintf(stderr, "ERROR: trying to access a not-set Action in\
        Transition_Table at pair (%s, %s)\n", q, a);
    return noop;
  }
  Map *q_row = get(delta, q);
  if(!contains_key(q_row, a)) {
    fprintf(stderr, "ERROR: trying to access a not-set Action in\
        Transition_Table at pair (%s, %s)\n", q, a);
    return noop;
  }
  return *((Action*)(get(q_row, a)));
}

void free_action(void *ptr, char *key)
{
  if(false) {
    fprintf(stderr, "%s\n", key);
  }
  Action *action = (Action*)ptr;
  free(action->p);
  free(action->b);
  free(action);
}
void free_map_proxy(void *ptr, char *key)
{
  if(false) {
    fprintf(stderr, "%s\n", key);
  }
  free_all((Map *)ptr, &free_action);
  free(ptr);
}
void free_table(Transition_Table *table)
{
  if(!table) { return; }
  Map delta = table->delta;
  free_all(&delta, &free_map_proxy);
  return;
}
