#include "emu.h"

#define BREAKPOINT_NUMBER 50

void dbg(CpuState *cpu, int total_instr_no, int *instr_to_line_no);
bool sequence(CpuState *cpu, int *breakpoint, int bpt_ptr, bool step);
