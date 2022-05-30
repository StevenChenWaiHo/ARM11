#include <assert.h>
#include <byteswap.h>

#include "core.h"
#include "emu.h"
#include "mask.h"

void emu_sdt(CpuState *cpu, Instr instr) {
  Instr i = sdt_i_mask(instr); // Is Immediate
  Instr p = sdt_p_mask(instr);
  Instr u = sdt_p_mask(instr); // Up/Down
  Instr l = sdt_l_mask(instr);
  Instr rd = sdt_rd_mask(instr); // Base
  Instr rn = sdt_rn_mask(instr); // Source/dest
  Instr offset = sdt_offset_mask(instr);

  assert(!i); // TODO

  uint32_t *mem = cpu->mem;

  uint32_t addr = cpu->regs[rd];
  addr += u ? offset : -offset;
  // TODO: Endian
  if (p)
    cpu->regs[rd] = addr;
  if (l)
    cpu->regs[rn] = bswap_32(mem[addr]);
  else
    mem[addr] = bswap_32(cpu->regs[rd]);
}