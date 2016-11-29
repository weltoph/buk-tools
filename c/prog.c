#include "prog.h"
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>

Statement *gen_assignment(uint8_t lhs_index, uint8_t rhs_index, int8_t constant, Statement *next)
{
  Statement *this_statement = malloc(sizeof(*this_statement));
  Assignment const ass = {.lhs_index = lhs_index, .rhs_index = rhs_index, .constant = constant};
  if (this_statement)
  {
    this_statement->type = STMT_ASSIGNMENT;
    this_statement->next = next;
    this_statement->value.ass = ass;
  }
  return this_statement;
}

Statement *get_last_statement(Statement *s)
{
  Statement *curr = s;
  while(curr->next != NULL)
    curr = curr->next;
  return curr;
}

Statement *gen_while(uint8_t cond_index, Statement *cond_true_branch, Statement *next)
{
  Statement *this_statement = malloc(sizeof(*this_statement));
  Statement *conditional_branch_last = get_last_statement(cond_true_branch);
  if (this_statement)
  {
    this_statement->type = STMT_WHILE;
    this_statement->next = next;
    this_statement->value.whl = (While){.cond_index = cond_index, .cond_true_branch = cond_true_branch};
    conditional_branch_last->next = this_statement;
  }
  return this_statement;
}

void next(Program *p)
{
  p->current_statement = p->current_statement->next;
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

void execute(Program *p)
{
  while(p->current_statement != NULL)
  {
    step(p);
  }
}
