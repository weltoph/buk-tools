#pragma once

#include <string.h>
#include <stdlib.h>

struct Map_Node;

typedef Map_Node Node;

typedef struct {
  Node *root;
} Map;

struct Map_Node {
  char *key;
  Node *left;
  Node *right;
  void *content;
};

bool insert(Map *map, char *key, void *content);
void *get(Map *map, char *key);
bool contains_key(Map *map, char *key);
void free_map(Map *map);
char *key_set_rep(Map *map);
