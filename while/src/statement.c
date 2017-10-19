#include "statement.h"
#include <stdint.h>
#include <stdlib.h>

static Statement *get_last_statement(Statement *s)
{
  Statement *curr = s;
  while(curr->next != NULL)
    curr = curr->next;
  return curr;
}

void free_statement(Statement *stmt)
{
  if(stmt == NULL) return;
  Statement *current = NULL;
  Statement *next = NULL;
  switch(stmt->type) {
    case STMT_WHILE:
      current = stmt->value.whl.cond_true_branch;
      while(current != stmt) {
        next = current->next;
        free_statement(current);
        current = next;
        next = NULL;
      }
      free(stmt);
      break;
    case STMT_ASSIGNMENT:
      free(stmt);
      break;
    default:
      break;
  }
}

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
