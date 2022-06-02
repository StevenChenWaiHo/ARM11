#ifndef AEMU_CORE_H
#define AEMU_CORE_H

#include <stdint.h>

#define MEMORY_SIZE 65536

typedef uint32_t Instr;

enum Reg { REG_PC = 15, REG_CPSR = 16 };

#endif
