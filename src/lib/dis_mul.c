#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "mask.h"

// Disasseble Multiply

void dis_mul(Instr i, char *cond) {
  Instr a = mul_a_mask(i);
  Instr s = mul_s_mask(i);
  Instr rd = mul_rd_mask(i);
  Instr rn = mul_rn_mask(i);
  Instr rs = mul_rs_mask(i);
  Instr rm = mul_rm_mask(i);

  if (a) {
    printf("mla%s r%d, r%d, r%d, r%d\n", cond, rd, rm, rs, rn);
  } else {
    printf("mul%s  r%d, r%d, r%d\n", cond, rd, rm, rs);
  }
}