#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "asm.h"
#include "cond.h"
#include "core.h"

// TODO: Str (ptr+len) struct
static bool ends_with(const char *s, size_t s_len, const char *suffix) {
  size_t suffixlen = strlen(suffix);
  if (s_len < suffixlen)
    return false;
  return strncmp(s + s_len - suffixlen, suffix, suffixlen) == 0;
}

static bool streq(const char *s, size_t s_len, const char *other) {
  size_t otherlen = strlen(other);
  if (s_len != otherlen)
    return false;
  return strncmp(s, other, s_len) == 0;
}

static Cond cond_parse(const char *iname, size_t inamelen) {
  if (ends_with(iname, inamelen, "eq"))
    return COND_EQ;
  else if (ends_with(iname, inamelen, "ne"))
    return COND_NE;
  else if (ends_with(iname, inamelen, "ge"))
    return COND_GE;
  else if (ends_with(iname, inamelen, "lt"))
    return COND_LT;
  else if (ends_with(iname, inamelen, "gt"))
    return COND_GT;
  else if (ends_with(iname, inamelen, "le"))
    return COND_LE;
  else
    return COND_AL;
}

// TODO: This probably exists in dis
static char *cond_name(Cond c) {
  switch (c) {
  case COND_EQ:
    return "eq";
  case COND_NE:
    return "ne";
  case COND_GE:
    return "ge";
  case COND_LT:
    return "lt";
  case COND_GT:
    return "gt";
  case COND_LE:
    return "le";
  case COND_AL:
    return "";
  default:
    assert(0);
  }
}

InstrKind parse_instr_name(const char *i, size_t len) {
  if (streq(i, len, "add"))
    return INSTR_ADD;
  else if (streq(i, len, "and"))
    return INSTR_AND;
  else if (streq(i, len, "b"))
    return INSTR_B;
  else if (streq(i, len, "cmp"))
    return INSTR_CMP;
  else if (streq(i, len, "eor"))
    return INSTR_EOR;
  else if (streq(i, len, "ldr"))
    return INSTR_LDR;
  else if (streq(i, len, "mla"))
    return INSTR_MLA;
  else if (streq(i, len, "mov"))
    return INSTR_MOV;
  else if (streq(i, len, "mul"))
    return INSTR_MUL;
  else if (streq(i, len, "orr"))
    return INSTR_ORR;
  else if (streq(i, len, "rsb"))
    return INSTR_RSB;
  else if (streq(i, len, "str"))
    return INSTR_STR;
  else if (streq(i, len, "sub"))
    return INSTR_SUB;
  else if (streq(i, len, "teq"))
    return INSTR_TEQ;
  else if (streq(i, len, "tst"))
    return INSTR_TST;
  else
    assert(0); // TODO: Handle
}

const char *instr_kind_name(InstrKind ik) {
  switch (ik) {
  case INSTR_ADD:
    return "INSTR_ADD";
  case INSTR_AND:
    return "INSTR_AND";
  case INSTR_B:
    return "INSTR_B";
  case INSTR_CMP:
    return "INSTR_CMP";
  case INSTR_EOR:
    return "INSTR_EOR";
  case INSTR_LDR:
    return "INSTR_LDR";
  case INSTR_MLA:
    return "INSTR_MLA";
  case INSTR_MOV:
    return "INSTR_MOV";
  case INSTR_MUL:
    return "INSTR_MUL";
  case INSTR_ORR:
    return "INSTR_ORR";
  case INSTR_RSB:
    return "INSTR_RSB";
  case INSTR_STR:
    return "INSTR_STR";
  case INSTR_SUB:
    return "INSTR_SUB";
  case INSTR_TEQ:
    return "INSTR_TEQ";
  case INSTR_TST:
    return "INSTR_TST";
  default:
    assert(0);
  }
}

InstrCommon instr_common_parse(const char *iname, size_t inamelen) {
  InstrCommon ic;
  ic.cond = cond_parse(iname, inamelen);
  ic.kind = parse_instr_name(iname, inamelen - strlen(cond_name(ic.cond)));
  return ic;
}
