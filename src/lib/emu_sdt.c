#include <assert.h>
#include <byteswap.h>
#include <stdio.h>

#include "core.h"
#include "emu.h"
#include "mask.h"
#include "shift.h"

static Instr get_word(CpuState *cpu, Instr start, Instr old_rd) {
  if (start >= 65536 * 4) {
    printf("Error: Out of bounds memory access at address 0x%08x\n", start);
    return old_rd;
  }
  Instr this_word = cpu->mem[start / 4];
  Instr next_word = cpu->mem[start / 4 + 1];
  int remainder = start % 4;
  switch (remainder) {
  case 0:
    return this_word;
  case 1:
    return sdt_word_1_mask(this_word) | next_word << 24;
  case 2:
    return sdt_word_2_mask(this_word) | next_word << 16;
  case 3:
    return sdt_word_3_mask(this_word) | next_word << 8;
  default:
    assert(0); // Unreachable
  }
}

static void set_word(CpuState *cpu, Instr value, Instr addr) {
  Instr this_word = cpu->mem[addr / 4];
  Instr next_word = cpu->mem[addr / 4 + 1];
  int remainder = addr % 4;
  switch (remainder) {
  case 0:
    cpu->mem[addr / 4] = value;
    break;
  case 1:
    cpu->mem[addr / 4] = sdt_word_4_mask(this_word) | sdt_word_6_mask(value)
                                                          << 8;
    cpu->mem[addr / 4 + 1] = sdt_word_1_mask(next_word) << 8 | value >> 24;
    break;
  case 2:
    cpu->mem[addr / 4] = sdt_word_5_mask(this_word) | sdt_word_5_mask(value)
                                                          << 16;
    cpu->mem[addr / 4 + 1] = sdt_word_2_mask(next_word) << 16 | value >> 16;
    break;
  case 3:
    cpu->mem[addr / 4] = sdt_word_6_mask(this_word) | sdt_word_4_mask(value)
                                                          << 24;
    cpu->mem[addr / 4 + 1] = sdt_word_3_mask(next_word) << 24 | value >> 8;
    break;
  }
}

void emu_sdt(CpuState *cpu, Instr instr) {
  Instr i = sdt_i_mask(instr);   // Is Immediate
  Instr p = sdt_p_mask(instr);   // pre post indexing
  Instr u = sdt_u_mask(instr);   // Up/Down
  Instr l = sdt_l_mask(instr);   // load/store
  Instr rd = sdt_rd_mask(instr); // Src / Dest
  Instr rn = sdt_rn_mask(instr); // Base
  Instr offset = sdt_offset_mask(instr);
  Instr rd_content = cpu->regs[rd];

  if (i) {
    offset = emu_shift(cpu, offset).i;
  }

  uint32_t addr = cpu->regs[rn];

  if (p) { // pre indexing
    addr += u ? offset : -offset;
    cpu->regs[rd] = addr;
  } else { // post indexing
    cpu->regs[rd] = addr;
    Instr new_addr = addr;
    new_addr += u ? offset : -offset;
    cpu->regs[rn] = new_addr;
  }

  if (rn == REG_PC)
    addr += 8;

  if (l) // load
    cpu->regs[rd] = get_word(cpu, addr, rd_content);
  else { // store
    cpu->regs[rd] = rd_content;
    set_word(cpu, rd_content, addr);
  }
}
