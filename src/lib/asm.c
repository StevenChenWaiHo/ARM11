#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

static AsmFn asm_fn[] = {
    [INSTR_ADD] = asm_dp,
    [INSTR_AND] = asm_dp,
    [INSTR_CMP] = asm_dp,
    [INSTR_EOR] = asm_dp,
    [INSTR_MOV] = asm_dp,
    [INSTR_ORR] = asm_dp,
    [INSTR_RSB] = asm_dp,
    [INSTR_SUB] = asm_dp,
    [INSTR_TEQ] = asm_dp,
    [INSTR_TST] = asm_dp,
    // Not DP
    [INSTR_B] = asm_br,
    [INSTR_MLA] = asm_mul,
    [INSTR_MUL] = asm_mul,
    [INSTR_STR] = asm_sdt,
    [INSTR_LDR] = asm_sdt,
};

static void asm_instr(Assembler *a, Token *t) {
  InstrCommon c = instr_common_parse(t->source, t->len);
  Instr i = asm_fn[c.kind](a, c);
  // TODO: Add cond
  size_t written = fwrite(&i, sizeof(Instr), 1, a->out);
  assert(written == 1); // TODO: Handle better.
}

void assemble(char *src, char *filename, FILE *out) {
  Assembler a;
  a.lexer = lexer_new(src, filename);;
  a.out = out;
  a.current = lexer_next(&a.lexer);

  // Token t;
  // do {
  //   t = asm_advance(&a);
  //   int tkind = t.kind;
  //   printf("%ld:%ld %20s `%.*s`\n", t.line + 1, t.column,
  //          token_kind_name(tkind), (int)t.len, t.source);
  // } while (t.kind != TOKEN_EOF);

  for (;;) {
    Token t = asm_advance(&a);
    switch (t.kind) {
    case TOKEN_IDENT:
      asm_instr(&a, &t);
      break;
    case TOKEN_EOF:
      return; // TODO: Cleanup?
    default:
      asm_err(&a, &t, "Expected identifier, but got `%.*s` (%s)", (int)t.len,
              t.source, token_kind_name(t.kind));
    }
  }
}

noreturn void asm_err(Assembler *a, Token *loc, char *fmt, ...) {
  // TODO: Use current?
  va_list fmt_args;
  va_start(fmt_args, fmt);
  size_t sz = vsnprintf(NULL, 0, fmt, fmt_args) + 1;
  va_end(fmt_args);
  char *msg = malloc(sz); // TODO: check
  va_start(fmt_args, fmt);
  vsnprintf(msg, sz, fmt, fmt_args);
  va_end(fmt_args);

  fprintf(stderr, "%s:%ld:%ld: %s\n", a->lexer.filename, loc->line + 1,
          loc->column + 1, msg);
  exit(1);
}

Token asm_expect(Assembler *a, TokenKind kind) {
  Token t = a->current;
  if (t.kind != kind)
    asm_err(a, &t, "Expected %s, but got %s(%.*s)", token_kind_name(kind),
            token_kind_name(t.kind), (int)t.len, t.source);
  a->current = lexer_next(&a->lexer);
  return t;
}
bool asm_match(Assembler *a, TokenKind kind, Token *out) {
  if (asm_peak(a, kind)) {
    *out = a->current;
    a->current = lexer_next(&a->lexer);
    return true;
  }
  return false;
}
bool asm_peak(Assembler *a, TokenKind kind) { return a->current.kind == kind; }
Token asm_advance(Assembler *a) {
  Token t = a->current;
  a->current = lexer_next(&a->lexer);
  return t;
}