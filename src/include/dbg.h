#include "emu.h"

#define BREAKPOINT_NUMBER 50

void dbg(CpuState *);
bool sequence(CpuState *, int *, int, bool);