#include "emu.h"

void emu_mul(CpuState *cpu, Instr i) {
  Instr a = mul_a_mask(i);
  Instr s = mul_s_mask(i);
  Instr rd = mul_rd_mask(i);
  Instr rn = mul_rn_mask(i);
  Instr rs = mul_rs_mask(i);
  Instr rm = mul_rm_mask(i);

  cpu->regs[rd] = cpu->regs[rm] * cpu->regs[rs];

  if (a == 1) {
    cpu->regs[rd] = cpu->regs[rd] + cpu->regs[rn];
  }

  if (s == 1) {
    uint32_t n_mask = 0x80000000;
    uint32_t z_mask = 0x40000000;
    // Set N flag
    cpu->regs[REG_CPSR] =
        (cpu->regs[REG_CPSR] & ~n_mask) | (cpu->regs[rd] & n_mask);
    // Set Z flag
    if (cpu->regs[rd] == 0) {
      cpu->regs[REG_CPSR] =
          (cpu->regs[REG_CPSR] & ~z_mask) | (cpu->regs[rd] & z_mask);
    }
  }
}
