#include "mask.h"
#include "core.h"

Instr cond_mask(Instr i) { return (i & 0xf0000000) >> 28; }
Instr type_mask(Instr i) { return (i & 0xc000000) >> 26; }
Instr type_mul_mask(Instr i) { return (i & 0x3c00000) >> 22; }
Instr type_mul2_mask(Instr i) { return (i & 0xf0) >> 4; }
Instr dp_opcode_mask(Instr i) { return (i & 0x1e00000) >> 21; }
Instr dp_i_mask(Instr i) { return (i & 0x2000000) >> 25; }
Instr dp_s_mask(Instr i) { return (i & 0x100000) >> 20; }
Instr dp_rn_mask(Instr i) { return (i & 0xf0000) >> 16; }
Instr dp_rd_mask(Instr i) { return (i & 0xf000) >> 12; }
Instr dp_operand2_mask(Instr i) { return (i & 0xfff) >> 0; }
Instr dp_operand2_rotate_mask(Instr i) { return (i & 0xf00) >> 8; }
Instr dp_operand2_imm_mask(Instr i) { return (i & 0xff) >> 0; }
Instr sdt_mode_mask(Instr i) { return (i & 0x3000000) >> 24; }
Instr sdt_u_mask(Instr i) { return (i & 0x800000) >> 23; }
Instr sdt_l_mask(Instr i) { return (i & 0x100000) >> 20; }
Instr sdt_rn_mask(Instr i) { return (i & 0xf0000) >> 16; }
Instr sdt_rd_mask(Instr i) { return (i & 0xf000) >> 12; }
Instr sdt_offset_mask(Instr i) { return (i & 0xfff) >> 0; }
Instr std_noshift_mask(Instr i) { return (i & 0xff0) >> 4; }
Instr sdt_shift_imm_mask(Instr i) { return (i & 0xf80) >> 7; }
Instr sdt_shift_mask(Instr i) { return (i & 0x60) >> 5; }
Instr sdt_rm_mask(Instr i) { return (i & 0xf) >> 0; }
Instr mul_a_mask(Instr i) { return (i & 0x200000) >> 21; }
Instr mul_s_mask(Instr i) { return (i & 0x100000) >> 20; }
Instr mul_rd_mask(Instr i) { return (i & 0xf0000) >> 16; }
Instr mul_rn_mask(Instr i) { return (i & 0xf000) >> 12; }
Instr mul_rs_mask(Instr i) { return (i & 0xf00) >> 8; }
Instr mul_rm_mask(Instr i) { return (i & 0xf) >> 0; }
Instr br_offset_mask(Instr i) { return (i & 0xffffff) >> 0; }
