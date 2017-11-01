#pragma once
#include <stdint.h>
#include "../../util/map.h"

#define MAX_INDEX 100

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
  uint8_t value;
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
  uint8_t registers[MAX_INDEX + 1];
} Prog;

/* allocates necessary ressources, leaves first, last and current at NULL */
Prog *new_prog();

bool append_command(Prog *prog, Command *cmd);

Command *new_instruction(Instruction instruction, uint8_t argument);
Command *new_end();
Command *new_jmp(char *label);
Command *new_cond(Cmp_Type cmp_type, uint8_t cmp_value, char *label);
Command *new_label(char *label);

char *cmd_to_string(Command *cmd);
void cmd_fprint(FILE *dest, Command *cmd);

bool consistency_check(Prog *prog);
void exec(Prog *prog);
void step(Prog *prog);
void set_reg(Prog *prog, uint8_t index, uint8_t value);
uint8_t get_reg(Prog *prog, uint8_t index);
void print_registers(Prog *prog, uint8_t start, uint8_t end);
void print_prog(Prog *prog);
bool is_finished(Prog *prog);
