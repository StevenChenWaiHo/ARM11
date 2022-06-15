#include <assert.h>
#include <stdlib.h>

#include "asm.h"
#include "bit_asm.h"

#define COND_START_BIT 28

#define MUL_TYPE_START_BIT 21

#define MUL_RD_START_BIT 16
#define MUL_RN_START_BIT 12
#define MUL_RS_START_BIT 8
#define MUL_RM_START_BIT 0

#define MUL_FIXED_BIT 9
#define MUL_FIXED_BIT_START 4

Instr asm_mul(Assembler *asmb, InstrCommon c, Instr ino) {
  // Cond should be 1110 for all dp
  assert(c.cond == COND_AL);
  Instr a = 0;
  Instr s = 0;
  Instr rd = 0;
  Instr rn = 0;
  Instr rs = 0;
  Instr rm = 0;

  rd = parse_reg_name(asm_expect(asmb, TOKEN_IDENT));
  asm_expect(asmb, TOKEN_COMMA);
  rm = parse_reg_name(asm_expect(asmb, TOKEN_IDENT));
  asm_expect(asmb, TOKEN_COMMA);
  rs = parse_reg_name(asm_expect(asmb, TOKEN_IDENT));

  if (c.kind == INSTR_MLA) {
    asm_expect(asmb, TOKEN_COMMA);
    rn = parse_reg_name(asm_expect(asmb, TOKEN_IDENT));
    a = 1;
  }
  return bit_asm_mul(a, s, rd, rn, rs, rm);
}
