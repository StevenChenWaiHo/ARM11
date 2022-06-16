#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "bit_asm.h"

#define DP_SHIFT_CONST_START_BIT 7

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

Instr parse_op2(Assembler *a, Instr *i) {
  Token num;
  // Immediate
  if (asm_match(a, TOKEN_HASH_NUM, &num)) {
    *i = 1;
    return asm_parse_imm(a, num);
  }

  Token rmt;
  if (asm_match(a, TOKEN_IDENT, &rmt)) {
    // Register
    *i = 0;
    Reg rm = parse_reg_name(rmt);
    // Check if using Shift
    if (!asm_match(a, TOKEN_COMMA, NULL))
      return rm;

    ShiftKind shift_type = asm_parse_shift_kind(a, asm_expect(a, TOKEN_IDENT));

    Token rs;
    if (asm_match(a, TOKEN_IDENT, &rs))
      return bit_asm_op2_shift_reg(rm, shift_type, parse_reg_name(rs)); // reg
    else
      // Shift by integer
      return bit_asm_op2_shift_imm(
          rm, shift_type,
          asm_parse_imm(a, asm_expect(a, TOKEN_HASH_NUM))); // imm
  }
  assert(0);
}

Instr asm_dp(Assembler *a, InstrCommon c, Instr ino) {
  Instr i = 0;
  Instr s = 0;
  Instr rn = 0;
  Instr rd = 0;
  Instr op2 = 0;
  Token out;

  switch (c.kind) {
  // Result Computing Instructions
  case INSTR_AND:
  case INSTR_EOR:
  case INSTR_SUB:
  case INSTR_RSB:
  case INSTR_ADD:
  case INSTR_ORR:
    rd = parse_reg_name(asm_expect(a, TOKEN_IDENT));
    asm_expect(a, TOKEN_COMMA);
    rn = parse_reg_name(asm_expect(a, TOKEN_IDENT));
    break;

    // Single Operand Assignment
  case INSTR_MOV:
    rd = parse_reg_name(asm_expect(a, TOKEN_IDENT));
    break;

    //  Flag Setting Instructions
  case INSTR_TST:
  case INSTR_TEQ:
  case INSTR_CMP:
    rn = parse_reg_name(asm_expect(a, TOKEN_IDENT));
    s = 1;
    break;
    // Special Case, Convert LSL to MOV
  case INSTR_LSL:
    rd = parse_reg_name(asm_expect(a, TOKEN_IDENT));
    asm_expect(a, TOKEN_COMMA);
    if (asm_match(a, TOKEN_HASH_NUM, &out)) {
      // Shift by integer
      op2 |= rd;
      op2 |= asm_parse_imm(a, out) << DP_SHIFT_CONST_START_BIT;
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
