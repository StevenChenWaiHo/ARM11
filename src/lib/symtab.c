#include <stdlib.h>

#include "symtab.h"

#define DEFAULT_CAP 4

static size_t grow_cap(size_t cap) { return cap ? cap * 2 : DEFAULT_CAP; }

SymTab sym_tab_new() {
  SymTab st;
  st.len = st.cap = 0;
  st.ptr = NULL;
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

void sym_tab_foreach(SymTab *st, void (*f)(SymTabEntry *)) {
  for (size_t i = 0; i < st->len; i++)
    f(&st->ptr[i]);
}


Tree n_sym_tab_new() {
  Tree st;
  TreeNode *root = malloc(sizeof(TreeNode));
  Str key;
  key.ptr = NULL;
  key.len = 0;
  root->left = root->right = NULL;
  root->height = root->value = 0;
  root->key = key;
  st.root = root;
  return st;
}
// If found, returns true, and writes to val
bool n_sym_tab_get(Tree *st, Str key, size_t *val) {
  Tree newtree = tree_get(st, key);
  if (newtree.root == NULL) {
    return false;    
  } else {
    *val = newtree.root->value;
    return true;
  }
}
// Returns true if key was added, false if key already existed
bool n_sym_tab_insert(Tree *st, Str key, size_t val) {
  size_t x;

  if (n_sym_tab_get(st, key, &x)) {
    return false;
  }
  tree_insert(st, key, val);
  return true;
}
void n_sym_tab_free(Tree st) { tree_free(&st); }

static void sym_tab_foreach_node (TreeNode *node, void (*f)(TreeNode *)) {
  f(node->left);
  f(node->right);
  f(node);
}

void n_sym_tab_foreach(Tree *st, void (*f)(TreeNode *)) {
  sym_tab_foreach_node(st->root, f);
}