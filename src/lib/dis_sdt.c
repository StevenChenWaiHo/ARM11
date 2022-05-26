#include <stdio.h>

#include "core.h"
#include "dis.h"
#include "mask.h"

/*
ldr Load register Rd := (address) Single Data Transfer
str Store register (address) := Rd Single Data Transfer
*/

void dis_sdt(Instr instr, char *cond) {
  Instr i = sdt_i_mask(instr);
  Instr p = sdt_p_mask(instr);
  Instr u = sdt_u_mask(instr);
  Instr l = sdt_l_mask(instr);   // Load or store
  Instr rd = sdt_rd_mask(instr); // Destination register
  Instr rn = sdt_rn_mask(instr); // Base register
  Instr offset = sdt_offset_mask(instr);

  char *iname = l ? "ldr" : "str";

  if (!i && p) {
    // 01 01
    // Immediate offset p306
    // think about UBL
  } else if (i && p) {
    // 01 11
    // Register offset p307/308
    // think about UBL
  } else if (!i && !p) {
    // 01 00
    // Imediate Post indexed p312
    // think about UBL
  } else if (i && !p) {
    // 01 10
    // Register post indexed p313/314
    // think about UBL
  }

  // Page 309 not in spec
}