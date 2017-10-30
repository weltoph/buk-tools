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

typedef struct {
  Command_Type type;
  union {
    uint8_t argument;
    char label[30];
    struct {
      uint8_t cond_value;
      char label[30];
    } cond_arguments;
  } 
} Command;
