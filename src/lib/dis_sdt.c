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

void dis_sdt(FILE *f, Instr instr, const char *cond) {
  Instr amode = sdt_mode_mask(instr);
  Instr ispos = sdt_u_mask(instr);
  Instr isload = sdt_l_mask(instr);
  Instr rd = sdt_rd_mask(instr);
  Instr rn = sdt_rn_mask(instr);
  Instr rm = sdt_rm_mask(instr);
  Instr offset = sdt_offset_mask(instr);
#ifdef AEMU_TRACE
  printf("Offset = %d\n", offset);
#endif

  char *iname = isload ? "ldr" : "str";
  char *sign = ispos ? "" : "-";

  // See "Arm Assembly Language: Fundamentals and Tecniques" by William Hohl

  switch (amode) {
  case 0: // 01 00, Imediate Post indexed p312
    fprintf(f, "%s%s %s, [%s], #%s%d\n", iname, cond, regname[rd], regname[rn],
            sign, offset);
    break;
  case 1: // 01 01. Immediate offset p306
    if (offset)
      fprintf(f, "%s%s %s, [%s, #%s%d]\n", iname, cond, regname[rd],
              regname[rn], sign, offset);
    else
      fprintf(f, "%s%s %s, [%s]\n", iname, cond, regname[rd], regname[rn]);
    break;
  case 2: // 01 10, Register post indexed. p313/314
    if (std_noshift_mask(instr)) {
      fprintf(f, "TODO");
      exit(-1);
    } else { // 313
      fprintf(f, "%s%s %s, [%s], %s%s\n", iname, cond, regname[rd], regname[rn],
              sign, regname[rm]);
    }
    break;
  case 3:                          // 01 11 Register offset p307/308
    if (std_noshift_mask(instr)) { // 308

      Instr shift_imm = sdt_shift_imm_mask(instr);
      Instr shift = sdt_shift_mask(instr);
      fprintf(f, "%s%s %s, [%s, %s%s, %s #%d]\n", iname, cond, regname[rd],
              regname[rn], sign, regname[rm], shiftname[shift], shift_imm);
    } else { // 307
      fprintf(f, "%s%s %s, [%s, %s%s]\n", iname, cond, regname[rd], regname[rn],
              sign, regname[rm]);
    }
    break;
  default:
    assert(0); // Invariant
  }

  // Page 309 not in spec
}
