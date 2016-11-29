#pragma once
#include <stdint.h>
#define MAX_INDEX 100

struct Statement;

typedef struct Statement Statement;

typedef enum {
  STMT_ASSIGNMENT,
  STMT_WHILE
} Statement_Type;

typedef struct {
  uint8_t lhs_index;
  uint8_t rhs_index;
  int8_t  constant;
} Assignment;

typedef struct {
  uint8_t cond_index;
  Statement *cond_true_branch;
} While;

struct Statement {
  Statement_Type type;
  Statement     *next;
  union {
    Assignment ass;
    While      whl;
  } value;
};

typedef struct{
  uint32_t variables[MAX_INDEX];
  Statement *current_statement;
  Statement *program_begin;
} Program;

Statement *gen_assignment(uint8_t lhs_index, uint8_t rhs_index, int8_t constant, struct Statement *next);

Statement *gen_while(uint8_t cond_index, struct Statement *cond_true_branch, struct Statement *next);

void execute(Program *p);

