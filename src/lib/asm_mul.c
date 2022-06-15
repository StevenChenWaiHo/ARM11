#include <assert.h>
#include <stdlib.h>

#include "asm.h"

#define COND_START_BIT 28

#define MUL_TYPE_START_BIT 21

#define MUL_RD_START_BIT 16
#define MUL_RN_START_BIT 12
#define MUL_RS_START_BIT 8
#define MUL_RM_START_BIT 0

#define MUL_FIXED_BIT 9
#define MUL_FIXED_BIT_START 4

Instr num_skip_prefix(Token token) { return atoi(++token.source.ptr); }

Instr asm_mul(Assembler *a, InstrCommon c) {

  // Cond should be 1110 for all dp

  Instr i = 0;
  assert(c.cond == COND_AL);

  Instr rd = num_skip_prefix(asm_expect(a, TOKEN_IDENT));
  Token comma = asm_expect(a, TOKEN_COMMA);
  Instr rm = num_skip_prefix(asm_expect(a, TOKEN_IDENT));
  comma = asm_expect(a, TOKEN_COMMA);
  Instr rs = num_skip_prefix(asm_expect(a, TOKEN_IDENT));

  if (c.kind == INSTR_MLA) {
    comma = asm_expect(a, TOKEN_COMMA);
    Instr rn = num_skip_prefix(asm_expect(a, TOKEN_IDENT));
    i |= 1 << MUL_TYPE_START_BIT | rn << MUL_RN_START_BIT;
  }
  return c.cond << COND_START_BIT | rd << MUL_RD_START_BIT |
         rs << MUL_RS_START_BIT | MUL_FIXED_BIT << MUL_FIXED_BIT_START |
         rm << MUL_RM_START_BIT | i;
}
