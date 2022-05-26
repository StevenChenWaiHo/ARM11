Instr cond_mask           (Instr i) { return (i & 0xf0000000)>>  28; }
Instr type_mask           (Instr i) { return (i & 0xc000000 )>>  26; }
Instr dp_opcode_mask      (Instr i) { return (i & 0x1e00000 )>>  21; }
Instr dp_i_mask           (Instr i) { return (i & 0x2000000 )>>  25; }
Instr dp_s_mask           (Instr i) { return (i & 0x100000  )>>  20; }
Instr dp_rn_mask          (Instr i) { return (i & 0xf0000   )>>  16; }
Instr dp_rd_mask          (Instr i) { return (i & 0xf000    )>>  12; }
Instr dp_operand2_mask    (Instr i) { return (i & 0xfff     )>>   0; }
