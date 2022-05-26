#ifndef AEMU_MASK_H
#define AEMU_MASK_H

#include "core.h"

Instr cond_mask(Instr i);
Instr type_mask(Instr i);
Instr dp_opcode_mask(Instr i);
Instr dp_i_mask(Instr i);
Instr dp_s_mask(Instr i);
Instr dp_rn_mask(Instr i);
Instr dp_rd_mask(Instr i);
Instr dp_operand2_mask(Instr i);

#endif