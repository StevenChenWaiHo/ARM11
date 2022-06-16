#include <assert.h>
#include <stdlib.h>

#include "asm.h"
#include "bit_asm.h"

Instr asm_mul(Assembler *a, InstrCommon c, Instr ino) {
  // Cond should be 1110 for all dp
  assert(c.cond == COND_AL);
  Instr a_flag = 0;
  Instr s = 0;
  Reg rd = 0;
  Reg rn = 0;
  Reg rs = 0;
  Reg rm = 0;

  rd = asm_expect_reg(a);
  asm_expect(a, TOKEN_COMMA);
  rm = asm_expect_reg(a);
  asm_expect(a, TOKEN_COMMA);
  rs = asm_expect_reg(a);

  if (c.kind == INSTR_MLA) {
    asm_expect(a, TOKEN_COMMA);
    rn = asm_expect_reg(a);
    a_flag = 1;
  }

  asm_expect(a, TOKEN_NEWLINE);
  return bit_asm_mul(a_flag, s, rd, rn, rs, rm);
}
