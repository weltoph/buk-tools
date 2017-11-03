#pragma once
#include <stdint.h>
#include "../../util/map.h"

#define MAX_INDEX 100
#define VALUE_T uint16_t
#define INDEX_T uint8_t

typedef enum {
  LOAD,
  STORE,
  ADD,
  SUB,
  MULT,
  DIV
} Instruction_Type;

typedef enum {
  CONSTANT,
  INDIRECT,
  STANDARD
} Instruction_Variant;

typedef struct {
  Instruction_Type type;
  Instruction_Variant variant;
} Instruction;

typedef enum {
  EQ,
  LEQ,
  GEQ,
  LE,
  GR
} Cmp_Type;

typedef enum {
  INST,
  END,
  JMP,
  COND,
  LABEL
} Command_Type;

struct Cmd;
typedef struct Cmd Command;

struct Cmd {
  Command_Type type;
  Instruction instruction;
  VALUE_T value;
  char label[30];
  Cmp_Type cmp_type;
  Command *next;
  Command *prev;
};

typedef struct {
  Command *first;
  Command *last;
  Command *current;
  Map *label_store;
  VALUE_T registers[MAX_INDEX + 1];
} Prog;

/* allocates necessary ressources, leaves first, last and current at NULL */
Prog *new_prog();

bool append_command(Prog *prog, Command *cmd);

Command *new_instruction(Instruction instruction, VALUE_T argument);
Command *new_end();
Command *new_jmp(char *label);
Command *new_cond(Cmp_Type cmp_type, VALUE_T cmp_value, char *label);
Command *new_label(char *label);

char *cmd_to_string(Command *cmd);
void cmd_fprint(FILE *dest, Command *cmd);

bool consistency_check(Prog *prog);
bool exec(Prog *prog);
bool step(Prog *prog);
void set_reg(Prog *prog, INDEX_T index, VALUE_T value);
VALUE_T get_reg(Prog *prog, INDEX_T index);
void print_registers(Prog *prog, INDEX_T start, INDEX_T end);
void print_prog(Prog *prog);
bool is_finished(Prog *prog);
