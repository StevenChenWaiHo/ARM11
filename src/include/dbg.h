#ifndef AEMU_DBG_H
#define AEMU_DBG_H
#include "emu.h"

#define BREAKPOINT_NUMBER 50

void dbg(uint32_t *mem, int total_instr_no, int *instr_to_line_no);

#endif