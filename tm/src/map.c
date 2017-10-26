#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "map.h"

static Node *new_node(char *key, Node *left, Node *right, void *content)
{
  Node *new = malloc(sizeof(*new));
  if(!new) { 
    fprintf(stderr, "ERROR: could not allocate memory for new Node structure\n");
    return NULL;
  }
  new->key = key;
  new->left = left;
  new->right = right;
  new->content = content;
}

static bool insert_into_node(Node *current, char *key, void *content)
{
  int cmp_value = strcmp(key, current->key);
  if(cmp_value == 0) {
    /* found correct node */
    current->content = content;
    return true;
  } else if(cmp_value < 0) {
    if(current->left == NULL) {
      /* insert left */
      current->left = new_node(key, NULL, NULL, content);
      return current->left != NULL;
    } else {
      return insert_into_node(current->left, key, content);
    }
  } else {
    if(current->right == NULL) {
      /* insert right */
      current->right = new_node(key, NULL, NULL, content);
      return current->right != NULL;
    } else {
      return insert_into_node(current->right, key, content);
    }
  }
}

bool insert(Map *map, char *key, void *content)
{
  fprintf(stderr, "LOG: inserting into key %s\n in Map", key);
  if(!key) {
    fprintf(stderr, "ERROR: refuse to insert NULL key into Map\n");
    return false;
  }
  if(strlen(key) == 0) {
    fprintf(stderr, "ERROR: refuse to insert empty key into Map\n");
    return false;
  }
  if(map->root == NULL) {
    Node *new_root = create_node(key, NULL, NULL, content);
    if(!new_root) {
      fprintf(stderr, "ERROR: could not insert key %s into Map (memory error)\n", key);
      return false;
    }
    map->root = new_root;
    return true;
  } else {
    return insert_into_node(map->root);
  }
}

void *get(Map *map, char *key)
{
  fprintf(stderr, "LOG: requesting key %s\n from Map", key);
  if(!contains_key(map, key)) {
    fprintf(stderr, "ERROR: key %s not present in map, returning invalid value\n", key);
    return NULL;
  }
  Node *current = map->root;
  for(int cmp_value = strcmp(key, current->key); cmp_value != 0;
      cmp_value = strcmp(key, current->key)) {
    if(cmp_value < 0) {
      /* descend left */
      current = current->left;
    } else {
      current = current->right;
    }
  }
  return current->content;
}

static bool contains_key_node(Node *node, char *key)
{
  if(!node) {
    fprintf(stderr, "LOG: Map does not have key %s", key);
    return false;
  } else {
    int cmp_value = strcmp(key, node->key);
    if(cmp_value == 0) {
      fprintf(stderr, "LOG: Map does have key %s", key);
      return true;
    } else if(cmp_value < 0) {
      return contains_key_node(node->left, key);
    } else {
      return contains_key_node(node->right, key);
    }
  }
  /* unreachable code */
  return false;
}

bool contains_key(Map *map, char *key)
{
  fprintf(stderr, "LOG: checking presence of key %s in Map", key);
  if(!map->root) {
    fprintf(stderr, "LOG: empty Map does not contain key %s", key);
    return false;
  }
  return contains_key_node(map->root);
}

static void free_node(Node *node)
{
  if(!node) { return; }
  free_node(node->left);
  free_node(node->right);
  free(node->content);
  free(node->key);
  free(node);
}

void free(Map *map)
{
  if(!map->root) { return; }
  free_node(map->root);
}

char *node_keys(Node *node)
{
  char *left_keys = node->left ? node_keys(node->left) : NULL;
  char *right_keys = node->right ? node_keys(node->right) : NULL;
  char *result;
  if(left_keys && right_keys) {
    /* left and right have keys */
    result = malloc(strlen(left_keys) + 1 + strlen(node->key) + 1 + strlen(right_keys) + 1);
    if(!result) {
      fprintf(stderr, "ERROR: could not allocate enough memory to represent keys");
    } else {
      sprintf(result, "%s,%s,%s", left_keys, node->key, right_keys);
    }
  } else if (left_keys) {
    /* only left has keys */
    result = malloc(strlen(left_keys) + 1 + strlen(node->key) + 1);
    if(!result) {
      fprintf(stderr, "ERROR: could not allocate enough memory to represent keys");
    } else {
      sprintf(result, "%s,%s", left_keys, node->key);
    }
  } else if (right_keys) {
    /* only right has keys */
    result = malloc(strlen(node->key) + 1 + strlen(right_keys) + 1);
    if(!result) {
      fprintf(stderr, "ERROR: could not allocate enough memory to represent keys");
    } else {
      sprintf(result, "%s,%s", node->key, right_keys);
    }
  } else {
    /* no keys left and right */
    result = malloc(strlen(node->key) + 1);
    if(!result) {
      fprintf(stderr, "ERROR: could not allocate enough memory to represent keys");
    } else {
      sprintf(result, "%s", node->key);
    }
  }
  free(left_keys);
  free(right_keys);
  return result;
}

char *key_set_rep(Map *map)
{
  char *result;
  const char empty_set[] = "{ }";
  if(!map->root) {
    result = malloc(strlen(empty_set) + 1);
    if(!result) {
      fprintf(stderr, "ERROR: could not allocate memory for key set representation");
    }
    strcpy(result, empty_set);
  } else {
    char *node_keys = node_keys(map->root);
    result = malloc(strlen(node_keys) + 2 + 1);
    if(!result) {
      fprintf(stderr, "ERROR: could not allocate memory for key set representation");
    }
    sprintf(result, "{%s}", node_keys);
    free(node_keys);
  }
  return result;
}
