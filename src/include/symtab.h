#ifndef AEMU_SYMTAB_H
#define AEMU_SYMTAB_H

#include <stdint.h>

#include "strmap.h"


// Internal
typedef struct {
  Str name;
  size_t offset;
} SymTabEntry;

typedef struct {
  SymTabEntry *ptr;
  size_t len;
  size_t cap;
} SymTab;

SymTab sym_tab_new();
// If found, returns true, and writes to val
bool sym_tab_get(SymTab *st, Str key, size_t *val);
// Returns true if key was added, false if key already existed
bool sym_tab_insert(SymTab *st, Str key, size_t val);
void sym_tab_free(SymTab);
void sym_tab_foreach(SymTab *st, void (*f)(SymTabEntry *));

Tree n_sym_tab_new();
// If found, returns true, and writes to val
bool n_sym_tab_get(Tree *st, Str key, size_t *val);
// Returns true if key was added, false if key already existed
bool n_sym_tab_insert(Tree *st, Str key, size_t val);
void n_sym_tab_free(Tree st);
void n_sym_tab_foreach(Tree *st, void (*f)(TreeNode *));

#endif