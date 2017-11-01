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
    cmd->prev = prog->last;
    prog->last = cmd;
    prog->last->next = NULL;
  }
  if(cmd->type == LABEL) {
    if(contains_key(prog->label_store, cmd->label)) {
      fprintf(stderr, "ERROR: inserting a label twice into Prog\n");
      return false;
    } else {
      const bool success = insert(prog->label_store, cmd->label, cmd);
      if(!success) {
        fprintf(stderr, "ERROR: insertion label %s into store failed!", cmd->label);
      }
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
  const char then_goto_string[] = " THEN GOTO ";
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
          + strlen(cmd->label) + 1 + 1);
      sprintf(repr, "%s%s%u%s$%s", if_string, cmp_string, cmd->value,
        then_goto_string, cmd->label);
      free(cmp_string);
      break;
    case JMP:
      repr = malloc(strlen(goto_string) + strlen(cmd->label) + 1);
      sprintf(repr, "%s%s", goto_string, cmd->label);
      break;
    case LABEL:
      repr = malloc(strlen(cmd->label) + 1 + 1);
      sprintf(repr, "$%s", cmd->label);
      break;
    default:
      repr = malloc(2);
      repr = "";
      break;
  }
  return repr;
}

void print_prog(Prog *prog)
{
  if(!prog || !(prog->first)) {
    fprintf(stdout, "Prog: (null)\n");
    return;
  }
  char *current_cmd_repr;
  Command *cmd = prog->first;
  do {
    current_cmd_repr = cmd_to_string(cmd);
    if(cmd == prog->current) {
      fprintf(stdout, "<<%s>>\n", current_cmd_repr);
    } else {
      fprintf(stdout, "  %s  \n", current_cmd_repr);
    }
    free(current_cmd_repr);
    cmd = cmd->next;
  } while(cmd != NULL);
}

void cmd_fprint(FILE *dest, Command *cmd)
{
  char *repr = cmd_to_string(cmd);
  fprintf(dest, "%s", repr);
  free(repr);
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

static bool is_valid_index(uint8_t param)
{
  return param <= MAX_INDEX;
}

uint8_t get_reg(Prog *prog, uint8_t index)
{
  if(!is_valid_index(index)) {
    fprintf(stderr, "RUNTIME-ERROR: accessing (read) out-of-range register %u\n",
        index);
    return 0;
  }
  return (prog->registers)[index];
}

static uint8_t get_instr_param(Prog *prog, uint8_t param, Instruction_Variant variant)
{
  switch(variant) {
    case CONSTANT: return param;
                   break;
    case STANDARD: return get_reg(prog, param);
                   break;
    case INDIRECT: return get_reg(prog, get_reg(prog, param));
                   break;
    default: fprintf(stderr, "RUNTIME-ERROR: unknown instruction variant\n");
             return param;
             break;
  }
}

void set_reg(Prog *prog, uint8_t index, uint8_t value)
{
  if(!is_valid_index(index)) {
    fprintf(stderr, "RUNTIME-ERROR: accessing (write) out-of-range register %u\n",
        index);
    return;
  }
  (prog->registers)[index] = value;
}

static void set_akku(Prog *prog, uint8_t value)
{
  set_reg(prog, 0, value);
}

static void exec_instruction(Prog *prog, Command *instr)
{
  Instruction_Variant variant = (instr->instruction).variant;
  uint8_t akku_value = get_reg(prog, 0);
  switch((instr->instruction).type) {
    case LOAD:  set_akku(prog, get_instr_param(prog, instr->value, variant));
                break;
    case STORE: set_reg(prog, instr->value, get_instr_param(prog, 0, variant));
                break;
    case ADD:   set_akku(prog, akku_value + get_instr_param(prog, instr->value, variant));
                break;
    case SUB:   set_akku(prog, akku_value - get_instr_param(prog, instr->value, variant));
                break;
    case MULT:  set_akku(prog, akku_value * get_instr_param(prog, instr->value, variant));
                break;
    case DIV:   set_akku(prog, akku_value / get_instr_param(prog, instr->value, variant));
                break;
  }
}

static void jmp_to_label(Prog *prog, char *label)
{
  Command *jmp_target = (Command *)get(prog->label_store, label);
  if(jmp_target == prog->current) {
    fprintf(stderr, "ERROR: jmp target equals jmp origin\n");
  }
  prog->current = jmp_target;
}

static bool eval_cond(Prog *prog, Command *cond)
{
  const uint8_t akku_val = get_reg(prog, 0);
  switch(cond->cmp_type) {
    case EQ:  return akku_val == cond->value;
              break;
    case LEQ: return akku_val <= cond->value;
              break;
    case GEQ: return akku_val >= cond->value;
              break;
    case LE:  return akku_val <  cond->value;
              break;
    case GR:  return akku_val >  cond->value;
              break;
  }
  return false;
}

void step(Prog *prog)
{
  if(!prog) {
    fprintf(stderr, "ERROR: cannot execute command in empty Program\n");
    return;
  }
  Command *current = prog->current;
  if(!current) {
    fprintf(stderr, "RUNTIME-ERROR: Prog has no commands left, maybe you forgot an END somewhere?\n");
    return;
  }
  switch(current->type) {
    case INST:  exec_instruction(prog, current);
                break;
    case JMP:   jmp_to_label(prog, current->label);
                /* specifically invalidate old value */
                current = NULL;
                break;
    case COND:  if(eval_cond(prog, current)) {
                  jmp_to_label(prog, current->label);
                  /* specifically invalidate old value */
                  current = NULL;
                }
                break;
    case LABEL: /* nothing to do on labels */
    case END:   /*               or end    */
                break;
    default:    fprintf(stderr, "RUNTIME-ERROR: executing unrecognized command\n");
                break;
  }
  /* advance the step */
  /*  DO NOT USE current->next instead of prog->current->next since this may
   *  ignore jmps */
  prog->current = prog->current->next;
}

void exec(Prog *prog)
{
  while(prog->current->type != END) {
    step(prog);
  }
}

void print_registers(Prog *prog, uint8_t start, uint8_t end)
{
  const uint8_t higher = end <= MAX_INDEX ? end : MAX_INDEX;
  for(uint8_t index = start; index <= higher; index++) {
    fprintf(stdout, "c(%u) = %u\n", index, get_reg(prog, index));
  }
}

bool is_finished(Prog *prog)
{
  if(!prog) { return true; }
  if(!(prog->current)) {
    fprintf(stderr, "RUNTIME-ERROR: inconsistent program state\n");
    return true;
  }
  return prog->current->type == END;
}
