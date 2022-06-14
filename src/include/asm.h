#ifndef AEMU_ASM_H
#define AEMU_ASM_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdnoreturn.h>

#include "cond.h"
#include "core.h"
#include "lexer.h"

typedef enum {
  INSTR_ADD,
  INSTR_AND,
  INSTR_B,
  INSTR_CMP,
  INSTR_EOR,
  INSTR_LDR,
  INSTR_MLA,
  INSTR_MOV,
  INSTR_MUL,
  INSTR_ORR,
  INSTR_RSB,
  INSTR_STR,
  INSTR_SUB,
  INSTR_TEQ,
  INSTR_TST,
} InstrKind;

typedef struct {
  Cond cond;
  InstrKind kind;
} InstrCommon;

typedef struct {
  Lexer lexer;
  FILE *out;
  Token current;
  // TODO: Str->Int map
} Assembler;

InstrCommon instr_common_parse(const char *iname, size_t inamelen);
void assemble(char *src, char *filename, FILE *);

typedef Instr (*AsmFn)(Assembler *, InstrCommon);

Instr asm_br(Assembler *, InstrCommon);
Instr asm_mul(Assembler *, InstrCommon);
Instr asm_sdt(Assembler *, InstrCommon);
Instr asm_dp(Assembler *, InstrCommon);

Token asm_expect(Assembler *, TokenKind);
Token asm_advance(Assembler *);
bool asm_match(Assembler *, TokenKind, Token *);
bool asm_peak(Assembler *, TokenKind);

noreturn void asm_err(Assembler *a, Token *loc, char *fmt, ...)
    __attribute__((format(printf, 3, 4)));

#endif
