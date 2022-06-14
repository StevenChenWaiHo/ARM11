#include <assert.h>

#include "asm.h"

Instr asm_sdt(Assembler *a, InstrCommon c) {
  Token reg = asm_expect(a, TOKEN_IDENT);
  reg = asm_expect(a, TOKEN_COMMA);
  reg = asm_expect(a, TOKEN_EQ_NUM);
  reg = asm_expect(a, TOKEN_NEWLINE);
  return 0;
}
