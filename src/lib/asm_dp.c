#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "bit_asm.h"

#define DP_SHIFT_CONST_START_BIT 7
#define DP_SHIFT_REG_START_BIT 8
#define DP_SHIFT_TYPE_START_BIT 5
#define DP_REG_SHIFT_FLAG_TYPE_START_BIT 4
#define IMMEDIATE_SIZE 8
#define ROTATE_START_BIT 8
#define ROTATE_LIMIT 16

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

static Instr rotate_instr(Instr n) { return (n << 2) | (n >> (32 - 2)); }

bool is_valid_imm(Instr imm) {
  for (int i = 0; i < ROTATE_LIMIT; i++) {
    // Valid for value that only uses lowest 8 bits
    if (bit_width(imm) <= IMMEDIATE_SIZE)
      return true;

    // Valid for value that only uses 8 bits (without rotating)
    int lowest_bit = ffs(imm);
    if (bit_width(imm >> lowest_bit) <= IMMEDIATE_SIZE)
      return true;

    i = rotate_instr(i);
  }
  return false;
}

Instr imm_encode(Instr n) {
  // PRE: n is valid imm
  assert(is_valid_imm(n));
  for (int shift = 1; shift < ROTATE_LIMIT; shift++) {
    n = rotate_instr(n);
    if (bit_width(n) <= IMMEDIATE_SIZE)
      return shift << ROTATE_START_BIT | n;
  }
  assert(0); // n is not a valid imm
}

static Instr asm_parse_imm(Assembler *a, Token t) {
  Instr n = asm_parse_number(a, t);
  if (!is_valid_imm(n))
    asm_err(a, &t, "`%.*s` out of range for immediate", (int)t.source.len,
            t.source.ptr);
  if (n > 0xFF)
    n = imm_encode(n);
  return n;
}

Instr parse_op2(Assembler *a, Instr *i) {
  Instr op2 = 0;
  Token out;
  // Immediate
  if (asm_match(a, TOKEN_HASH_NUM, &out)) {
    *i = 1;
    op2 = asm_parse_imm(a, out);
    return op2;
  } else if (asm_match(a, TOKEN_IDENT, &out)) {
    // Register
    *i = 0;
    op2 = parse_reg_name(out);
    // Check if using Shift
    if (!asm_match(a, TOKEN_COMMA, &out)) {
      return op2;
    }
    if (!asm_match(a, TOKEN_IDENT, &out)) {
      assert(0);
    }
    Instr shift_type = asm_parse_instr_common(a, &out).kind;
    switch (shift_type) {
    case INSTR_LSL:
    case INSTR_LSR:
    case INSTR_ASR:
    case INSTR_ROR:
      if (asm_match(a, TOKEN_IDENT, &out)) {
        // Shift by Register
        op2 |= 1 << DP_REG_SHIFT_FLAG_TYPE_START_BIT;
        op2 |= ik_to_dpsk(shift_type) << DP_SHIFT_TYPE_START_BIT;
        op2 |= parse_reg_name(out) << DP_SHIFT_REG_START_BIT;
      } else {
        // Shift by integer
        op2 |= asm_parse_imm(a, out) << DP_SHIFT_CONST_START_BIT;
      }
      break;
    default:
      assert(0);
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
  case INSTR_ANDEQ:
    return 0;
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
