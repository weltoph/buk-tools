#pragma once
#include <stdint.h>
#include "statement.h"
#define MAX_INDEX 100

typedef struct{
  uint32_t variables[MAX_INDEX + 1];
  Statement *current_statement;
  Statement *program_begin;
} Program;

void execute(Program *p);

void execute_from_top(Program *p);

void reset(Program *p);

void free_statements(Program *p);
