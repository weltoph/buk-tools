#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "limits.h"
#include "map.h"

typedef struct String_Set {
  Map *set;
};

void add_value(String_Set *set, char *value);
bool value_in_set(String_Set *set, char *value);
char *set_repr(String_Set *set); 
void free_set(String_Set *set);
