#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "limits.h"
#include "set.h"

static void check_alloc_set(String_Set *set)
{
  if(!set->set) {
    set->set = malloc(sizeof(*(set->set)));
    if(!set->set) {
      fprintf(stderr, "ERROR: could not allocate memory for Set\n");
    }
  }
}

void add_value(String_Set *set, char *value)
{
  check_alloc_set(set);
  if(value_in_set(set, value)) { return; }
  else {
    const bool success = insert(set->set, value, NULL);
    if(!success) {
      fprintf(stderr, "ERROR: could not insert value %s in Set\n", value);
    }
  }
}
bool value_in_set(String_Set *set, char *value)
{
  check_alloc_set(set);
  return contains_key(set->set, value);
}

char *set_repr(String_Set *set)
{
  check_alloc_set(set);
  return key_set_rep(set->set);
}
void free_set(String_Set *set);
