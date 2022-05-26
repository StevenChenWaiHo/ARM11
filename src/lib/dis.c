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
  // printf(" cond: %s\n", cond);
  Instr type = type_mask(i);
  // printf(" type: %d\n", type);
  switch (type) {
  case 0: // Data processing or multiply
          // TODO: Is this data processing or multiply?
    dis_dp(i, cond);
    break;
  case 1: // Single data transfer
    dis_sdt(i, cond);
    break;
  case 2: // Branch
    printf("TODO\n");
    exit(-1);
  }
}
