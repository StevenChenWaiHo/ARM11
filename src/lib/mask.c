#include "mask.h"
#include "core.h"

Instr cond_mask(Instr i) { return (i & 0xf0000000) >> 28; }
Instr type_mask(Instr i) { return (i & 0xc000000) >> 26; }
Instr type_mul_mask(Instr i) { return (i & 0x3c00000) >> 22; }
Instr type_mul2_mask(Instr i) { return (i & 0xf0) >> 4; }
Instr dp_opcode_mask(Instr i) { return (i & 0x1e00000) >> 21; }
Instr dp_i_mask(Instr i) { return (i & 0x2000000) >> 25; }
Instr set_dp_i(Instr i, bool b) { return (i & ~0x2000000) | (b << 25); }
Instr dp_s_mask(Instr i) { return (i & 0x100000) >> 20; }
Instr set_dp_s(Instr i, bool b) { return (i & ~0x100000) | (b << 20); }
Instr dp_rn_mask(Instr i) { return (i & 0xf0000) >> 16; }
Instr dp_rd_mask(Instr i) { return (i & 0xf000) >> 12; }
Instr dp_operand2_mask(Instr i) { return (i & 0xfff) >> 0; }
Instr dp_operand2_rotate_mask(Instr i) { return (i & 0xf00) >> 8; }
Instr dp_operand2_imm_mask(Instr i) { return (i & 0xff) >> 0; }
Instr dp_operand2_rm_mask(Instr i) { return (i & 0xf) >> 0; }
Instr dp_operand2_shift_type_mask(Instr i) { return (i & 0x60) >> 5; }
Instr dp_operand2_shift_ind_mask(Instr i) { return (i & 0x10) >> 4; }
Instr set_dp_operand2_shift_ind(Instr i, bool b) { return (i & ~0x10) | (b << 4); }
Instr dp_operand2_shift_const_mask(Instr i) { return (i & 0x3ff80) >> 7; }
Instr dp_operand2_shift_rs_mask(Instr i) { return (i & 0x700) >> 8; }
Instr dp_bit_31_mask(Instr i) { return (i & 0x80000000) >> 31; }
Instr set_dp_bit_31(Instr i, bool b) { return (i & ~0x80000000) | (b << 31); }
Instr sdt_mode_mask(Instr i) { return (i & 0x3000000) >> 24; }
Instr sdt_i_mask(Instr i) { return (i & 0x2000000) >> 25; }
Instr set_sdt_i(Instr i, bool b) { return (i & ~0x2000000) | (b << 25); }
Instr sdt_p_mask(Instr i) { return (i & 0x1000000) >> 24; }
Instr set_sdt_p(Instr i, bool b) { return (i & ~0x1000000) | (b << 24); }
Instr sdt_u_mask(Instr i) { return (i & 0x800000) >> 23; }
Instr set_sdt_u(Instr i, bool b) { return (i & ~0x800000) | (b << 23); }
Instr sdt_l_mask(Instr i) { return (i & 0x100000) >> 20; }
Instr set_sdt_l(Instr i, bool b) { return (i & ~0x100000) | (b << 20); }
Instr sdt_rn_mask(Instr i) { return (i & 0xf0000) >> 16; }
Instr sdt_rd_mask(Instr i) { return (i & 0xf000) >> 12; }
Instr sdt_offset_mask(Instr i) { return (i & 0xfff) >> 0; }
Instr std_noshift_mask(Instr i) { return (i & 0xff0) >> 4; }
Instr sdt_shift_imm_mask(Instr i) { return (i & 0xf80) >> 7; }
Instr sdt_shift_mask(Instr i) { return (i & 0x60) >> 5; }
Instr sdt_rm_mask(Instr i) { return (i & 0xf) >> 0; }
Instr mul_a_mask(Instr i) { return (i & 0x200000) >> 21; }
Instr set_mul_a(Instr i, bool b) { return (i & ~0x200000) | (b << 21); }
Instr mul_s_mask(Instr i) { return (i & 0x100000) >> 20; }
Instr set_mul_s(Instr i, bool b) { return (i & ~0x100000) | (b << 20); }
Instr mul_rd_mask(Instr i) { return (i & 0xf0000) >> 16; }
Instr mul_rn_mask(Instr i) { return (i & 0xf000) >> 12; }
Instr mul_rs_mask(Instr i) { return (i & 0xf00) >> 8; }
Instr mul_rm_mask(Instr i) { return (i & 0xf) >> 0; }
Instr br_offset_mask(Instr i) { return (i & 0xffffff) >> 0; }
Instr cpsr_n_mask(Instr i) { return (i & 0x80000000) >> 31; }
Instr set_cpsr_n(Instr i, bool b) { return (i & ~0x80000000) | (b << 31); }
Instr cpsr_z_mask(Instr i) { return (i & 0x40000000) >> 30; }
Instr set_cpsr_z(Instr i, bool b) { return (i & ~0x40000000) | (b << 30); }
Instr cpsr_c_mask(Instr i) { return (i & 0x20000000) >> 29; }
Instr set_cpsr_c(Instr i, bool b) { return (i & ~0x20000000) | (b << 29); }
Instr cpsr_v_mask(Instr i) { return (i & 0x10000000) >> 28; }
Instr set_cpsr_v(Instr i, bool b) { return (i & ~0x10000000) | (b << 28); }
Instr sdt_word_1_mask(Instr i) { return (i & 0xffffff00) >> 8; }
Instr sdt_word_2_mask(Instr i) { return (i & 0xffff0000) >> 16; }
Instr sdt_word_3_mask(Instr i) { return (i & 0xff000000) >> 24; }
Instr sdt_word_4_mask(Instr i) { return (i & 0xff) >> 0; }
Instr sdt_word_5_mask(Instr i) { return (i & 0xffff) >> 0; }
Instr sdt_word_6_mask(Instr i) { return (i & 0xffffff) >> 0; }
