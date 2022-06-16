#include <assert.h>
#include <stdbool.h>

#include "bit_asm.h"

#include "asm.h"
#include "core.h"
#include "dp.h"

Instr bit_width(Instr i) { return i == 0 ? 0 : 32 - __builtin_clz(i); }

static void check_reg(Reg r) { assert(bit_width(r) <= 4); }

// TODO: Check values are in bounds
Instr bit_asm_sdt(bool i, bool p, bool u, bool l, Reg rn, Reg rd,
                  Instr offset) {
  check_reg(rn);
  check_reg(rd);
  assert(bit_width(offset) <= 12);
  return 1 << 26 | i << 25 | p << 24 | u << 23 | l << 20 | rn << 16 | rd << 12 |
         offset;
}

Instr bit_asm_mul(bool a, bool s, Reg rd, Reg rn, Reg rs, Reg rm) {
  check_reg(rd);
  check_reg(rn);
  check_reg(rs);
  check_reg(rm);
  return a << 21 | s << 20 | rd << 16 | rn << 12 | rs << 8 | 9 << 4 | rm;
}

Instr bit_asm_dp(bool i, Instr opcode, bool s, Reg rn, Reg rd, Instr op2) {
  check_reg(rn);
  check_reg(rd);
  assert(bit_width(opcode) <= 4);
  assert(bit_width(op2) <= 12);
  return i << 25 | opcode << 21 | s << 20 | rn << 16 | rd << 12 | op2;
}

Instr bit_asm_br(Instr offset) {
  assert(bit_width(offset) <= 24);
  return offset | 5 << 25;
}

Instr bit_asm_op2_shift_reg(Reg rm, ShiftKind shift_type, Reg rs) {
  check_reg(rm);
  check_reg(rs);
  assert(bit_width(shift_type) <= 4);
  return rs << 8 | shift_type << 5 | 1 << 4 | rm;
}
Instr bit_asm_op2_shift_imm(Reg rm, ShiftKind shift_type, Instr imm) {
  check_reg(rm);
  assert(bit_width(shift_type) <= 4);
  assert(bit_width(imm) <= 5);
  return imm << 7 | shift_type << 5 | rm;
}