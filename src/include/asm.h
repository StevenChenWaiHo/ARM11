#ifndef AEMU_ASM_H
#define AEMU_ASM_H

#include <stddef.h>

#include "cond.h"

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

InstrCommon instr_common_parse(const char *iname, size_t inamelen);
#endif
