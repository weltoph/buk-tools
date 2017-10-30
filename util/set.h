#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "limits.h"
#include "map.h"

typedef struct {
  Map *set;
} String_Set;

/*
 * initialises new set
 */
String_Set new_set();

/*
 * adds a value to new set
 */
void add_value(String_Set *set, char *value);

/*
 * checks if value is present in set and returns ptr to it
 */
char *value_in_set(String_Set *set, char *value);

/*
 * returns a string representation of the set which is provided to the
 * callee making him responsible for freeing the memory
 */
char *set_repr(String_Set *set); 

/*
 * free's the set
 */
void free_set(String_Set *set);

/*
 * checks if sub is subset of super
 */
bool is_subset(String_Set *sub, String_Set *super);
