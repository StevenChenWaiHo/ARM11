#ifndef AEMU_CONDFN_H
#define AEMU_CONDFN_H
#include "cond.h"
typedef bool (*CpuCondFn)(CpuState *);

static CpuCondFn condfns[] = {
    [COND_EQ] = emu_cond_eq, [COND_NE] = emu_cond_ne, [COND_GE] = emu_cond_ge,
    [COND_LT] = emu_cond_lt, [COND_GT] = emu_cond_gt, [COND_LE] = emu_cond_le,
    [COND_AL] = emu_cond_al,
};

#endif