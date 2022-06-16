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

Str str_new(const char *s, size_t l) {
  Str str = {s, l};
  return str;
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
