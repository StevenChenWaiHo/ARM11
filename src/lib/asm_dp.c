#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "bit_asm.h"

// #define DP_RN_START_BIT 16
// #define DP_RD_START_BIT 12

#define DP_OPERAND2_START_BIT 0
#define DP_OPERAND2_SIZE 32

#define ROTATE_LIMIT 16

#define DP_SHIFT_CONST_START_BIT 7
#define DP_SHIFT_REG_START_BIT 8

bool check_valid_imm(Instr imm) {
  for (int i = 0; i < ROTATE_LIMIT; i++) {
    // Valid for value that only uses lowest 8 bits
    if ((imm & ~0xff) == 0) {
      return true;
    }

    // Valid for value that only uses 8 bits (without rotating)
    int lowest_bit = ffs(imm);
    if ((imm & ~(0xff << lowest_bit)) == 0) {
      return true;
    }

    i = (i >> 1) | (i << (DP_OPERAND2_SIZE - 1));
  }
  return false;
}

Instr parse_op2(Assembler *a, Instr *i) {
  Instr op2 = 0;
  Token out;
  // Immediate
  if (asm_match(a, TOKEN_HASH_NUM, &out)) {
    *i = 1;
    op2 = asm_parse_number(a, out);
    return op2;
  }
  // Register
  if (asm_match(a, TOKEN_IDENT, &out)) {
    *i = 1;
    op2 = asm_parse_number(a, out);
    // Check if using Shift Register
    if (asm_match(a, TOKEN_COMMA, &out)) {
      // Shift by Register
      if (asm_match(a, TOKEN_IDENT, &out)) {
        op2 |= 1;
        op2 |= parse_reg_name(out) << DP_SHIFT_REG_START_BIT;
      } else if (asm_match(a, TOKEN_IDENT, &out)) {
        op2 |= asm_parse_number(a, out) << DP_SHIFT_CONST_START_BIT;
      }
    }
    return op2;
  }
  assert(0);
}

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
  default:
    assert(0); // Invariant
  }
}

Instr asm_dp(Assembler *a, InstrCommon c, Instr ino) {
  // Cond should be 1110 for all dp
  assert(c.cond == COND_AL);

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
  default:
    assert(0);
    break;
  }

  asm_expect(a, TOKEN_COMMA);
  op2 = parse_op2(a, &i);
  asm_expect(a, TOKEN_NEWLINE);

  return bit_asm_dp(i, ik_to_dpk(c.kind), s, rn, rd, op2);
}
