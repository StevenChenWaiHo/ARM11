#ifndef AEMU_SHIFT_H
#define AEMU_SHIFT_H

#include "emu.h"

typedef struct {
  Instr i;
  bool carry;
} InstrCarry;

InstrCarry emu_shift(CpuState *, Instr);
Instr emu_do_shift(Instr val, Instr by, Instr type);

#endif
