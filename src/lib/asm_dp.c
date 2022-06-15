#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"

#define COND_START_BIT 28

#define I_START_BIT 25

#define OPCODE_START_BIT 21

#define S_START_BIT 20

#define DP_RN_START_BIT 16
#define DP_RD_START_BIT 12

#define DP_OPERAND2_START_BIT 0

Instr num_skip_prefix(Token token) { return atoi(++token.source.ptr); }

Instr asm_operand2(Token token, Instr *imm) {
  switch (*token.source.ptr) {
  case '#':
    *imm = 1;
    // TODO: Check valid Immediate
    if (!check_valid_imm(*(token.source.ptr + 1))) {
      perror("Immediate not valid");
    }
    // Hex for Immediate
    if (strncmp(*(token.source.ptr + 1), "0x", 2) == 0) {
      return strtol((token.source.ptr + 1),
                    (token.source.ptr + token.source.len), 16);
    }
    break;

  case 'r':
    *imm = 0;
    break;

  default:
    assert(0);
    break;
  }
  return num_skip_prefix(token);
}

Instr asm_mul(Assembler *a, InstrCommon c) {

  Instr i = 0;
  Instr rd = 0;
  Token comma;
  Instr rn = 0;
  Token operand2_token;
  Instr imm = 0;
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
  operand2 = asm_operand2(operand2_token, &imm);

  return i | c.cond << COND_START_BIT | imm << I_START_BIT |
         c.kind << OPCODE_START_BIT | rn << DP_RN_START_BIT |
         rd << DP_RD_START_BIT | operand2 << DP_OPERAND2_START_BIT;
}
