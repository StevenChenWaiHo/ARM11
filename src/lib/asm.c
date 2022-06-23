#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "asm.h"
#include "bit_asm.h"
#include "cond.h"
#include "core.h"
#include "dis.h"

#define LOWEST_8_BIT_MASK 0xFF
#define DP_SHIFT_CONST_MAX 0x1F
#define ROTATE_START_BIT 8
#define ROTATE_LIMIT 16

#define SHIFT_CONST_MAX 0x1F

static const char *instrname[] = {
    [INSTR_ADD] = "add", [INSTR_AND] = "and", [INSTR_B] = "b",
    [INSTR_CMP] = "cmp", [INSTR_EOR] = "eor", [INSTR_LDR] = "ldr",
    [INSTR_MLA] = "mla", [INSTR_MOV] = "mov", [INSTR_MUL] = "mul",
    [INSTR_ORR] = "orr", [INSTR_RSB] = "rsb", [INSTR_STR] = "str",
    [INSTR_SUB] = "sub", [INSTR_TEQ] = "teq", [INSTR_TST] = "tst",
    [INSTR_LSL] = "lsl",
};

static Cond asm_parse_cond(Assembler *a, Str cname, Token *t) {
  if (str_eq(cname, "eq"))
    return COND_EQ;
  else if (str_eq(cname, "ne"))
    return COND_NE;
  else if (str_eq(cname, "ge"))
    return COND_GE;
  else if (str_eq(cname, "lt"))
    return COND_LT;
  else if (str_eq(cname, "gt"))
    return COND_GT;
  else if (str_eq(cname, "le"))
    return COND_LE;
  else if (str_eq(cname, ""))
    return COND_AL;
  else
    asm_err(a, t, "Expected cond, got `%.*s`", (int)cname.len, cname.ptr);
}

ShiftKind asm_parse_shift_kind(Assembler *a, Token t) {
  if (str_eq(t.source, "lsl"))
    return SHIFT_LSL;
  else if (str_eq(t.source, "lsr"))
    return SHIFT_LSR;
  else if (str_eq(t.source, "asr"))
    return SHIFT_ASR;
  else
    asm_err(a, &t, "Expected shift, got `%.*s`", (int)t.source.len,
            t.source.ptr);
}

InstrKind asm_parse_instr_name(Assembler *a, Token *t) {
  if (str_starts_with(t->source, "add"))
    return INSTR_ADD;
  else if (str_starts_with(t->source, "and"))
    return INSTR_AND;
  else if (str_starts_with(t->source, "b"))
    return INSTR_B;
  else if (str_starts_with(t->source, "cmp"))
    return INSTR_CMP;
  else if (str_starts_with(t->source, "eor"))
    return INSTR_EOR;
  else if (str_starts_with(t->source, "ldr"))
    return INSTR_LDR;
  else if (str_starts_with(t->source, "mla"))
    return INSTR_MLA;
  else if (str_starts_with(t->source, "mov"))
    return INSTR_MOV;
  else if (str_starts_with(t->source, "mul"))
    return INSTR_MUL;
  else if (str_starts_with(t->source, "orr"))
    return INSTR_ORR;
  else if (str_starts_with(t->source, "rsb"))
    return INSTR_RSB;
  else if (str_starts_with(t->source, "str"))
    return INSTR_STR;
  else if (str_starts_with(t->source, "sub"))
    return INSTR_SUB;
  else if (str_starts_with(t->source, "teq"))
    return INSTR_TEQ;
  else if (str_starts_with(t->source, "tst"))
    return INSTR_TST;
  else if (str_starts_with(t->source, "lsl"))
    return INSTR_LSL;
  else
    asm_err(a, t, "Expected instruction, but got `%.*s`", (int)t->source.len,
            t->source.ptr);
}

Reg asm_expect_reg(Assembler *a) {
  return asm_parse_reg_name(a, asm_expect(a, TOKEN_IDENT));
}

Reg asm_parse_reg_name(Assembler *a, Token t) {
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
    asm_err(a, &t, "Expected register, got `%.*s`", (int)t.source.len,
            t.source.ptr);
}

static Instr rotate_instr(Instr n) { return (n << 2) | (n >> (32 - 2)); }

bool is_valid_imm(Instr imm) {
  for (int i = 0; i < ROTATE_LIMIT; i++) {
    // Valid for value that only uses lowest 8 bits
    if ((imm & ~LOWEST_8_BIT_MASK) == 0) {
      return true;
    }
    // Valid for value that only uses 8 bits (without rotating)
    int lowest_bit = ffs(imm) - 1;
    if ((imm & ~(LOWEST_8_BIT_MASK << lowest_bit)) == 0 &&
        lowest_bit % 2 == 0) {
      return true;
    }
    imm = rotate_instr(imm);
  }
  return false;
}

// Return a encoded unsigned immediate
Instr imm_encode(Assembler *a, Token t, Instr n) {
  if (!is_valid_imm(n)) {
    asm_err(a, &t, "`%.*s` out of range for immediate", (int)t.source.len,
            t.source.ptr);
  }
  if (n <= LOWEST_8_BIT_MASK) {
    return n;
  }
  for (int shift = 1; shift < ROTATE_LIMIT; shift++) {
    n = rotate_instr(n);
    if ((n & ~LOWEST_8_BIT_MASK) == 0)
      return shift << ROTATE_START_BIT | n;
  }
  asm_err(a, &t, "`%.*s` Invalid immediate", (int)t.source.len, t.source.ptr);
}

// Parser for numbers
Instr asm_parse_number(Assembler *a, Token t, bool *neg) {
  assert(t.kind == TOKEN_EQ_NUM || t.kind == TOKEN_HASH_NUM);
  Str s = t.source;
  *neg = false;
  Instr n;
  if (str_starts_with(s, "=0x") || str_starts_with(s, "#0x")) {
    s = str_trim_start(s, 3); // trim =0x
    if (!str_parse_hex(s, &n))
      asm_err(a, &t, "Invalid number %.*s", (int)t.source.len, t.source.ptr);
  } else if (str_starts_with(s, "#-0x")) {
    s = str_trim_start(s, 4);
    if (!str_parse_hex(s, &n))
      asm_err(a, &t, "Invalid number %.*s", (int)t.source.len, t.source.ptr);
    *neg = true;
  } else if (str_starts_with(s, "#") || str_starts_with(s, "=")) {
    s.ptr += 1;
    s.len -= 1;

    if (!str_parse_dec(s, &n))
      asm_err(a, &t, "Invalid number %.*s", (int)t.source.len, t.source.ptr);
  } else {
    assert(0); // Token and str not match
  }
  return n;
}

// Return unsigned immediate up to 12 bits
Instr asm_parse_imm(Assembler *a, Token t) {
  bool neg;
  Instr n = asm_parse_number(a, t, &neg);
  if (neg)
    asm_err(a, &t, "Unexpected negitive `%.*s`", (int)t.source.len,
            t.source.ptr);
  return imm_encode(a, t, n);
}

// Return a signed immediate and change the neg flag
Instr asm_parse_signed_imm(Assembler *a, Token t, bool *neg) {
  Instr n = asm_parse_number(a, t, neg);
  return imm_encode(a, t, n);
}

// Basically checks {, <shift>} in spec
// Output: operand2 instruction, use_reg is set if shift by register
Instr asm_parse_shift_reg(Assembler *a, Reg rm, bool support_use_by_reg) {
  if (!asm_match(a, TOKEN_COMMA, NULL)) {
    return rm;
  }
  ShiftKind shift_type = asm_parse_shift_kind(a, asm_expect(a, TOKEN_IDENT));
  Token rs;
  // Shift by Register
  if (asm_match(a, TOKEN_IDENT, &rs)) {
    if (!support_use_by_reg) {
      asm_err(a, &rs, "Shift by register not supported in LDR instructions");
    }
    return bit_asm_op2_shift_reg(rm, shift_type, asm_parse_reg_name(a, rs));
  } else {
    // Shift by Integer Constant
    Token imm = asm_expect(a, TOKEN_HASH_NUM);
    Instr imm_instr = asm_parse_shift_imm(a, imm);
    return bit_asm_op2_shift_imm(rm, shift_type, imm_instr);
  }
}

Instr asm_parse_shift_imm(Assembler *a, Token t) {
  Instr n = asm_parse_imm(a, t);
  if (n > DP_SHIFT_CONST_MAX)
    asm_err(a, &t, "Const too large for a shift const: %d (max is %d)", n,
            DP_SHIFT_CONST_MAX);
  return n;
}

InstrCommon asm_parse_instr_common(Assembler *a, Token *t) {
  assert(t->kind == TOKEN_IDENT);
  InstrCommon ic;
  ic.kind = asm_parse_instr_name(a, t);
  size_t kindlen = strlen(instrname[ic.kind]);
  ic.cond = asm_parse_cond(a, str_trim_start(t->source, kindlen), t);
  size_t condlen = strlen(condname[ic.cond]);
  assert(condlen + kindlen == t->source.len);

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
    [INSTR_LSL] = asm_dp,
    // Not DP
    [INSTR_B] = asm_br,
    [INSTR_MLA] = asm_mul,
    [INSTR_MUL] = asm_mul,
    [INSTR_STR] = asm_sdt,
    [INSTR_LDR] = asm_sdt,
};

void asm_reset(Assembler *a) {
  a->lexer = lexer_new(a->lexer.source, a->lexer.filename);
  a->current = lexer_next(&a->lexer);
}

void asm_write_word(Assembler *a, Instr i) {
  size_t written = fwrite(&i, sizeof(Instr), 1, a->out);
  assert(written == 1); // TODO: Handle better.
}

void asm_instr(Assembler *a, Token *t, Instr ino) {
  InstrCommon c = asm_parse_instr_common(a, t);
  Instr i = asm_fn[c.kind](a, c, ino);
  i |= c.cond << 28;
  asm_write_word(a, i);
}

#ifdef AEMU_TRACE
static void st_dbg(TreeNode *node) {
  printf("   \"%.*s\"  -> %lu\n", (int)node->key.len, node->key.ptr,
         node->value);
}
#endif

size_t asm_pass1(Assembler *a) {
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
      sym_tab_insert(&a->symtab, str_trim_end(t.source, 1), n_instr);
      // TODO: Add to symbol table here.
      asm_expect(a, TOKEN_NEWLINE);
      break;
    case TOKEN_EOF:
      goto done;
    case TOKEN_NEWLINE:
      break;
    default:
      asm_err(a, &t, "Expected line instruction name, got %s",
              token_kind_name(t.kind));
    }
  }
done:
  asm_reset(a);
  return n_instr;
}

void asm_free(Assembler a) { sym_tab_free(a.symtab); }

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

  Instr ino = 0;
  for (;;) {
    Token t = asm_advance(&a);
    switch (t.kind) {
    case TOKEN_IDENT:
      asm_instr(&a, &t, ino);
      ino++;
      break;
    case TOKEN_LABEL:
      asm_expect(&a, TOKEN_NEWLINE);
      break;
    case TOKEN_EOF:
      goto done;
    case TOKEN_NEWLINE:
      break;
    default:
      asm_err(&a, &t, "Expected identifier or label, but got `%.*s` (%s)",
              (int)t.source.len, t.source.ptr, token_kind_name(t.kind));
    }
  }
done:
  assert(ino == a.n_instrs);
  for (size_t i = 0; i < a.n_consts; i++)
    asm_write_word(&a, a.consts[i]);

  asm_free(a);
}

noreturn void asm_err(Assembler *a, Token *loc, char *fmt, ...) {
  // TODO: Take token by value, here and everywhere.
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
#ifdef FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION
  exit(0);
#else
  exit(1);
#endif
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
