#include <assert.h>

#include "asm.h"
#include "bit_asm.h"

Instr asm_sdt(Assembler *a, InstrCommon c, Instr ino) {
  assert(c.kind == INSTR_STR || c.kind == INSTR_LDR);

  Reg rd = asm_expect_reg(a);
  asm_expect(a, TOKEN_COMMA);

  Token num;

  Instr ret;

  if (asm_match(a, TOKEN_EQ_NUM, &num)) {
    // assert(c.kind == INSTR_LDR);
    if (c.kind != INSTR_LDR)
      asm_err(a, &num, "Cannot use str with immediate number");

    // ldr r0,=0x02
    bool neg;
    Instr imm_val = asm_parse_number(a, num, &neg);
    assert(!neg);        // TODO: Is this right
    if (imm_val <= 0xff) // mov
      ret = bit_asm_dp(
          /*i=*/true, DP_MOV, /*s=*/false, 0, rd, imm_val);
    else {
      Instr constno = asm_add_const(a, imm_val);
      int64_t const_off = ((int64_t)constno + a->n_instrs - ino - 2) * 4;

      ret = bit_asm_sdt(/*offset_reg=*/false, /*pre_index=*/true,
                        /*up=*/const_off >= 0,
                        /*ldr=*/true, REG_PC, rd,
                        const_off >= 0 ? const_off : -const_off);
    }
  } else if (asm_match(a, TOKEN_LSQUARE, NULL)) {
    Reg rn = asm_expect_reg(a);
    Instr offset = 0;
    bool offset_reg = false;
    bool pre_index = true;
    bool neg = false;
    if (asm_match(a, TOKEN_COMMA, NULL)) {
      Token shtok;
      if (asm_match(a, TOKEN_HASH_NUM, &shtok))
        offset = asm_parse_simm(a, shtok, &neg);
      else {
        Reg reg = asm_expect_reg(a);
        ShiftKind sk = 0;
        Instr shift_by = 0;
        if (asm_match(a, TOKEN_COMMA, NULL)) {
          Token skt = asm_expect(a, TOKEN_IDENT);
          sk = asm_parse_shift_kind(a, skt);
          shift_by = asm_parse_imm(a, asm_expect(a, TOKEN_HASH_NUM));
        }
        offset = bit_asm_op2_shift_imm(reg, sk, shift_by);
        offset_reg = true;
      }
    }
    asm_expect(a, TOKEN_RSQUARE);
    Token tcomma;
    if (asm_match(a, TOKEN_COMMA, &tcomma)) {
      if (offset || offset_reg)
        asm_err(a, &tcomma,
                "Already seen extra pre index arguments, cannot use post index "
                "arguments");
      pre_index = false;

      Token o_reg_2;

      if (asm_match(a, TOKEN_IDENT, &o_reg_2)) {
        offset = asm_parse_reg_name(a, o_reg_2);
        offset_reg = true;
      } else {
        Token im = asm_expect(a, TOKEN_HASH_NUM);
        offset = asm_parse_simm(a, im, &neg);
      }
    }

    ret = bit_asm_sdt(/*offset_reg=*/offset_reg, pre_index, /*up=*/!neg,
                      /*ldr=*/c.kind == INSTR_LDR, rn, rd, offset);
  } else {
    Token t = asm_advance(a);
    asm_err(a, &t, "Expected `[` or `=0x...`, but got `%.*s`",
            (int)t.source.len, t.source.ptr);
  }

  asm_expect(a, TOKEN_NEWLINE);
  return ret;
}
