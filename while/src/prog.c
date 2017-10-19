#include "prog.h"
#include "statement.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

void next(Program *p)
{
  p->current_statement = p->current_statement->next;
}

bool terminated(Program *p)
{
  return p->current_statement == NULL;
}

void step(Program *p)
{
  Statement *curr_stmt = p->current_statement;

  if(curr_stmt->type == STMT_ASSIGNMENT)
  {
    uint8_t lhs = p->current_statement->value.ass.lhs_index;
    uint8_t rhs = p->current_statement->value.ass.rhs_index;
    int8_t con = p->current_statement->value.ass.constant;

    p->variables[lhs] = (p->variables[rhs] + con) < 0 ? 0 : (p->variables[rhs] + con);
    next(p);
  } else {
    uint8_t cond_index = p->current_statement->value.whl.cond_index;
    Statement* cond_branch = p->current_statement->value.whl.cond_true_branch;
    if (p->variables[cond_index] != 0)
      p->current_statement = cond_branch;
    else
      next(p);
  }
}

void free_statements(Program *p)
{
  Statement *tmp_next = NULL;
  for(reset(p); !terminated(p); p->current_statement = tmp_next) {
    tmp_next = p->current_statement->next;
    free_statement(p->current_statement);
  }
}

void reset(Program *p)
{
  p->current_statement = p->program_begin;
}

void execute(Program *p)
{
  while(!terminated(p))
  {
    step(p);
  }
}

void execute_from_top(Program *p)
{
  reset(p);
  execute(p);
}
