#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "mask.h"

// Dissaseble Data Processing Instructions

static Instr rotation_right(Instr n, Instr d) { return (n >> d) | (n << (32 - d)); }

Instr operand_immediate(Instr operand2) {
  Instr rotate = dp_operand2_rotate_mask(operand2);
  Instr immediate = dp_operand2_imm_mask(operand2);
  return rotation_right(immediate, 2 * rotate);
}

/*
  TODO: When operand2 is a register (shifting)
  See spec page 7
*/

void print_type_3_args(Instr imm, char *opcode, char *cond, Instr rd, Instr rn,
                  Instr operand2) {
  if (imm) {
    printf("%s%s r%d, r%d, #0x%x\n", opcode, cond, rd, rn,
           operand_immediate(operand2));
  } else {
    printf("%s%s r%d, r%d, r%d\n", opcode, cond, rd, rn, operand2);
  }
}

void print_type_2_args(Instr imm, char *opcode, char *cond, Instr rn,
                  Instr operand2) {
  if (imm) {
    printf("%s%s r%d, #0x%x\n", opcode, cond, rn, operand_immediate(operand2));
  } else {
    printf("%s%s r%d, r%d\n", opcode, cond, rn, operand2);
  }
}

static char *opcodename[] = {
    [0] = "and",  [1] = "eor",  [2] = "sub",  [3] = "rsb",
    [4] = "add",  [8] = "tst",  [9] = "teq",  [10] = "cmp",
    [11] = "cmp", [12] = "orr", [13] = "mov",
};

void dis_dp(Instr i, char *cond) {
  Instr opcodeno = dp_opcode_mask(i);
  char *opcode = opcodename[opcodeno];

  Instr imm = dp_i_mask(i);
  Instr rn = dp_rn_mask(i);
  Instr rd = dp_rd_mask(i);
  Instr operand2 = dp_operand2_mask(i);

  switch (opcodeno) {
  case 0: // and
  case 1: // eor
  case 2: // sub
  case 3: // rsb
  case 4: // add
  case 12: // orr
    print_type_3_args(imm, opcode, cond, rd, rn, operand2);
    break;
  case 8: // tst
  case 9: // teq
  case 10: // cmp
    print_type_2_args(imm, opcode, cond, rn, operand2);
    break;
  case 13: // mov
    print_type_2_args(imm, opcode, cond, rd, operand2);
    break;
  }
}