#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "bit_asm.h"

static DpKind ik_to_dpk(InstrKind ik) {
  switch (ik) {
  case INSTR_AND:
    return DP_AND;
  case INSTR_EOR:
    return DP_EOR;
  case INSTR_SUB:
    return DP_SUB;
  case INSTR_RSB:
    return DP_RSB;
  case INSTR_ADD:
    return DP_ADD;
  case INSTR_TST:
    return DP_TST;
  case INSTR_TEQ:
    return DP_TEQ;
  case INSTR_CMP:
    return DP_CMP;
  case INSTR_ORR:
    return DP_ORR;
  case INSTR_MOV:
    return DP_MOV;
  case INSTR_LSL:
    return DP_MOV; // LSL is translated to MOV
  default:
    assert(0); // Invariant
  }
}

// Up to 12 bits.
Instr parse_op2(Assembler *a, bool *i) {
  Token num;
  // Immediate
  if (asm_match(a, TOKEN_HASH_NUM, &num)) {
    *i = true;
    return asm_parse_imm(a, num);
  }
  *i = false;
  Reg rm = asm_expect_reg(a);
  return asm_parse_shift_reg(a, rm);
}

Instr asm_dp(Assembler *a, InstrCommon c, Instr ino) {
  bool i = false;
  bool s = false;
  Instr rn = 0;
  Instr rd = 0;
  Instr op2 = 0;

  switch (c.kind) {
  // Result Computing Instructions
  case INSTR_AND:
  case INSTR_EOR:
  case INSTR_SUB:
  case INSTR_RSB:
  case INSTR_ADD:
  case INSTR_ORR:
    rd = asm_expect_reg(a);
    asm_expect(a, TOKEN_COMMA);
    rn = asm_expect_reg(a);
    break;

    // Single Operand Assignment
  case INSTR_MOV:
    rd = asm_expect_reg(a);
    break;

    //  Flag Setting Instructions
  case INSTR_TST:
  case INSTR_TEQ:
  case INSTR_CMP:
    rn = asm_expect_reg(a);
    s = true;
    break;
    // Special Case, Convert LSL to MOV
  case INSTR_LSL:
    rd = asm_expect_reg(a);
    asm_expect(a, TOKEN_COMMA);
    Token imm;
    if (asm_match(a, TOKEN_HASH_NUM, &imm)) {
      // Shift by integer
      Instr imm_instr = asm_parse_shift_imm(a, imm);
      op2 = bit_asm_op2_shift_imm(rd, SHIFT_LSL, imm_instr);
    }
    asm_expect(a, TOKEN_NEWLINE);
    return bit_asm_dp(i, ik_to_dpk(c.kind), s, rn, rd, op2);
    break;
  default:
    assert(0);
    break;
  }

  asm_expect(a, TOKEN_COMMA);
  op2 = parse_op2(a, &i);
  asm_expect(a, TOKEN_NEWLINE);

  return bit_asm_dp(i, ik_to_dpk(c.kind), s, rn, rd, op2);
}
