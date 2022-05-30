#include <assert.h>

#include "dp.h"
#include "emu.h"

typedef Instr (*EmuDpFn)(CpuState *, Instr, Instr);

static Instr emu_dp_and(CpuState *cpu, Instr rn, Instr op2) { return rn & op2; }
static Instr emu_dp_eor(CpuState *cpu, Instr rn, Instr op2) { return rn ^ op2; }
static Instr emu_dp_sub(CpuState *cpu, Instr rn, Instr op2) { return rn - op2; }
static Instr emu_dp_rsb(CpuState *cpu, Instr rn, Instr op2) { return op2 - rn; }
static Instr emu_dp_add(CpuState *cpu, Instr rn, Instr op2) { return rn + op2; }
static Instr emu_dp_tst(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_teq(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_cmp(CpuState *cpu, Instr rn, Instr op2) { assert(0); }
static Instr emu_dp_orr(CpuState *cpu, Instr rn, Instr op2) { return rn | op2; }
static Instr emu_dp_mov(CpuState *cpu, Instr rn, Instr op2) { return op2; }

static EmuDpFn opcodefn[] = {
    [DP_AND] = emu_dp_and, [DP_EOR] = emu_dp_eor, [DP_SUB] = emu_dp_sub,
    [DP_RSB] = emu_dp_rsb, [DP_ADD] = emu_dp_add, [DP_TST] = emu_dp_tst,
    [DP_TEQ] = emu_dp_teq, [DP_CMP] = emu_dp_cmp, [DP_ORR] = emu_dp_orr,
    [DP_MOV] = emu_dp_mov};

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