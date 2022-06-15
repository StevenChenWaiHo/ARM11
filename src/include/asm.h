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
  size_t n_instrs;
  // TODO: Str->Int map
  Instr *consts;
  size_t n_consts;
} Assembler;

InstrCommon instr_common_parse(Str instr);
void assemble(char *src, char *filename, FILE *);

typedef Instr (*AsmFn)(Assembler *, InstrCommon, Instr);

Instr asm_br(Assembler *a, InstrCommon c, Instr ino);
Instr asm_mul(Assembler *a, InstrCommon c, Instr ino);
Instr asm_sdt(Assembler *a, InstrCommon c, Instr ino);
Instr asm_dp(Assembler *a, InstrCommon c, Instr ino);

Token asm_expect(Assembler *, TokenKind);
Token asm_advance(Assembler *);
bool asm_match(Assembler *, TokenKind, Token *);
bool asm_peak(Assembler *, TokenKind);

Instr asm_parse_number(Assembler *, Token);
Instr asm_add_const(Assembler *, Instr);

Reg parse_reg_name(Token);

noreturn void asm_err(Assembler *a, Token *loc, char *fmt, ...)
    __attribute__((format(printf, 3, 4)));

#endif