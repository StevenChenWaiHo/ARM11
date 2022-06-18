#include <string.h>

#include "str.h"

bool str_ends_with(Str s, char *ending) {
  size_t l_ending = strlen(ending);
  if (l_ending > s.len)
    return false;
  return strncmp(s.ptr + s.len - l_ending, ending, l_ending) == 0;
}

bool str_starts_with(Str s, char *start) {
  size_t l_start = strlen(start);
  if (l_start > s.len)
    return false;
  return strncmp(s.ptr, start, l_start) == 0;
}

bool str_eq(Str s, char *other) {
  if (strlen(other) != s.len)
    return false;
  return strncmp(s.ptr, other, s.len) == 0;
}

bool str_eqs(Str s1, Str s2) {
  if (s1.len != s2.len)
    return false;
  return memcmp(s1.ptr, s2.ptr, s1.len) == 0;
}

Str str_new(const char *s, size_t l) {
  Str str = {s, l};
  return str;
}
Str str_lit(const char *s) { return str_new(s, strlen(s)); }

static size_t min(size_t a, size_t b) { return (a < b) ? a : b; }

// return < 0 if str1 < str2, > 0 if str2 < str1, = 0 if str1 = str2
int str_cmp(Str str1, Str str2) {
  size_t len = min(str1.len, str2.len);
  int cmp = strncmp(str1.ptr, str2.ptr, len);
  if (cmp == 0 && str1.len != str2.len) {
    if (str1.len < str2.len) {
      return -1;
    } else {
      return 1;
    }
  } else {
    return cmp;
  }
}

Str str_trim_end(Str s, size_t n) {
  assert(n <= s.len);
  s.len -= n;
  return s;
}
Str str_trim_start(Str s, size_t n) {
  assert(n <= s.len);
  s.len -= n;
  s.ptr += n;
  return s;
}

// TODO: Handle overflow
bool str_parse_hex(Str s, Instr *out) {
  Instr n = 0;
  for (size_t i = 0; i < s.len; i++) {
    char c = s.ptr[i];
    Instr d;
    if ('0' <= c && c <= '9')
      d = c - '0';
    else if ('a' <= c && c <= 'f')
      d = c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
      d = c - 'A' + 10;
    else
      return false;
    n = 16 * n + d;
  }
  *out = n;
  return true;
}

bool str_parse_dec(Str s, Instr *out) {
  Instr n = 0;
  for (size_t i = 0; i < s.len; i++) {
    char c = s.ptr[i];
    Instr d;
    if ('0' <= c && c <= '9')
      d = c - '0';
    else
      return false;
    n = 10 * n + d;
  }
  *out = n;
  return true;
}
