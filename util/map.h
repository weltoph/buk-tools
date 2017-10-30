#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

struct Map_Node;

typedef struct Map_Node Node;

typedef struct {
  Node *root;
} Map;

struct Map_Node {
  char *key;
  Node *left;
  Node *right;
  void *content;
};

/*
 * initialises a new map
 */
Map *new_map();

/*
 * insert content in map with key
 *
 * the content of key is hereby reproduced within the map
 * but content is simply stored as is (be aware of not freeing their content
 * prematurely)
 */
bool insert(Map *map, char *key, void *content);

/*
 * returns the content key for the key (or NULL if key is not present which
 * will also issue an error message
 */
void *get(Map *map, char *key);

/*
 * checks if key is present within the map and returns ptr to key
 */
char *contains_key(Map *map, char *key);

/*
 * frees the complete map structure but not the contents
 */
void free_map(Map *map);

/*
 * creates a string representation of the key set which is provided to the
 * caller (and has to be free'd by him)
 */
char *key_set_rep(Map *map);

/*
 * executes given function on all content values
 */
void visit_content(Map *map, void *init_acc,
    void (*function_ptr)(void *content, char *key, void *accumulator));
