#ifndef AEMU_CORE_H
#define AEMU_CORE_H

#include <stdint.h>

#define MEMORY_SIZE 65536
#define REGISTER_NUMBER 17

typedef uint32_t Instr;

typedef enum {
  REG_0,
  REG_1,
  REG_2,
  REG_3,
  REG_4,
  REG_5,
  REG_6,
  REG_7,
  REG_8,
  REG_9,
  REG_10,
  REG_11,
  REG_12,
  REG_PC = 15,
  REG_CPSR = 16
} Reg;

#define DBG printf("%s:%d\n", __FILE__, __LINE__)

#endif
