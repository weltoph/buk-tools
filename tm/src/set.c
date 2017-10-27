#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "limits.h"
#include "set.h"
#include "map.h"

static bool check_init_set(String_Set *set)
{
  if(!set->set) {
    if(!set->set) {
      fprintf(stderr, "ERROR: operating on uninitialised Set\n");
      return false;
    }
  }
  return true;
}

void add_value(String_Set *set, char *value)
{
  if(!check_init_set(set)) { return; }
  if(value_in_set(set, value)) { return; }
  else {
    const bool success = insert(set->set, value, NULL);
    if(!success) {
      fprintf(stderr, "ERROR: could not insert value %s in Set\n", value);
    }
  }
}

char *value_in_set(String_Set *set, char *value)
{
  if(!check_init_set(set)) { return NULL; }
  return contains_key(set->set, value);
}

char *set_repr(String_Set *set)
{
  if(!check_init_set(set)) { return NULL; }
  return key_set_rep(set->set);
}

void free_set(String_Set *set)
{
  free_map(set->set);
  free(set->set);
}

String_Set new_set()
{
  Map *intern = malloc(sizeof(*intern));
  if(!intern) {
    fprintf(stderr, "ERROR: memory error on allocating internals of Set\n");
  } else {
    *intern = new_map();
  }
  return (String_Set){ .set = intern };
}

