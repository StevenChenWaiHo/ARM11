#ifndef AEMU_EMU_H
#define AEMU_EMU_H

#include <stdbool.h>
#include <stdint.h>

#include "core.h"
#include "mask.h"

typedef struct {
  uint32_t regs[17];
  uint32_t *mem;
} CpuState;

void emu(CpuState *);
void print_state(CpuState *);

void emu_br(CpuState *, Instr);
void emu_dp(CpuState *, Instr);
void emu_mul(CpuState *, Instr);
void emu_sdt(CpuState *, Instr);

bool emu_cond_eq(CpuState *);
bool emu_cond_ne(CpuState *);
bool emu_cond_ge(CpuState *);
bool emu_cond_lt(CpuState *);
bool emu_cond_gt(CpuState *);
bool emu_cond_le(CpuState *);
bool emu_cond_al(CpuState *);

#endif