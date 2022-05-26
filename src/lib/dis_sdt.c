#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "dis.h"
#include "mask.h"

/*
ldr Load register Rd := (address) Single Data Transfer
str Store register (address) := Rd Single Data Transfer
*/

static char *shift_name(Instr s) {
  switch (s) {
  case 0:
    return "lsl";
  case 1:
    return "lsr";
  case 2:
    return "asr";
  case 3:
    return "ror"; // TODO: ROR or RRX?
  }
  assert(0); // Invariant
}

void dis_sdt(Instr instr, char *cond) {
  Instr i = sdt_i_mask(instr);   // Addressing mode I
  Instr p = sdt_p_mask(instr);   // Addressing mode II
  Instr u = sdt_u_mask(instr);   // Add (1) or sub (0) to base
  Instr l = sdt_l_mask(instr);   // Load or store
  Instr rd = sdt_rd_mask(instr); // Destination register
  Instr rn = sdt_rn_mask(instr); // Base register
  Instr offset = sdt_offset_mask(instr);

  char *iname = l ? "ldr" : "str";

  // See "Arm Assembly Language: Fundamentals and Tecniques" by William Hohl
  if (!i && p) {
    // 01 01
    // Immediate offset p306
    // think about UBL
    int32_t off_val = u ? offset : -offset;
    if (off_val)
      printf("%s%s %s, [%s, #%d]\n", iname, cond, regname[rd], regname[rn],
             off_val);
    else
      printf("%s%s %s, [%s]\n", iname, cond, regname[rd], regname[rn]);
  } else if (i && p) {
    // 01 11
    // Register offset p307/308
    // think about UBL
    Instr rm = sdt_rm_mask(instr);
    char *sign = u ? "" : "-";
    if (std_noshift_mask(instr)) {
      // 308
      Instr shift_imm = sdt_shift_imm_mask(instr);
      Instr shift = sdt_shift_mask(instr);

      printf("%s%s %s, [%s, %s%s, %s #%d", iname, cond, regname[rd],
             regname[rn], sign, regname[rm], shift_name(shift), shift_imm);
    } else {
      // 307
      printf("%s%s %s, [%s, %s%s]\n", iname, cond, regname[rd], regname[rn],
             sign, regname[rm]);
    }
  } else if (!i && !p) {
    // 01 00
    // Imediate Post indexed p312
    // think about UBL
    printf("TODO");
    exit(-1);
  } else if (i && !p) {
    // 01 10
    // Register post indexed p313/314
    // think about UBL

    if (std_noshift_mask(instr)) {
      printf("TODO");
      exit(-1);
    } else {
      // 313
      Instr rm = sdt_rm_mask(instr);
      char *sign = u ? "" : "-";

      printf("%s%s %s, [%s], %s%s\n", iname, cond, regname[rd], regname[rn],
             sign, regname[rm]);
    }
  }

  // Page 309 not in spec
}