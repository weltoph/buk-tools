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
  /* init all registers to 0; this might be redundant */
  for(uint8_t i = 0; i <= MAX_INDEX; i++) {
    (ret->registers)[i] = 0;
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
  if(cmd->type == LABEL) {
    if(contains_key(prog->label_store, cmd->label)) {
      fprintf(stderr, "ERROR: inserting a label twice into Prog\n");
      return false;
    } else {
      insert(prog->label_store, cmd->label, cmd);
    }
  }
  return true;
}

Command *new_instruction(Instruction instruction, uint8_t argument)
{
  Command *new_cmd = allocate_command();
  if(!new_cmd) { return NULL; }
  new_cmd->type = INST;
  new_cmd->instruction = instruction;
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

static char *instr_to_string(Instruction instruction)
{
  char prefix[4] = "";
  switch(instruction.variant) {
    case CONSTANT: strcpy(prefix, "C");
                   break;
    case INDIRECT: strcpy(prefix, "IND");
                   break;
    case STANDARD: strcpy(prefix, "");
                   break;
    default:       strcpy(prefix, "");
                   break;
  }
  char instr[6] = "";
  switch(instruction.type) {
    case LOAD: strcpy(instr, "LOAD");
               break;
    case STORE: strcpy(instr, "STORE");
                break;
    case ADD: strcpy(instr, "ADD");
              break;
    case SUB: strcpy(instr, "SUB");
              break;
    case MULT: strcpy(instr, "MULT");
               break;
    case DIV: strcpy(instr, "DIV");
              break;
  }
  char *repr = malloc(strlen(prefix) + strlen(instr) + 1);
  sprintf(repr, "%s%s", prefix, instr);
  return repr;
}

static char *cmp_to_string(Cmp_Type type)
{
  char type_rep[3] = "";
  switch(type) {
    case EQ: strcpy(type_rep, "=");
             break;
    case LEQ: strcpy(type_rep, "<=");
             break;
    case GEQ: strcpy(type_rep, ">=");
             break;
    case LE: strcpy(type_rep, "<");
             break;
    case GR: strcpy(type_rep, ">");
             break;
    default: strcpy(type_rep, "X");
             break;
  }
  char *repr = malloc(strlen(type_rep) + 1);
  strcpy(repr, type_rep);
  return repr;
}

char *cmd_to_string(Command *cmd)
{
  char *repr;
  char *instr;
  char *cmp_string;
  const char end_string[] = "END";
  const char if_string[] = "IF c(0)";
  const char goto_string[] = "GOTO ";
  const char then_goto_string[] = "THEN GOTO ";
  const char number_dummy[] = "xxx";
  switch(cmd->type) {
    case INST:
      instr = instr_to_string(cmd->instruction);
      repr = malloc(strlen(instr) + 4 + 1);
      sprintf(repr, "%s %u", instr, cmd->value);
      free(instr);
      break;
    case END:
      repr = malloc(strlen(end_string) + 1);
      strcpy(repr, end_string);
      break;
    case COND:
      cmp_string = cmp_to_string(cmd->cmp_type);
      repr = malloc(strlen(if_string) + strlen(cmp_string)
          + strlen(number_dummy) + strlen(then_goto_string)
          + strlen(cmd->label) + 1);
      sprintf(repr, "%s%s%u%s%s", if_string, cmp_string, cmd->value,
        then_goto_string, cmd->label);
      free(cmp_string);
      break;
    case JMP:
      repr = malloc(strlen(goto_string) + strlen(cmd->label) + 1);
      sprintf(repr, "%s%s", goto_string, cmd->label);
      break;
    case LABEL:
      repr = malloc(strlen(cmd->label) + 1);
      strcpy(repr, cmd->label);
      break;
    default:
      repr = malloc(2);
      repr = "";
      break;
  }
  return repr;
}


bool consistency_check(Prog *prog)
{
  if(!prog) { return true; }
  if(!prog->first) { return true; }
  for(Command *curr = prog->first; curr != NULL; curr = curr->next) {
    if(curr->type == JMP || curr->type == COND || curr->type == LABEL) {
      if(!contains_key(prog->label_store, curr->label)) {
        fprintf(stderr, "CONSISTENCY-ERROR: label %s is unknown\n", curr->label);
        return false;
      }
    }
  }
  return true;
}

void step(Prog *prog)
{
  Command *current = prog->current;
  switch(current->type) {
    case INST:  break;
    case END:   break;
    case JMP:   break;
    case COND:  break;
    case LABEL: break;
    default:    fprintf(stderr, "RUNTIME-ERROR: executing unrecognized command\n");
                break;
  }
}
