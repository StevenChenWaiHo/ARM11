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
    bool neg;
    Instr imm_val = asm_parse_number(a, num, &neg);
    assert(!neg);        // TODO: Is this right
    if (imm_val <= 0xff) // mov
      ret = bit_asm_dp(
          /*i=*/true, DP_MOV, /*s=*/false, 0, rd, imm_val);
    else {
      Instr constno = asm_add_const(a, imm_val);
      Instr const_off = (constno + a->n_instrs - ino - 2) * 4;

      ret = bit_asm_sdt(/*offset_reg=*/false, /*pre_index=*/true, /*up=*/true,
                        /*ldr=*/true, REG_PC, rd, const_off);
    }
  } else if (asm_match(a, TOKEN_LSQUARE, NULL)) {
    Reg rn = parse_reg_name(asm_expect(a, TOKEN_IDENT));
    Instr offset = 0;
    bool offset_reg = false;
    bool pre_index = true;
    bool neg = false;
    if (asm_match(a, TOKEN_COMMA, NULL)) {
      Token shtok;
      if (asm_match(a, TOKEN_HASH_NUM, &shtok))
        offset = asm_parse_simm(a, shtok, &neg);
      else {
        Reg reg = parse_reg_name(asm_expect(a, TOKEN_IDENT));
        offset = reg;
        offset_reg = true;
      }
    }
    asm_expect(a, TOKEN_RSQUARE);
    if (asm_match(a, TOKEN_COMMA, NULL)) {
      Token o_reg_2 = asm_expect(a, TOKEN_IDENT);
      assert(offset == 0);
      assert(offset_reg == false);
      offset = parse_reg_name(o_reg_2);
      offset_reg = true;
      pre_index = false;
    }

    ret = bit_asm_sdt(/*offset_reg=*/offset_reg, pre_index, /*up=*/!neg,
                      /*ldr=*/c.kind == INSTR_LDR, rn, rd, offset);
  } else {
    assert(0); // TODO: Good error.
  }

  asm_expect(a, TOKEN_NEWLINE);
  return ret;
}
