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
Instr sdt_i_mask(Instr);
Instr sdt_p_mask(Instr);
Instr sdt_u_mask(Instr);
Instr sdt_rn_mask(Instr);
Instr sdt_rd_mask(Instr);
Instr sdt_offset_mask(Instr);
Instr sdt_l_mask(Instr);
#endif
