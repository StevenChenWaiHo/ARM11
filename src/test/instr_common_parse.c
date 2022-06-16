#include <assert.h>
#include <string.h>

#include "asm.h"

#define TEST_CASE(name_raw, ikind, icond)                                      \
  do {                                                                         \
    size_t name_len = strlen(name_raw);                                        \
    char *name_to_parse = name_raw " extra for test";                          \
    InstrCommon parsed = instr_common_parse(str_new(name_to_parse, name_len)); \
    assert(parsed.kind == ikind);                                              \
    assert(parsed.cond == icond);                                              \
  } while (0)

int main() {
  TEST_CASE("add", INSTR_ADD, COND_AL);
  TEST_CASE("and", INSTR_AND, COND_AL);
  TEST_CASE("bne", INSTR_B, COND_NE);
  TEST_CASE("cmp", INSTR_CMP, COND_AL);
  TEST_CASE("eoreq", INSTR_EOR, COND_EQ);
  TEST_CASE("ldrle", INSTR_LDR, COND_LE);
  TEST_CASE("mlagt", INSTR_MLA, COND_GT);
}
