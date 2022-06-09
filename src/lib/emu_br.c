#ifdef AEMU_TRACE
#include <stdio.h>
#endif

#include "emu.h"

void emu_br(CpuState *cpu, Instr i) {
  Instr offset = br_offset_mask(i);
  offset <<= 2;
  if ((offset >> 25) & 0x1) {
    offset |= 0xFC000000;
  }
  offset += 8;
#ifdef AEMU_TRACE
  fprintf(stderr, "branch to %x\n", cpu->regs[REG_PC] + offset);
#endif

  cpu->regs[REG_PC] += offset;
}
