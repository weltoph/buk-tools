#include <stdio.h>
#include "prog.h"
#include "../../util/map.h"

Prog *new_prog()
{
  Prog *ret = malloc(sizeof(*ret));
  if(!ret) {
    fprintf(stderr, "ERROR: memory allocation for Prog failed\n");
    return NULL;
  }
  ret->label_store = new_map();  
  if(!ret->label_store) {
    return NULL;
  }
  return ret;
}

static Command *allocate_command()
{
  Command *ret = malloc(sizeof(*ret));
  if(!ret) {
    fprintf(stderr, "ERROR: memory allocation for Command failed\n");
  }
  return ret;
}

bool append_command(Prog *prog, Command *cmd)
{
  if(!prog) {
    fprintf(stderr, "ERROR: appending to NULL Prog\n");
    return false;
  }
  if(!cmd) {
    fprintf(stderr, "ERROR: appending NULL Command to Prog\n");
    return false;
  }
  if(prog->first == NULL) {
    prog->first = cmd;
    prog->last  = cmd;
    prog->current = cmd;
  } else {
    prog->last->next = cmd;
    cmd->prev = prog->last->next;
    prog->last = cmd;
    prog->last->next = NULL;
  }
  if(command->type == LABEL) {
    if(contains_key(prog->label_store, command->label)) {
      fprintf(stderr, "ERROR: inserting a label twice into Prog\n");
      return false;
    } else {
      insert(prog->label_store, label, new_cmd);
    }
  }
  return true;
}

Command *new_instruction(Instruction instruction, uint8_t argument)
{
  Command *new_cmd = allocate_command();
  if(!new_cmd) { return NULL; }
  new_cmd->type = INST;
  new_cmd->value = argument;
  return new_cmd;
}

Command *new_end()
{
  Command *new_cmd = allocate_command();
  if(!new_cmd) { return NULL; }
  new_cmd->type = END;
  return new_cmd;
}

Command *new_jmp(char *label)
{
  Command *new_cmd = allocate_command();
  if(!new_cmd) { return NULL; }
  new_cmd->type = JMP;
  strcpy(new_cmd->label, label);
  return new_cmd;
}

Command *new_cond(Cmp_Type cmp_type, uint8_t cmp_value, char *label)
{
  Command *new_cmd = allocate_command();
  if(!new_cmd) { return NULL; }
  new_cmd->type = COND;
  new_cmd->cmp_type = cmp_type;
  new_cmd->value = cmp_value;
  strcpy(new_cmd->label, label);
  return new_cmd;
}

Command *new_label(char *label)
{
  Command *new_cmd = allocate_command();
  if(!new_cmd) { return NULL; }
  new_cmd->type = LABEL;
  strcpy(new_cmd->label, label);
  return new_cmd;
}
