mask_c = """#include "mask.h"\n#include "core.h"\n\n"""
mask_h = """#ifndef AEMU_MASK_H
#define AEMU_MASK_H

#include <stdbool.h>
#include "core.h"\n\n"""


def maskgen(name, start, l):
    global mask_h
    global mask_c
    bits = "1" * l + "0" * start
    bits = eval("0b" + bits)
    shift = start
    comp = 0xFFFF ^ bits
    h = hex(bits)
    # return f"const Instr {name.upper()}_MASK = {h};"
    mask_name = name + "_mask"
    mask_c += f"Instr {mask_name}(Instr i) {{ return (i & {h}) >> {shift}; }}\n"
    mask_h += f"Instr {mask_name}(Instr);\n"
    if l == 1:  # TODO: Generalizer setters.
        set_name = "set_" + name
        mask_h += f"Instr {set_name}(Instr, bool);\n"
        mask_c += f"Instr {set_name}(Instr i, bool b) {{ return (i & ~{h}) | (b << {shift}); }}\n"


if __name__ == "__main__":
    # TODO: Deduplicate and rename
    for i in [
        # Top level dispatch
        ("cond", 28, 4),
        ("type", 26, 2),
        ("type_mul", 22, 4),
        ("type_mul2", 4, 4),
        # Data processing
        ("dp_opcode", 21, 4),
        ("dp_i", 25, 1),
        ("dp_s", 20, 1),
        ("dp_rn", 16, 4),
        ("dp_rd", 12, 4),
        ("dp_operand2", 0, 12),
        ("dp_operand2_rotate", 8, 4),
        ("dp_operand2_imm", 0, 8),
        ("dp_operand2_rm", 0, 4),
        ("dp_operand2_shift_type", 5, 2),
        ("dp_operand2_shift_ind", 4, 1),
        ("dp_operand2_shift_const", 7, 11),
        ("dp_operand2_shift_rs", 8, 3),
        ("dp_bit_31", 31, 1),
        # Signle data transfer
        ("sdt_mode", 24, 2),
        ("sdt_i", 25, 1),
        ("sdt_p", 24, 1),
        ("sdt_u", 23, 1),
        ("sdt_l", 20, 1),
        ("sdt_rn", 16, 4),
        ("sdt_rd", 12, 4),
        ("sdt_offset", 0, 12),
        ("std_noshift", 4, 8),
        ("sdt_shift_imm", 7, 5),
        ("sdt_shift", 5, 2),
        ("sdt_rm", 0, 4),
        # Multiplication
        ("mul_a", 21, 1),
        ("mul_s", 20, 1),
        ("mul_rd", 16, 4),
        ("mul_rn", 12, 4),
        ("mul_rs", 8, 4),
        ("mul_rm", 0, 4),
        # Branch
        ("br_offset", 0, 24),
        # CPSR
        ("cpsr_n", 31, 1),
        ("cpsr_z", 30, 1),
        ("cpsr_c", 29, 1),
        ("cpsr_v", 28, 1),
        # emu_sdt_word
        ("sdt_word_1", 8, 24),
        ("sdt_word_2", 16, 16),
        ("sdt_word_3", 24, 8),
        ("sdt_word_4", 0, 8),
        ("sdt_word_5", 0, 16),
        ("sdt_word_6", 0, 24),
    ]:
        maskgen(*i)
    mask_h += "#endif\n"
    open("lib/mask.c", "w").write(mask_c)
    open("include/mask.h", "w").write(mask_h)
