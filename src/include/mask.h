#ifndef AEMU_MASK_H
#define AEMU_MASK_H

#include <stdbool.h>
#include "core.h"

Instr cond_mask(Instr);
Instr type_mask(Instr);
Instr type_mul_mask(Instr);
Instr type_mul2_mask(Instr);
Instr dp_opcode_mask(Instr);
Instr dp_i_mask(Instr);
Instr set_dp_i(Instr, bool);
Instr dp_s_mask(Instr);
Instr set_dp_s(Instr, bool);
Instr dp_rn_mask(Instr);
Instr dp_rd_mask(Instr);
Instr dp_operand2_mask(Instr);
Instr dp_operand2_rotate_mask(Instr);
Instr dp_operand2_imm_mask(Instr);
Instr dp_operand2_rm_mask(Instr);
Instr dp_operand2_shift_type_mask(Instr);
Instr dp_operand2_shift_ind_mask(Instr);
Instr set_dp_operand2_shift_ind(Instr, bool);
Instr dp_operand2_shift_const_mask(Instr);
Instr dp_operand2_shift_rs_mask(Instr);
Instr dp_bit_31_mask(Instr);
Instr set_dp_bit_31(Instr, bool);
Instr sdt_mode_mask(Instr);
Instr sdt_i_mask(Instr);
Instr set_sdt_i(Instr, bool);
Instr sdt_p_mask(Instr);
Instr set_sdt_p(Instr, bool);
Instr sdt_u_mask(Instr);
Instr set_sdt_u(Instr, bool);
Instr sdt_l_mask(Instr);
Instr set_sdt_l(Instr, bool);
Instr sdt_rn_mask(Instr);
Instr sdt_rd_mask(Instr);
Instr sdt_offset_mask(Instr);
Instr std_noshift_mask(Instr);
Instr sdt_shift_imm_mask(Instr);
Instr sdt_shift_mask(Instr);
Instr sdt_rm_mask(Instr);
Instr mul_a_mask(Instr);
Instr set_mul_a(Instr, bool);
Instr mul_s_mask(Instr);
Instr set_mul_s(Instr, bool);
Instr mul_rd_mask(Instr);
Instr mul_rn_mask(Instr);
Instr mul_rs_mask(Instr);
Instr mul_rm_mask(Instr);
Instr br_offset_mask(Instr);
Instr cpsr_n_mask(Instr);
Instr set_cpsr_n(Instr, bool);
Instr cpsr_z_mask(Instr);
Instr set_cpsr_z(Instr, bool);
Instr cpsr_c_mask(Instr);
Instr set_cpsr_c(Instr, bool);
Instr cpsr_v_mask(Instr);
Instr set_cpsr_v(Instr, bool);
Instr sdt_word_1_mask(Instr);
Instr sdt_word_2_mask(Instr);
Instr sdt_word_3_mask(Instr);
Instr sdt_word_4_mask(Instr);
Instr sdt_word_5_mask(Instr);
Instr sdt_word_6_mask(Instr);
#endif
