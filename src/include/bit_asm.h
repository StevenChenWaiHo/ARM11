#ifndef AEMU_BIT_ASM_H
#define AEMU_BIT_ASM_H

#include "core.h"
#include "dp.h"

Instr bit_asm_sdt(bool i, bool p, bool u, bool l, Reg rn, Reg rd, Instr offset);
Instr bit_asm_mul(bool a, bool s, Reg rd, Reg rn, Reg rs, Reg rm);
Instr bit_asm_dp(bool i, DpKind opcode, bool s, Reg rn, Reg rd, Instr op2);
Instr bit_asm_br(Instr offset);

#endif