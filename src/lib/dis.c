#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "dis.h"
#include "mask.h"

const char *regname[] = {"r0",  "r1", "r2", "r3", "r4",  "r5",
                         "r6",  "r7", "r8", "r8", "r10", "r11",
                         "r12", "sp", "lr", "pc", "cspr"};
const char *condname[] = {[0] = "eq",  [1] = "ne",  [10] = "ge", [11] = "lt",
                          [12] = "gt", [13] = "le", [14] = ""};
const char *shiftname[] = {[0] = "lsl", [1] = "lsr", [2] = "asr", [3] = "ror"};

void dis(int offset, Instr i) {
  printf("%03d: %08x\n", offset, i);
  Instr condno = cond_mask(i);
  char *cond = condname[condno];

  Instr type = type_mask(i);
  Instr type_mul = type_mul_mask(i);
  Instr type_mul2 = type_mul2_mask(i);

  switch (type) {
  case 0: // Data processing or multiply
    if (type_mul == 0 && type_mul2 == 9)
      dis_mul(i, cond);
    else
      dis_dp(i, cond);
    break;
  case 1: // Single data transfer
    dis_sdt(i, cond);
    break;
  case 2: // Branch
    dis_br(i, cond, offset);
    break;
  default:
    fprintf(stderr, "Unknown type %x\n", type);
    exit(EXIT_FAILURE);
  }
}
