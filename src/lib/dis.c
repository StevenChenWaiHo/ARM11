#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "dis.h"
#include "mask.h"

void dis(int offset, Instr i) {
  printf("%03d: %08x\n", offset, i);
  Instr condno = cond_mask(i);
  char *cond;
  switch (condno) {
  case 0:
    cond = "eq";
    break;
  case 1:
    cond = "ne";
    break;
  case 10:
    cond = "ge";
    break;
  case 11:
    cond = "lt";
    break;
  case 12:
    cond = "gt";
    break;
  case 13:
    cond = "le";
    break;
  case 14:
    cond = "";
    break;
  default:
    fprintf(stderr, "Unknown cond %x\n", condno);
    exit(EXIT_FAILURE);
  }

  Instr type = type_mask(i);
  Instr type_mul = type_mul_mask(i);
  Instr type_mul2 = type_mul2_mask(i);
  
  switch (type) {
  case 0: // Data processing or multiply
    if (type_mul == 0 && type_mul2 == 9) {
      dis_mul(i, cond);
    } else {
      dis_dp(i, cond);
    }
    break;
  case 1: // Single data transfer
    dis_sdt(i, cond);
    break;
  case 2: // Branch
    dis_br(i, cond);
    break;
    printf("TODO\n");
    exit(-1);
  }
}
