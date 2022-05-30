#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "dis.h"
#include "mask.h"

// Dissasseble Data Processing Instructions

static char *opcodename[] = {
    [0] = "and",  [1] = "eor",  [2] = "sub", [3] = "rsb",
    [4] = "add",  [8] = "tst",  [9] = "teq", [10] = "cmp",
    [11] = "cmp", [12] = "orr", [13] = "mov"};

static Instr rotation_right(Instr n, Instr d) {
  return (n >> d) | (n << (32 - d));
}

Instr operand_immediate(Instr operand2) {
  Instr rotate = dp_operand2_rotate_mask(operand2);
  Instr immediate = dp_operand2_imm_mask(operand2);
  return rotation_right(immediate, 2 * rotate);
}

void operand_register_3_args(int opcodeno, char *cond, Instr rd, Instr rn,
                             Instr operand2) {
  Instr rm = dp_operand2_rm_mask(operand2);
  Instr ind = dp_operand2_shift_ind_mask(operand2);
  Instr shift_type = dp_operand2_shift_type_mask(operand2);
  char *new_opcode = shiftname[shift_type];
  if (ind) { // Operand2 is an immediate value
    Instr rs = dp_operand2_shift_rs_mask(operand2);
    printf("%s%s r%d, r%d, r%d, %s r%d\n", opcodename[opcodeno], cond, rd, rn,
           rm, new_opcode, rs);
  } else {
    printf("%s%s r%d, r%d, r%d\n", opcodename[opcodeno], cond, rd, rn,
           operand2);
  }
}

void operand_register_2_args(int opcodeno, char *cond, Instr rn,
                             Instr operand2) {
  Instr rm = dp_operand2_rm_mask(operand2);
  Instr ind = dp_operand2_shift_ind_mask(operand2);
  Instr shift_type = dp_operand2_shift_type_mask(operand2);
  char *new_opcode = shiftname[shift_type];
  if (ind) {
    Instr rs = dp_operand2_shift_rs_mask(operand2);
    printf("%s%s r%d, r%d, %s r%d\n", opcodename[opcodeno], cond, rn, rm,
           new_opcode, rs);
  } else { // shift ops has the opcode of mov
    Instr shift_by = dp_operand2_shift_const_mask(operand2);
    if (!shift_by || opcodeno != 13) { // 13 - mov
      printf("%s%s r%d, r%d\n", opcodename[opcodeno], cond, rn, operand2);
    } else {
      printf("%s%s r%d, r%d, #0x%x\n", new_opcode, cond, rn, rn, shift_by);
    }
  }
}

void print_type_3_args(Instr imm, int opcodeno, char *cond, Instr rd, Instr rn,
                       Instr operand2) {
  if (imm) {
    printf("%s%s r%d, r%d, #0x%x\n", opcodename[opcodeno], cond, rd, rn,
           operand_immediate(operand2));
  } else {
    operand_register_3_args(opcodeno, cond, rd, rn, operand2);
  }
}

void print_type_2_args(Instr imm, int opcodeno, char *cond, Instr rn,
                       Instr operand2) {
  if (imm) {
    printf("%s%s r%d, #0x%x\n", opcodename[opcodeno], cond, rn,
           operand_immediate(operand2));
  } else {
    operand_register_2_args(opcodeno, cond, rn, operand2);
  }
}

void dis_dp(Instr i, char *cond) {
  Instr opcodeno = dp_opcode_mask(i);

  Instr imm = dp_i_mask(i);
  Instr rn = dp_rn_mask(i);
  Instr rd = dp_rd_mask(i);
  Instr operand2 = dp_operand2_mask(i);

  switch (opcodeno) {
  case 0:  // and
  case 1:  // eor
  case 2:  // sub
  case 3:  // rsb
  case 4:  // add
  case 12: // orr
    print_type_3_args(imm, opcodeno, cond, rd, rn, operand2);
    break;
  case 8:  // tst
  case 9:  // teq
  case 10: // cmp
    print_type_2_args(imm, opcodeno, cond, rn, operand2);
    break;
  case 13: // mov
    print_type_2_args(imm, opcodeno, cond, rd, operand2);
    break;
  }
}