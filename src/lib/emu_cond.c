#include "emu.h"
#include "mask.h"
#include "unused.h"

static bool cpu_n(CpuState *cpu) { return cpsr_n_mask(cpu->regs[REG_CPSR]); }
static bool cpu_z(CpuState *cpu) { return cpsr_z_mask(cpu->regs[REG_CPSR]); }
// static bool cpu_c(CpuState *cpu) { return cpsr_c_mask(cpu->regs[REG_CPSR]); }
static bool cpu_v(CpuState *cpu) { return cpsr_v_mask(cpu->regs[REG_CPSR]); }

bool emu_cond_eq(CpuState *cpu) { return cpu_z(cpu); }
bool emu_cond_ne(CpuState *cpu) { return !cpu_z(cpu); }
bool emu_cond_ge(CpuState *cpu) { return cpu_n(cpu) == cpu_v(cpu); }
bool emu_cond_lt(CpuState *cpu) { return cpu_n(cpu) != cpu_v(cpu); }
bool emu_cond_gt(CpuState *cpu) {
  return !cpu_z(cpu) && cpu_n(cpu) == cpu_v(cpu);
}
bool emu_cond_le(CpuState *cpu) {
  return cpu_z(cpu) || cpu_n(cpu) != cpu_v(cpu);
}
bool emu_cond_al(UNUSED CpuState *cpu) { return true; }