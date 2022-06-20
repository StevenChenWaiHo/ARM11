#include <assert.h>

#include "asm.h"
#include "bit_asm.h"

Instr parse_offset(Assembler *a, bool *offset_reg, bool *neg) {
  Token shtok;
  if (asm_match(a, TOKEN_HASH_NUM, &shtok)) // [Rn, <#expression>
    return asm_parse_signed_imm(a, shtok, neg);
  else {
    Token sign;
    if (asm_match(a, TOKEN_SIGN, &sign)) { // [Rn, {+/-}
      if (str_eq(sign.source, "-")) {
        *neg = 1;
      } else if (str_eq(sign.source, "+")) {
        assert(*neg == 0);
        *neg = 0;
      }
      asm_err(a, &sign, "Unknown token for TOKEN_SIGN");
    }

    Reg reg = asm_parse_reg_name(asm_expect(a, TOKEN_IDENT)); // [Rn, {+/-}Rm
    *offset_reg = true;
    return asm_parse_shift_reg(a, reg); // [Rn, {+/-}Rm{,<shift>}]
  }
  return 0; // No offset
}

Instr asm_sdt(Assembler *a, InstrCommon c, Instr ino) {
  assert(c.kind == INSTR_STR || c.kind == INSTR_LDR);

  Reg rd = asm_parse_reg_name(asm_expect(a, TOKEN_IDENT));
  asm_expect(a, TOKEN_COMMA);
  Token num;
  Instr ret;

  if (asm_match(a, TOKEN_EQ_NUM, &num)) { // <=expression> (signed)
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
    Reg rn = asm_parse_reg_name(asm_expect(a, TOKEN_IDENT));
    Instr offset = 0;
    bool offset_reg = false;
    bool pre_index = true;
    bool neg = false;
    if (asm_match(a, TOKEN_COMMA, NULL)) {         // [Rn,
      offset = parse_offset(a, &offset_reg, &neg); // [Rn, {+/-}Rm{, <shift>}
    }

    asm_expect(a, TOKEN_RSQUARE);
    if (asm_match(a, TOKEN_COMMA, NULL)) { // [Rn],
      assert(offset == 0);
      assert(offset_reg == false);
      pre_index = false;
      offset = parse_offset(a, &offset_reg, &neg); // [Rn], {+/-}Rm{, <shift>}
    }

    ret = bit_asm_sdt(/*offset_reg=*/offset_reg, pre_index, /*up=*/!neg,
                      /*ldr=*/c.kind == INSTR_LDR, rn, rd, offset);
  } else {
    assert(0); // TODO: Good error.
  }

  asm_expect(a, TOKEN_NEWLINE);
  return ret;
}
