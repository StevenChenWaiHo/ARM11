#ifndef AEMU_STR_H
#define AEMU_STR_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#include "core.h"

// This is basicly a rust &str, which means:
// - Not null terminated.
// - Does not represent ownership
// - Don't pass a pointer to it (unless to directly modify fields)
// - Don't modify throught the pointer (ok to change ptr and len, but not *ptr
//     or ptr[...])
typedef struct {
  const char *ptr;
  size_t len;
} Str;

// TODO: Add a test

bool str_ends_with(Str s, char *ending);
bool str_starts_with(Str s, char *start);

bool str_eq(Str s, char *other);
bool str_eqs(Str s1, Str s2);

Str str_new(const char *, size_t);
Str str_lit(const char *);

// TODO: Flesh out, no raw access to str outside str.c

// s[:-n]
Str str_trim_end(Str s, size_t n);

bool str_parse_hex(Str s, Instr *out);
bool str_parse_dec(Str s, Instr *out);

#endif
