#ifndef AEMU_DP_H
#define AEMU_DP_H

#include "core.h"

typedef enum {
  DP_AND = 0,
  DP_EOR = 1,
  DP_SUB = 2,
  DP_RSB = 3,
  DP_ADD = 4,
  DP_TST = 8,
  DP_TEQ = 9,
  DP_CMP = 10,
  DP_ORR = 12,
  DP_MOV = 13,
} DpKind;

typedef enum {
  DP_SHIFT_LSL,
  DP_SHIFT_LSR,
  DP_SHIFT_ASR,
  DP_SHIFT_ROR
} DpShiftKind;

Instr operand_immediate(Instr);

#endif
