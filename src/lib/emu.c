#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "emu.h"

typedef bool (*CpuCondFn)(CpuState *);

static CpuCondFn condfns[] = {
    [0] = emu_cond_eq,  [1] = emu_cond_ne,  [10] = emu_cond_ge,
    [11] = emu_cond_lt, [12] = emu_cond_gt, [13] = emu_cond_le,
    [14] = emu_cond_al,
};

void emu(CpuState *cpu) {
  uint32_t *imem = cpu->mem;
  Instr i = imem[cpu->regs[REG_PC] << 2];
  if (!i) // HLT special case
    return;

  fprintf(stderr, "0x%x: %x\n", cpu->regs[REG_PC], i);
  Instr condno = cond_mask(i);
  CpuCondFn cond = condfns[condno];
  if (!cond(cpu)) {
    return;
  }

  Instr type = type_mask(i);
  Instr type_mul = type_mul_mask(i);
  Instr type_mul2 = type_mul2_mask(i);
  switch (type) {
  case 0: // Data processing or multiply
    if (type_mul == 0 && type_mul2 == 9)
      emu_mul(cpu, i);
    else
      emu_dp(cpu, i);
    break;
  case 1: // Single data transfer
    emu_sdt(cpu, i);
    break;
  case 2: // Branch
    emu_br(cpu, i);
    break;
  default:
    fprintf(stderr, "Unknown type %x\n", type);
    exit(EXIT_FAILURE);
  }
}

void print_state(CpuState *cpu) {}