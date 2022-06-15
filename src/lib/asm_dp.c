#include "core.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"

#define COND_START_BIT 28

#define I_START_BIT 25

#define DP_OPCODE_START_BIT 21

#define S_START_BIT 20

#define DP_RN_START_BIT 16
#define DP_RD_START_BIT 12

#define DP_OPERAND2_START_BIT 0
#define DP_OPERAND2_SIZE 32

#define ROTATE_LIMIT 16

bool check_valid_imm(Instr imm) {
  for (int i = 0; i < ROTATE_LIMIT; i++) {
    // Valid for value that only uses lowest 8 bits
    if (imm & ~0xff == 0) {
      return true;
    }

    // Valid for value that only uses 8 bits (without rotating)
    int lowest_bit = ffs(imm);
    if (imm & (~0xff << lowest_bit) == 0) {
      return true;
    }

    i = (i >> 1) | (i << (DP_OPERAND2_SIZE - 1));
  }
  return false;
}

Instr num_skip_prefix(Token token) { return atoi(++token.source.ptr); }

Instr asm_operand2(Token token, Instr *imm_flag) {
  Instr imm = 0;

  switch (*token.source.ptr) {
  case '#':
    *imm_flag = 1;
    if (strncmp(*(token.source.ptr + 1), "0x", 2) == 0) {
      // Hex for Immediate
      imm = strtol((token.source.ptr + 1),
                   (token.source.ptr + token.source.len), 16);
    } else {
      imm = num_skip_prefix(token);
    }

    if (!check_valid_imm(*(token.source.ptr + 1))) {
      perror("Immediate not valid");
    }
    break;

  case 'r':
    *imm_flag = 0;
    break;

  default:
    assert(0);
    break;
  }
  return imm;
}

Instr asm_mul(Assembler *a, InstrCommon c) {

  Instr i = 0;
  Instr rd = 0;
  Token comma;
  Instr rn = 0;
  Token operand2_token;
  Instr imm_flag = 0;
  Instr operand2;

  switch (c.kind) {
  // Result Computing Instructions
  case INSTR_AND:
  case INSTR_EOR:
  case INSTR_SUB:
  case INSTR_RSB:
  case INSTR_ADD:
  case INSTR_ORR:
    rd = skip_prefix(asm_expect(a, TOKEN_IDENT));
    comma = asm_expect(a, TOKEN_COMMA);
    rn = skip_prefix(asm_expect(a, TOKEN_IDENT));
    comma = asm_expect(a, TOKEN_COMMA);
    break;

  // Single Operand Assignment
  case INSTR_MOV:
    rd = skip_prefix(asm_expect(a, TOKEN_IDENT));
    break;

  case INSTR_TST:
  case INSTR_TEQ:
  case INSTR_CMP:
    rn = skip_prefix(asm_expect(a, TOKEN_IDENT));
    break;
  default:
    assert(0);
    break;
  }

  comma = asm_expect(a, TOKEN_COMMA);
  operand2_token = asm_expect(a, TOKEN_IDENT);
  operand2 = asm_operand2(operand2_token, &imm_flag);

  return i | c.cond << COND_START_BIT | imm_flag << I_START_BIT |
         c.kind << DP_OPCODE_START_BIT | rn << DP_RN_START_BIT |
         rd << DP_RD_START_BIT | operand2 << DP_OPERAND2_START_BIT;
}
