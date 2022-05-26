#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "mask.h"

// Disaseble Data Processing Instructions

void dis_dp(Instr i, char *cond) {
  Instr opcodeno = dp_opcode_mask(i);
  char *opcode;
  switch (opcodeno) {
  case 0:
    opcode = "and";
    break;
  case 1:
    opcode = "eor";
    break;
  case 2:
    opcode = "sub";
    break;
  case 3:
    opcode = "rsb";
    break;
  case 4:
    opcode = "add";
    break;
  case 8:
    opcode = "tst";
    break;
  case 9:
    opcode = "teq";
    break;
  case 10:
    opcode = "cmp";
    break;
  case 12:
    opcode = "orr";
    break;
  case 13:
    opcode = "mov";
    break;
  default:
    printf("Unknown opcode %d\n", opcodeno);
    exit(-1);
  }

  Instr imm = dp_i_mask(i);
  Instr rn = dp_rn_mask(i);
  Instr rd = dp_rd_mask(i);
  Instr operand2 = dp_operand2_mask(i);

  printf("%s%s r%d, r%d, #%d\n", opcode, cond, rn, rd, operand2);
}