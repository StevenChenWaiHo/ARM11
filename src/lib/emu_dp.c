#include <assert.h>

#include "emu.h"

typedef Instr (*EmuDpFn)(CpuState *, Instr, Instr);

static Instr emu_dp_and(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_eor(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_sub(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_rsb(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_add(CpuState *cpu, Instr rn, Instr op2) { return rn + op2; }
static Instr emu_dp_tst(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_teq(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_cmp(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_orr(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_mov(CpuState *cpu, Instr rn, Instr op2) { return op2; }

static EmuDpFn opcodefn[] = {
    [0] = emu_dp_and,  [1] = emu_dp_eor, [2] = emu_dp_sub, [3] = emu_dp_rsb,
    [4] = emu_dp_add,  [8] = emu_dp_tst, [9] = emu_dp_teq, [10] = emu_dp_cmp,
    [12] = emu_dp_orr, [13] = emu_dp_mov};

static Instr rotation_right(Instr n, Instr d) {
  return (n >> d) | (n << (32 - d));
}

void emu_dp(CpuState *cpu, Instr instr) {
  Instr op = dp_opcode_mask(instr);
  Instr i = dp_i_mask(instr); // Immediate
  Instr s = dp_s_mask(instr); // Set Flags
  Instr rn = dp_rn_mask(instr);
  Instr rd = dp_rd_mask(instr);

  Instr op2;
  if (i) {
    op2 = operand_immediate(dp_operand2_mask(instr));
  } else {
    Instr rm = dp_operand2_rm_mask(instr);
    // TODO: SHift
    op2 = cpu->regs[rm];
  }

  assert(!s);

  cpu->regs[REG_CPSR] = 0;
  EmuDpFn fn = opcodefn[op];
  Instr r = fn(cpu, cpu->regs[rn], op2);
  cpu->regs[rd] = r;
  //   Instr z = r == 0;
  //   Instr n = dp_bit_31_mask(r);
  // TODO: Update flags
}