#ifndef AEMU_STR_H
#define AEMU_STR_H

#include <stdbool.h>
#include <stddef.h>

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

bool str_ends_with(Str s, char *ending);
bool str_eq(Str s, char *other);
Str str_new(const char *, size_t);

#endif