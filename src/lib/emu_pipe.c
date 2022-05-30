#include <byteswap.h> // TODO: Can we use GNU extension.
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
  Instr fetch = 0;
  bool decode_empty = true;
  bool fetch_empty = true;
  Instr decode = 0;

  for (;;) {

#ifdef AEMU_TRACE
    fprintf(stderr, "0x%x: %x\n", cpu->regs[REG_PC], i);
#endif

    // Execute
    if (!decode_empty) {
      if (!decode) // HLT special case
        break;

      Instr condno = cond_mask(decode);
      CpuCondFn cond = condfns[condno];
      if (!cond(cpu))
        continue;

      Instr type = type_mask(decode);
      Instr type_mul = type_mul_mask(decode);
      Instr type_mul2 = type_mul2_mask(decode);
      switch (type) {
      case 0: // Data processing or multiply
        if (type_mul == 0 && type_mul2 == 9)
          emu_mul(cpu, decode);
        else
          emu_dp(cpu, decode);
        break;
      case 1: // Single data transfer
        emu_sdt(cpu, decode);
        break;
      case 2: // Branch
        decode_empty = true;
        emu_br(cpu, decode);
        break;
      default:
        fprintf(stderr, "Unknown type %x\n", type);
        exit(EXIT_FAILURE);
      }
    }

    // Decode
    if (!fetch_empty) {
      decode = fetch;
      decode_empty = false;
    }

    // Fetch
    fetch = imem[cpu->regs[REG_PC] >> 2];
    fetch_empty = false;

    // Increment PC
    cpu->regs[REG_PC] += 4;
  }
}

void print_state(CpuState *cpu) {

  printf("Registers:\n");
  // TODO: Magic Numbers 13
  for (int i = 0; i < 13; i++) {
    printf("$%-2d : %10d (0x%08x)\n", i, cpu->regs[i], cpu->regs[i]);
  }
  // TODO: Emulate Pipeline (Adding 8 is sus)
  printf("PC  : %10d (0x%08x)\n", cpu->regs[REG_PC], cpu->regs[REG_PC]);
  printf("CPSR: %10d (0x%08x)\n", cpu->regs[REG_CPSR], cpu->regs[REG_CPSR]);
  printf("Non-zero memory:\n");
  for (int i = 0; i < 16384; i++) // TODO: put in const
    if (cpu->mem[i])
      printf("0x%08x: 0x%08x\n", i * 4, bswap_32(cpu->mem[i]));
  // TODO: Print Non-zero Memory
}