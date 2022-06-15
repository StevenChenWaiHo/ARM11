#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "cond.h"
#include "core.h"

static Cond cond_parse(Str instr) {
  if (str_ends_with(instr, "eq"))
    return COND_EQ;
  else if (str_ends_with(instr, "ne"))
    return COND_NE;
  else if (str_ends_with(instr, "ge"))
    return COND_GE;
  else if (str_ends_with(instr, "lt"))
    return COND_LT;
  else if (str_ends_with(instr, "gt"))
    return COND_GT;
  else if (str_ends_with(instr, "le"))
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

InstrKind parse_instr_name(Str instr) {
  if (str_eq(instr, "add"))
    return INSTR_ADD;
  else if (str_eq(instr, "and"))
    return INSTR_AND;
  else if (str_eq(instr, "b"))
    return INSTR_B;
  else if (str_eq(instr, "cmp"))
    return INSTR_CMP;
  else if (str_eq(instr, "eor"))
    return INSTR_EOR;
  else if (str_eq(instr, "ldr"))
    return INSTR_LDR;
  else if (str_eq(instr, "mla"))
    return INSTR_MLA;
  else if (str_eq(instr, "mov"))
    return INSTR_MOV;
  else if (str_eq(instr, "mul"))
    return INSTR_MUL;
  else if (str_eq(instr, "orr"))
    return INSTR_ORR;
  else if (str_eq(instr, "rsb"))
    return INSTR_RSB;
  else if (str_eq(instr, "str"))
    return INSTR_STR;
  else if (str_eq(instr, "sub"))
    return INSTR_SUB;
  else if (str_eq(instr, "teq"))
    return INSTR_TEQ;
  else if (str_eq(instr, "tst"))
    return INSTR_TST;
  else
    assert(0); // TODO: Handle
}
Reg parse_reg_name(Token t) {
  assert(t.kind == TOKEN_IDENT);
  Str regname = t.source;
  if (str_eq(regname, "r0"))
    return REG_0;
  else if (str_eq(regname, "r1"))
    return REG_1;
  else if (str_eq(regname, "r2"))
    return REG_2;
  else if (str_eq(regname, "r3"))
    return REG_3;
  else if (str_eq(regname, "r4"))
    return REG_4;
  else if (str_eq(regname, "r5"))
    return REG_5;
  else if (str_eq(regname, "r6"))
    return REG_6;
  else if (str_eq(regname, "r7"))
    return REG_7;
  else if (str_eq(regname, "r8"))
    return REG_8;
  else if (str_eq(regname, "r9"))
    return REG_9;
  else if (str_eq(regname, "r10"))
    return REG_10;
  else if (str_eq(regname, "r11"))
    return REG_11;
  else if (str_eq(regname, "r12"))
    return REG_12;
  else
    assert(0); // TODO: Nice error
}

Instr asm_parse_number(Assembler *a, Token t) {
  assert(t.kind == TOKEN_EQ_NUM || t.kind == TOKEN_HASH_NUM);
  Str s = t.source;
  if (str_starts_with(s, "=0x") || str_starts_with(s, "#0x")) {
    s.ptr += 3;
    s.len -= 3;
    Instr n;
    if (!str_parse_hex(s, &n))
      asm_err(a, &t, "Invalid number %.*s", (int)t.source.len, t.source.ptr);
    return n;
  } else if (str_starts_with(s, "#")) {
    s.ptr += 1;
    s.len -= 1;
    Instr n;
    if (!str_parse_dec(s, &n))
      asm_err(a, &t, "Invalid number %.*s", (int)t.source.len, t.source.ptr);
    return n;
  }
  assert(0);
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

InstrCommon instr_common_parse(Str instr) {
  InstrCommon ic;
  ic.cond = cond_parse(instr);
  instr.len -= strlen(cond_name(ic.cond));
  ic.kind = parse_instr_name(instr);
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

static void asm_reset(Assembler *a) {
  a->lexer = lexer_new(a->lexer.source, a->lexer.filename);
  a->current = lexer_next(&a->lexer);
}

static void asm_instr(Assembler *a, Token *t, Instr ino) {
  InstrCommon c = instr_common_parse(t->source);
  Instr i = asm_fn[c.kind](a, c, ino);
  i |= c.cond << 28;
  DBG;
  size_t written = fwrite(&i, sizeof(Instr), 1, a->out);
  fflush(a->out);       // Temp hack so during abort we get some output.
  assert(written == 1); // TODO: Handle better.
}

#ifdef AEMU_TRACE
static void st_dbg(SymTabEntry *ste) {
  printf("   \"%.*s\"  -> %lu\n", (int)ste->name.len, ste->name.ptr,
         ste->offset);
}
#endif

static size_t asm_pass1(Assembler *a) {
  asm_reset(a);
  a->symtab = sym_tab_new();
  size_t n_instr = 0;
  Token t;
  for (;;) {
    t = asm_advance(a);
    switch (t.kind) {
    case TOKEN_IDENT:
      n_instr++;
      do
        t = asm_advance(a);
      while (t.kind != TOKEN_NEWLINE && t.kind != TOKEN_EOF);
      if (t.kind == TOKEN_EOF)
        goto done;
      break;
    case TOKEN_LABEL:
      sym_tab_insert(&a->symtab, t.source, n_instr);
      // TODO: Add to symbol table here.
      asm_expect(a, TOKEN_NEWLINE);
      break;
    case TOKEN_EOF:
      goto done;
    default:
      asm_err(a, &t, "Expected line instruction name, got %s",
              token_kind_name(t.kind));
    }
  }
done:
  asm_reset(a);
  return n_instr;
}

void assemble(char *src, char *filename, FILE *out) {
  Assembler a;
  a.lexer = lexer_new(src, filename);
  a.out = out;
  a.n_instrs = asm_pass1(&a);
  a.consts = NULL;
  a.n_consts = 0;

#ifdef AEMU_TRACE
  Token t;
  do {
    t = asm_advance(&a);
    int tkind = t.kind;
    printf("%ld:%ld %20s `%.*s`\n", t.line + 1, t.column,
           token_kind_name(tkind), (int)t.source.len, t.source.ptr);
  } while (t.kind != TOKEN_EOF);

  printf("\n{\n");
  sym_tab_foreach(&a.symtab, st_dbg);
  printf("}\n");
#endif
  asm_reset(&a);

  for (Instr ino = 0;; ino++) {
    Token t = asm_advance(&a);
    switch (t.kind) {
    case TOKEN_IDENT:
      asm_instr(&a, &t, ino);
      break;
    case TOKEN_EOF:
      return; // TODO: Cleanup?
    default:
      asm_err(&a, &t, "Expected identifier, but got `%.*s` (%s)",
              (int)t.source.len, t.source.ptr, token_kind_name(t.kind));
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
            token_kind_name(t.kind), (int)t.source.len, t.source.ptr);
  a->current = lexer_next(&a->lexer);
  return t;
}
bool asm_match(Assembler *a, TokenKind kind, Token *out) {
  if (asm_peak(a, kind)) {
    if (out)
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

Instr asm_add_const(Assembler *a, Instr value) {
  // TODO: use ptr,len,cap. Not ptr,len
  a->n_consts++;
  a->consts = reallocarray(a->consts, a->n_consts, sizeof(Instr));
  a->consts[a->n_consts - 1] = value;
  return a->n_consts - 1;
}
