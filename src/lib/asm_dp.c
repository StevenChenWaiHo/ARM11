#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "bit_asm.h"

#define DP_SHIFT_CONST_START_BIT 7
#define DP_SHIFT_REG_START_BIT 8
#define DP_SHIFT_TYPE_START_BIT 5
#define DP_REG_SHIFT_FLAG_TYPE_START_BIT 4

#define I_START_BIT 25

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
  case INSTR_ANDEQ:
    return DP_ADD; // opcode AND and ANDEQ is 0
  case INSTR_LSL:
    return DP_MOV; // LSL is translated to MOV
  default:
    assert(0); // Invariant
  }
}

static DpShiftKind ik_to_dpsk(Instr ik) {
  switch (ik) {
  case INSTR_LSL:
    return DP_SHIFT_LSL;
  case INSTR_LSR:
    return DP_SHIFT_LSR;
  case INSTR_ASR:
    return DP_SHIFT_ASR;
  case INSTR_ROR:
    return DP_SHIFT_ROR;
  default:
    assert(0); // Invariant
  }
}

Instr parse_op2(Assembler *a, Instr *i) {
  Instr op2 = 0;
  Token imm;
  Token rm;
  Token shift_type;
  Token rs;
  Token shift_imm;

  Instr shift_type_instr;
  // Immediate
  if (asm_match(a, TOKEN_HASH_NUM, &imm)) {
    *i = 1;
    op2 = asm_parse_number(a, imm);
    return op2;
  } else if (asm_match(a, TOKEN_IDENT, &rm)) {
    // Register
    *i = 0;
    op2 = parse_reg_name(rm);
    // Check if using Shift
    if (!asm_match(a, TOKEN_COMMA, NULL)) {
      return op2;
    }
    shift_type_instr =
        instr_common_parse(asm_expect(a, TOKEN_IDENT).source).kind;
    switch (shift_type_instr) {
    case INSTR_LSL:
    case INSTR_LSR:
    case INSTR_ASR:
    case INSTR_ROR:
      if (asm_match(a, TOKEN_IDENT, &rs)) {
        // Shift by Register
        op2 |= 1 << DP_REG_SHIFT_FLAG_TYPE_START_BIT;
        op2 |= ik_to_dpsk(shift_type_instr) << DP_SHIFT_TYPE_START_BIT;
        op2 |= parse_reg_name(rs) << DP_SHIFT_REG_START_BIT;
      } else {
        // Shift by integer
        shift_imm = asm_expect(a, TOKEN_HASH_NUM);
        op2 |= asm_parse_number(a, shift_imm) << DP_SHIFT_CONST_START_BIT;
      }
      break;
    default:
      assert(0); // Unknown shift type instr
      break;
    }
    return op2;
  }
  assert(0);
}

Instr asm_dp(Assembler *a, InstrCommon c, Instr ino) {
  Instr i = 0;
  Instr s = 0;
  Instr rn = 0;
  Instr rd = 0;
  Instr op2 = 0;
  Token shift_imm;

  switch (c.kind) {
  // Result Computing Instructions
  case INSTR_AND:
    if (c.cond == COND_EQ) // Special Case ANDEQ
    {
      return 0;
    }
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
  case INSTR_LSL:
    rd = parse_reg_name(asm_expect(a, TOKEN_IDENT));
    asm_expect(a, TOKEN_COMMA);
    shift_imm = asm_expect(a, TOKEN_HASH_NUM);
    op2 |= rd;
    op2 |= asm_parse_number(a, shift_imm) << DP_SHIFT_CONST_START_BIT;
    asm_expect(a, TOKEN_NEWLINE);
    return bit_asm_dp(i, ik_to_dpk(c.kind), s, rn, rd, op2);
    break;
  default:
    assert(0); // Unknown instr
    break;
  }

  asm_expect(a, TOKEN_COMMA);
  op2 = parse_op2(a, &i);
  asm_expect(a, TOKEN_NEWLINE);

  return bit_asm_dp(i, ik_to_dpk(c.kind), s, rn, rd, op2);
}
