#include <assert.h>
#include <stdlib.h>

#include "asm.h"
#include "bit_asm.h"

Instr asm_mul(Assembler *asmb, InstrCommon c, Instr ino) {
  // Cond should be 1110 for all dp
  assert(c.cond == COND_AL);
  Instr a = 0;
  Instr s = 0;
  Instr rd = 0;
  Instr rn = 0;
  Instr rs = 0;
  Instr rm = 0;

  rd = asm_parse_reg_name(asm_expect(asmb, TOKEN_IDENT));
  asm_expect(asmb, TOKEN_COMMA);
  rm = asm_parse_reg_name(asm_expect(asmb, TOKEN_IDENT));
  asm_expect(asmb, TOKEN_COMMA);
  rs = asm_parse_reg_name(asm_expect(asmb, TOKEN_IDENT));

  if (c.kind == INSTR_MLA) {
    asm_expect(asmb, TOKEN_COMMA);
    rn = asm_parse_reg_name(asm_expect(asmb, TOKEN_IDENT));
    a = 1;
  }

  asm_expect(asmb, TOKEN_NEWLINE);
  return bit_asm_mul(a, s, rd, rn, rs, rm);
}
