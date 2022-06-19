#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "bit_asm.h"

#define DP_SHIFT_CONST_START_BIT 7
#define DP_SHIFT_CONST_MAX 0x1F

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
    Reg rm = parse_reg_name(rmt);
    *i = 0;
    return parse_shift_reg(a, rm);
  }
  assert(0);
}

Instr asm_dp(Assembler *a, InstrCommon c, Instr ino) {
  Instr i = 0;
  Instr s = 0;
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
    Token imm;
    asm_expect(a, TOKEN_COMMA);
    if (asm_match(a, TOKEN_HASH_NUM, &imm)) {
      // Shift by integer
      Instr imm_instr = asm_parse_imm(a, imm);
      if (imm_instr > DP_SHIFT_CONST_MAX) {
        asm_err(a, &imm, "Const too large for a shift const: %d", imm_instr);
      }
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
