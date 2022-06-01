#include "core.h"
#include "emu.h"
#include "shift.h"

// used when dp operand2 / sdt offset is a register

typedef InstrCarry (*EmuShiftFn)(Instr, Instr);

static InstrCarry emu_shift_lsl(Instr rm, Instr amount) {
  InstrCarry ins;
  ins.i = (amount >= 32) ? 0 : rm << amount;
  ins.carry = (amount == 0) ? 0 : (rm >> (32 - amount)) & 0x1;
  return ins;
}

static InstrCarry emu_shift_lsr(Instr rm, Instr amount) {
  InstrCarry ins;
  ins.i = (amount >= 32) ? 0 : rm >> amount;
  ins.carry = (amount == 0) ? 0 : (rm << (32 - amount)) & 0x80000000;
  return ins;
}

static InstrCarry emu_shift_asr(Instr rm, Instr amount) {
  InstrCarry ins;
  ins.i =
      (rm >> amount) | ((rm & 0x80000000) ? ~((1L << (32 - amount)) - 1L) : 0L);
  ins.carry = (amount == 0) ? 0 : (rm << (32 - amount)) & 0x80000000;
  return ins;
}

static InstrCarry emu_shift_ror(Instr rm, Instr amount) {
  InstrCarry ins;
  ins.i = (rm << (32 - amount)) | (rm >> amount);
  ins.carry = (amount == 0) ? 0 : (rm << (32 - amount)) & 0x80000000;
  return ins;
}

static EmuShiftFn opcodefn[] = {[0] = emu_shift_lsl,
                                [1] = emu_shift_lsr,
                                [2] = emu_shift_asr,
                                [3] = emu_shift_ror};

InstrCarry emu_shift(CpuState *cpu, Instr offset) {
  Instr rm = dp_operand2_rm_mask(offset);
  Instr ind = dp_operand2_shift_ind_mask(offset);
  Instr shift_type = dp_operand2_shift_type_mask(offset);
  Instr rm_value = cpu->regs[rm];
  Instr amount;
  if (!ind) {
    amount = dp_operand2_shift_const_mask(offset);
  } else {
    Instr rs = dp_operand2_shift_rs_mask(offset);
    Instr rs_content = cpu->regs[rs];
    amount = rs_content & 0xff;
  }
  return opcodefn[shift_type](rm_value, amount);
}