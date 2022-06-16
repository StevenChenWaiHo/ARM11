#include <assert.h>
#include <string.h>

#include "asm.h"
#include "bit_asm.h"

#define TESTCASE(n, bool)                                                      \
  do {                                                                         \
    assert(is_valid_imm(n) == bool);                                           \
  } while (0)

int main() {
  TESTCASE(511, 0);
  TESTCASE(0x0003FC00, 1);
  TESTCASE(0x00000102, 0);
  TESTCASE(0xFF0000FF, 0);
  TESTCASE(0xC0000034, 1);
}