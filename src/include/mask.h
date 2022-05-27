#ifndef AEMU_MASK_H
#define AEMU_MASK_H

#include "core.h"

Instr cond_mask(Instr);
Instr type_mask(Instr);
Instr type_mul_mask(Instr);
Instr type_mul2_mask(Instr);
Instr dp_opcode_mask(Instr);
Instr dp_i_mask(Instr);
Instr dp_s_mask(Instr);
Instr dp_rn_mask(Instr);
Instr dp_rd_mask(Instr);
Instr dp_operand2_mask(Instr);
Instr dp_operand2_rotate_mask(Instr);
Instr dp_operand2_imm_mask(Instr);
Instr sdt_mode_mask(Instr);
Instr sdt_u_mask(Instr);
Instr sdt_l_mask(Instr);
Instr sdt_rn_mask(Instr);
Instr sdt_rd_mask(Instr);
Instr sdt_offset_mask(Instr);
Instr std_noshift_mask(Instr);
Instr sdt_shift_imm_mask(Instr);
Instr sdt_shift_mask(Instr);
Instr sdt_rm_mask(Instr);
Instr mul_a_mask(Instr);
Instr mul_s_mask(Instr);
Instr mul_rd_mask(Instr);
Instr mul_rn_mask(Instr);
Instr mul_rs_mask(Instr);
Instr mul_rm_mask(Instr);
Instr br_offset_mask(Instr);
#endif
