#include "prog.h"

bool append_instruction(Prog *prog, Instruction instruction, uint8_t argument);
bool append_end(Prog *prog);
bool append_jmp(Prog *prog, char *label);
bool append_cond(Prog *prog, uint8_t cmp_value, char *label);
bool append_label(Prog *prog, char *label);

