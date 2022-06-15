#include <assert.h>

#include "asm.h"
#include "bit_asm.h"

Instr asm_sdt(Assembler *a, InstrCommon c, Instr ino) {
  assert(c.kind == INSTR_STR || c.kind == INSTR_LDR);

  Reg rd = parse_reg_name(asm_expect(a, TOKEN_IDENT));
  asm_expect(a, TOKEN_COMMA);

  Token num;

  Instr ret;

  if (asm_match(a, TOKEN_EQ_NUM, &num)) {
    assert(c.kind == INSTR_LDR);
    // ldr r0,=0x02
    Instr imm_val = asm_parse_number(a, num);
    if (imm_val <= 0xff)
      // mov
      assert(0);
    else {
      Instr constno = asm_add_const(a, imm_val);
      Instr const_off = (constno + a->n_instrs - ino - 2) * 4;

      ret = bit_asm_sdt(/*offset_reg=*/false, /*pre_index=*/true, /*up=*/true,
                        /*ldr=*/true, REG_PC, rd, const_off);
    }
  } else if (asm_match(a, TOKEN_LSQUARE, NULL)) {
    Reg rn = parse_reg_name(asm_expect(a, TOKEN_IDENT));
    asm_expect(a, TOKEN_RSQUARE);
    assert(0);
  } else {
    assert(0); // TODO: Good error.
  }

  asm_expect(a, TOKEN_NEWLINE);
  return ret;
}