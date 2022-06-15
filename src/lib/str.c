#include <string.h>

#include "str.h"

bool str_ends_with(Str s, char *ending) {
  size_t l_ending = strlen(ending);
  if (l_ending > s.len)
    return false;
  return strncmp(s.ptr + s.len - l_ending, ending, l_ending) == 0;
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
