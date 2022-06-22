#include <assert.h>

#include "str.h"
#include "symtab.h"

static void incnode(SymTabEntry *node) { node->value++; }

static bool node_maintain_invariant(SymTabEntry *node) {
  int inv;
  bool maintain;
  if (node->left && str_cmp(node->key, node->left->key) <= 0)
    return false;
  if (node->right && str_cmp(node->key, node->right->key) >= 0)
    return false;

  if (!node) {
    inv = 0;
    maintain = true;
  } else if (!node->left && !node->right) {
    inv = 0;
    maintain = true;
  } else if (!node->left) {
    inv = height(node->right);
    maintain = node_maintain_invariant(node->right);
  } else if (!node->right) {
    inv = height(node->left);
    maintain = node_maintain_invariant(node->left);
  } else {
    inv = height(node->left) - height(node->right);
    maintain = node_maintain_invariant(node->left) &&
               node_maintain_invariant(node->right);
  }
  return inv >= -1 && inv <= 1 && maintain;
}

static bool maintain_invariant(SymTab st) {
  return node_maintain_invariant(st.root);
}

int main() {
  SymTab st = sym_tab_new();
  assert(sym_tab_insert(&st, str_lit("l1"), 1));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l2"), 2));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l3"), 3));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l4"), 4));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l5"), 5));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l6"), 6));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l7"), 7));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l8"), 8));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l9"), 9));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l10"), 10));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l11"), 11));
  assert(maintain_invariant(st));
  assert(sym_tab_insert(&st, str_lit("l12"), 12));
  assert(maintain_invariant(st));

  assert(!sym_tab_insert(&st, str_lit("l1"), 1));
  assert(!sym_tab_insert(&st, str_lit("l2"), 2));
  assert(!sym_tab_insert(&st, str_lit("l3"), 3));
  assert(!sym_tab_insert(&st, str_lit("l4"), 4));
  assert(!sym_tab_insert(&st, str_lit("l5"), 5));
  assert(!sym_tab_insert(&st, str_lit("l6"), 6));
  assert(!sym_tab_insert(&st, str_lit("l7"), 7));
  assert(!sym_tab_insert(&st, str_lit("l8"), 8));
  assert(!sym_tab_insert(&st, str_lit("l9"), 9));
  assert(!sym_tab_insert(&st, str_lit("l10"), 10));
  assert(!sym_tab_insert(&st, str_lit("l11"), 11));
  assert(!sym_tab_insert(&st, str_lit("l12"), 12));

  size_t n;
  assert(sym_tab_get(&st, str_lit("l1"), &n));
  assert(n == 1);
  assert(!sym_tab_get(&st, str_lit("l1_"), &n));
  assert(sym_tab_get(&st, str_lit("l2"), &n));
  assert(n == 2);
  assert(!sym_tab_get(&st, str_lit("l2_"), &n));
  assert(sym_tab_get(&st, str_lit("l3"), &n));
  assert(n == 3);
  assert(!sym_tab_get(&st, str_lit("l3_"), &n));
  assert(sym_tab_get(&st, str_lit("l4"), &n));
  assert(n == 4);
  assert(!sym_tab_get(&st, str_lit("l4_"), &n));
  assert(sym_tab_get(&st, str_lit("l5"), &n));
  assert(n == 5);
  assert(!sym_tab_get(&st, str_lit("l5_"), &n));
  assert(sym_tab_get(&st, str_lit("l6"), &n));
  assert(n == 6);
  assert(!sym_tab_get(&st, str_lit("l6_"), &n));
  assert(sym_tab_get(&st, str_lit("l7"), &n));
  assert(n == 7);
  assert(!sym_tab_get(&st, str_lit("l7_"), &n));
  assert(sym_tab_get(&st, str_lit("l8"), &n));
  assert(n == 8);
  assert(!sym_tab_get(&st, str_lit("l8_"), &n));
  assert(sym_tab_get(&st, str_lit("l9"), &n));
  assert(n == 9);
  assert(!sym_tab_get(&st, str_lit("l9_"), &n));
  assert(sym_tab_get(&st, str_lit("l10"), &n));
  assert(n == 10);
  assert(!sym_tab_get(&st, str_lit("Xl10_"), &n));
  assert(sym_tab_get(&st, str_lit("l11"), &n));
  assert(n == 11);
  assert(!sym_tab_get(&st, str_lit("Xl11_"), &n));
  assert(sym_tab_get(&st, str_lit("l12"), &n));
  assert(n == 12);
  assert(!sym_tab_get(&st, str_lit("Xl12_"), &n));

  sym_tab_foreach(&st, incnode);
  assert(sym_tab_get(&st, str_lit("l1"), &n));
  assert(n == 2);
  assert(!sym_tab_get(&st, str_lit("l1_"), &n));
  assert(sym_tab_get(&st, str_lit("l2"), &n));
  assert(n == 3);
  assert(!sym_tab_get(&st, str_lit("l2_"), &n));
  assert(sym_tab_get(&st, str_lit("l3"), &n));
  assert(n == 4);
  assert(!sym_tab_get(&st, str_lit("l3_"), &n));
  assert(sym_tab_get(&st, str_lit("l4"), &n));
  assert(n == 5);
  assert(!sym_tab_get(&st, str_lit("l4_"), &n));
  assert(sym_tab_get(&st, str_lit("l5"), &n));
  assert(n == 6);
  assert(!sym_tab_get(&st, str_lit("l5_"), &n));
  assert(sym_tab_get(&st, str_lit("l6"), &n));
  assert(n == 7);
  assert(!sym_tab_get(&st, str_lit("l6_"), &n));
  assert(sym_tab_get(&st, str_lit("l7"), &n));
  assert(n == 8);
  assert(!sym_tab_get(&st, str_lit("l7_"), &n));
  assert(sym_tab_get(&st, str_lit("l8"), &n));
  assert(n == 9);
  assert(!sym_tab_get(&st, str_lit("l8_"), &n));
  assert(sym_tab_get(&st, str_lit("l9"), &n));
  assert(n == 10);
  assert(!sym_tab_get(&st, str_lit("l9_"), &n));
  assert(sym_tab_get(&st, str_lit("l10"), &n));
  assert(n == 11);
  assert(!sym_tab_get(&st, str_lit("Xl10_"), &n));
  assert(sym_tab_get(&st, str_lit("l11"), &n));
  assert(n == 12);
  assert(!sym_tab_get(&st, str_lit("Xl11_"), &n));
  assert(sym_tab_get(&st, str_lit("l12"), &n));
  assert(n == 13);
  assert(!sym_tab_get(&st, str_lit("Xl12_"), &n));

  sym_tab_free(st);
}