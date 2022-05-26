def maskgen(name, start, l):
    bits = "1" * l + "0" * start
    shift = start 
    h = hex(eval("0b" + bits))
    #return f"const Instr {name.upper()}_MASK = {h};"
    name += "_mask"
    func = f"Instr {name:20}(Instr i) {{ return (i & {h:10})>> {shift:3}; }}"
    return func


if __name__ == "__main__":
    for i in [
        ("cond", 28, 4),
        ("type", 26, 2),
        ("dp_opcode", 21, 4),
        ("dp_i", 25, 1),
        ("dp_s", 20, 1),
        ("dp_rn", 16, 4),
        ("dp_rd", 12, 4),
        ("dp_operand2", 0, 12),
    ]:
        print(maskgen(*i))
