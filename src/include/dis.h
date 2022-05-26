#ifndef AEMU_DIS_H
#define AEMU_DIS_H

#include "core.h"

extern const char *regname[17];

void dis_br(Instr i, char *cond);
void dis_dp(Instr i, char *cond);
void dis_mul(Instr i, char *cond);
void dis_sdt(Instr i, char *cond);
void dis(int offset, Instr i);

#endif
