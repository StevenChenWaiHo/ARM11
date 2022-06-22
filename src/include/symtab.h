#ifndef AEMU_SYMTAB_H
#define AEMU_SYMTAB_H

#include <stdint.h>

#include "str.h"

// map of string as key to int as value

// Internal
typedef struct Node {
  Str key;
  size_t value;
  struct Node *left;
  struct Node *right;
  size_t height;
} SymTabEntry;

typedef struct {
  SymTabEntry *root;
} SymTab;

size_t height(SymTabEntry *node);
SymTab sym_tab_new();
// If found, returns true, and writes to val
bool sym_tab_get(SymTab *st, Str key, size_t *val);
// Returns true if key was added, false if key already existed
bool sym_tab_insert(SymTab *st, Str key, size_t val);
void sym_tab_free(SymTab st);
void sym_tab_foreach(SymTab *st, void (*f)(SymTabEntry *));

#endif
