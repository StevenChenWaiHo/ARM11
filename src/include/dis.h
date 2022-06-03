#ifndef AEMU_DIS_H
#define AEMU_DIS_H

#include <stdio.h>

#include "core.h"

extern const char *regname[17];
extern const char *shiftname[4];

void dis_br(FILE *f, Instr i, const char *cond, int pc);
void dis_dp(FILE *f, Instr i, const char *cond);
void dis_mul(FILE *f, Instr i, const char *cond);
void dis_sdt(FILE *f, Instr i, const char *cond);
void dis(FILE *f, int offset, Instr i);

#endif
