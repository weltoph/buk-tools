#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../../util/map.h"
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
  Map *delta = table->delta;
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
    q_row = new_map();
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
  Map *delta = table->delta;
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void free_action(void *ptr, char *key, void *acc)
{
  Action *action = (Action*)ptr;
  free(action->p);
  free(action->b);
  free(action);
}
void free_map_proxy(void *ptr, char *key, void *acc)
{
  visit_content((Map *)ptr, NULL, &free_action);
  free_map((Map *)ptr);
  free(ptr);
}
#pragma GCC diagnostic pop
void free_table(Transition_Table *table)
{
  if(!table) { return; }
  Map *delta = table->delta;
  visit_content(delta, NULL, &free_map_proxy);
  free_map(delta);
  free(delta);
  return;
}

typedef struct {
  bool value;
  String_Set *states;
  String_Set *tapealph;
  char *end_state;
  bool outgoing_end;
  bool incoming_end;
  char *start_state;
  bool outgoing_start;
} Accumulator;

void check_tapealph_consistency(void *content, char *key, void *accumulator)
{
  Accumulator *acc = (Accumulator *)accumulator;
  bool key_in_tapealph = (value_in_set(acc->tapealph, key) != NULL);
  Action *action = (Action *)content;
  bool action_state = (value_in_set(acc->states, action->p) != NULL);
  bool action_tapechar = (value_in_set(acc->tapealph, action->b) != NULL);
  acc->value &= key_in_tapealph && action_state && action_tapechar;
  acc->incoming_end |= (strcmp(acc->end_state, action->p) == 0);
}
void check_states_consistency(void *content, char *key, void *accumulator)
{
  Accumulator *acc = (Accumulator *)accumulator;
  bool key_in_states = (value_in_set(acc->states, key) != NULL);
  acc->value &= key_in_states;
  acc->outgoing_end |= (strcmp(acc->end_state, key) == 0);
  acc->outgoing_start |= (strcmp(acc->start_state, key) == 0);
  if(acc->value) {
    visit_content((Map *)content, acc, &check_tapealph_consistency);
  }
}
bool consistent_table(Transition_Table *table, String_Set *states,
    String_Set *tapealph, char *start_state, char *end_state)
{
  Accumulator acc = (Accumulator){ .value = true, .states = states,
    .tapealph = tapealph, .outgoing_end = false, .incoming_end = false,
    .end_state = end_state, .start_state = start_state, .outgoing_start = false};
  visit_content(table->delta, &acc, &check_states_consistency);
  if(!acc.value) {
    fprintf(stderr, "\t- not all states / tapecharacters listed in the corresponding sets\n");
  }
  if(acc.outgoing_end) {
    fprintf(stderr, "\t- final state does have an outgoing transition\n");
  }
  if(!acc.incoming_end) {
    fprintf(stderr, "\t- final state does not have an incoming transition\n");
  }
  if(!acc.outgoing_start) {
    fprintf(stderr, "\t- start state does not have an outgoing transition\n");
  }
  return acc.value && !acc.outgoing_end && acc.incoming_end && acc.outgoing_start;
}
