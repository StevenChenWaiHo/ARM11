#ifndef AEMU_MASK_H
#define AEMU_MASK_H

#include "core.h"

Instr cond_mask(Instr);
Instr type_mask(Instr);
Instr dp_opcode_mask(Instr);
Instr dp_i_mask(Instr);
Instr dp_s_mask(Instr);
Instr dp_rn_mask(Instr);
Instr dp_rd_mask(Instr);
Instr dp_operand2_mask(Instr);
Instr type_mul_mask(Instr);
Instr mul_a_mask(Instr);
Instr mul_s_mask(Instr);
Instr mul_rd_mask(Instr);
Instr mul_rn_mask(Instr);
Instr mul_rs_mask(Instr);
Instr mul_rm_mask(Instr);
#endif
