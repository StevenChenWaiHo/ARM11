#ifndef AEMU_EMU_H
#define AEMU_EMU_H

#include <stdint.h>

#include "core.h"

typedef struct {
  uint32_t regs[17];
  uint8_t *mem;
} CpuState;

void emu(CpuState *);

void emu_br(CpuState *, Instr);
void emu_dp(CpuState *, Instr);
void emu_mul(CpuState *, Instr);
void emu_sdt(CpuState *, Instr);

#endif