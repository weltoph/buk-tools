#pragma once
#include <stdint.h>

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
  union {
    uint8_t argument;
    struct {
      bool is_conditional;
      uint8_t cmp_value;
      char label[30];
    } cond_arguments;
  } data;
  Command *next;
  Command *prev;
}

typedef struct {
  Command *first;
  Command *last;
  Command *current;
} Prog;

bool append_instruction(Prog *prog, Instruction instruction, uint8_t argument);
bool append_end(Prog *prog);
bool append_jmp(Prog *prog, char *label);
bool append_cond(Prog *prog, uint8_t cmp_value, char *label);
bool append_label(Prog *prog, char *label);

