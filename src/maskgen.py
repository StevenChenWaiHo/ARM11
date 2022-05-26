mask_c = """#include "mask.h"\n#include "core.h"\n\n"""
mask_h = """#ifndef AEMU_MASK_H
#define AEMU_MASK_H

#include "core.h"\n\n"""


def maskgen(name, start, l):
    global mask_h
    global mask_c
    bits = "1" * l + "0" * start
    shift = start
    h = hex(eval("0b" + bits))
    # return f"const Instr {name.upper()}_MASK = {h};"
    name += "_mask"
    mask_c += f"Instr {name}(Instr i) {{ return (i & {h}) >> {shift}; }}\n"
    mask_h += f"Instr {name}(Instr);\n"


if __name__ == "__main__":
    for i in [
        ("cond", 28, 4),
        ("type", 26, 2),
        ("dp_opcode", 21, 4),
        ("type_mul", 4, 4),
        ("dp_i", 25, 1),
        ("dp_s", 20, 1),
        ("dp_rn", 16, 4),
        ("dp_rd", 12, 4),
        ("dp_operand2", 0, 12),
        ("sdt_i", 25, 1),
        ("sdt_p", 24, 1),
        ("sdt_u", 23, 1),
        ("sdt_rn", 16, 4),
        ("sdt_rd", 12, 4),
        ("sdt_offset", 0, 12),
        ("sdt_l", 20, 1),
        ("mul_a", 21, 1),
        ("mul_s", 20, 1),
        ("mul_rd", 16, 4),
        ("mul_rn", 12, 4),
        ("mul_rs", 8, 4),
        ("mul_rm", 0, 4),
        ("dp_branch", 0, 24),
    ]:
        maskgen(*i)
    mask_h += "#endif\n"
    open("lib/mask.c", "w").write(mask_c)
    open("include/mask.h", "w").write(mask_h)
