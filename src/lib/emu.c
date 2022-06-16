#include <byteswap.h> // TODO: Can we use GNU extension.
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cond.h"
#include "emu.h"
#ifdef AEMU_TRACE
#include "dis.h"
#endif

typedef bool (*CpuCondFn)(CpuState *);

static CpuCondFn condfns[] = {
    [COND_EQ] = emu_cond_eq, [COND_NE] = emu_cond_ne, [COND_GE] = emu_cond_ge,
    [COND_LT] = emu_cond_lt, [COND_GT] = emu_cond_gt, [COND_LE] = emu_cond_le,
    [COND_AL] = emu_cond_al,
};

void emu(CpuState *cpu) {
  uint32_t *imem = cpu->mem;

  for (;;) {
    Instr i = imem[cpu->regs[REG_PC] >> 2];
#ifdef AEMU_TRACE
    // dis(cpu->regs[REG_PC] / 4, i); // TODO: Fix
    printf("CSPR: %08x\n\n", cpu->regs[REG_CPSR]);
#endif
    if (!i) // HLT special case
      break;

    Instr condno = cond_mask(i);
    CpuCondFn cond = condfns[condno];
    if (!cond(cpu)) {
      cpu->regs[REG_PC] += 4;
      continue;
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
      cpu->regs[REG_PC] += 4;
      break;
    case 1: // Single data transfer
      emu_sdt(cpu, i);
      cpu->regs[REG_PC] += 4;
      break;
    case 2: // Branch
      emu_br(cpu, i);
      break;
    default:
      fprintf(stderr, "Unknown type %x\n", type);
      exit(EXIT_FAILURE);
    }
  }
}

void print_state(CpuState *cpu) {

  printf("Registers:\n");
  // TODO: Magic Numbers 13
  for (int i = 0; i < REGISTER_NUMBER - 4; i++) {
    printf("$%-2d : %10d (0x%08x)\n", i, cpu->regs[i], cpu->regs[i]);
  }
  // TODO: Emulate Pipeline (Adding 8 is sus)
  printf("PC  : %10d (0x%08x)\n", cpu->regs[REG_PC] + 8, cpu->regs[REG_PC] + 8);
  printf("CPSR: %10d (0x%08x)\n", cpu->regs[REG_CPSR], cpu->regs[REG_CPSR]);
  printf("Non-zero memory:\n");
  for (int i = 0; i < MEMORY_SIZE / 4; i++) // TODO: put in const
    if (cpu->mem[i])
      printf("0x%08x: 0x%08x\n", i * 4, bswap_32(cpu->mem[i]));
  // TODO: Print Non-zero Memory
}