#include <stdlib.h>

#include "symtab.h"

#define DEFAULT_CAP 4

static size_t grow_cap(size_t cap) { return cap ? cap * 2 : DEFAULT_CAP; }

SymTab sym_tab_new() {
  SymTab st;
  st.ptr = st.len = st.cap = 0;
  return st;
}
// If found, returns true, and writes to val
bool sym_tab_get(SymTab *st, Str key, size_t *val) {
  for (size_t i = 0; i < st->len; i++)
    if (str_eqs(key, st->ptr[i].name)) {
      *val = st->ptr[i].offset;
      return true;
    }
  return false;
}
// Returns true if key was added, false if key already existed
bool sym_tab_insert(SymTab *st, Str key, size_t val) {
  size_t x;

  if (sym_tab_get(st, key, &x))
    return false;

  if (st->cap == st->len) {
    st->cap = grow_cap(st->cap);
    st->ptr = reallocarray(st->ptr, st->cap, sizeof(SymTabEntry));
  }
  SymTabEntry ste = {key, val};
  st->ptr[st->len] = ste;
  st->len++;
  return true;
}
void sym_tab_free(SymTab st) { free(st.ptr); }