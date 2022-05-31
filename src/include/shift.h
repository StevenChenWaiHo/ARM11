#ifndef AEMU_SHIFT_H
#define AEMU_SHIFT_H

#include "core.h"
#include "emu.h"

typedef struct {
  Instr i;
  bool carry;
} InstrCarry;

InstrCarry emu_shift(CpuState *, Instr);

#endif