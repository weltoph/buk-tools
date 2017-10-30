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
  return (String_Set){ .set = new_map() };
}


typedef struct {
  bool value;
  String_Set *super;
} Accumulation;
/* check_subset is meant as visitor provided to visit_content and therefore has
 * to have a specific signature but since we do not need the content parameter
 * gcc issues a unused-paramter warning which we disable temporarily here
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void check_subset(void *content, char *key, void *accumulator)
{
  Accumulation *acc = (Accumulation *)accumulator;
  bool key_in_super = (value_in_set(acc->super, key) != NULL);
  acc->value &= key_in_super;
}
#pragma GCC diagnostic pop
bool is_subset(String_Set *sub, String_Set *super)
{
  Accumulation acc = (Accumulation){ .value = true, .super = super};
  visit_content(sub->set, &acc, &check_subset);
  return acc.value;
}
