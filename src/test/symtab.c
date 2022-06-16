#include <assert.h>

#include "str.h"
#include "symtab.h"

int main() {
  SymTab st = sym_tab_new();
  assert(sym_tab_insert(&st, str_lit("l1"), 1));
  assert(sym_tab_insert(&st, str_lit("l2"), 2));
  assert(sym_tab_insert(&st, str_lit("l3"), 3));
  assert(sym_tab_insert(&st, str_lit("l4"), 4));
  assert(sym_tab_insert(&st, str_lit("l5"), 5));
  assert(sym_tab_insert(&st, str_lit("l6"), 6));
  assert(sym_tab_insert(&st, str_lit("l7"), 7));
  assert(sym_tab_insert(&st, str_lit("l8"), 8));
  assert(sym_tab_insert(&st, str_lit("l9"), 9));
  assert(sym_tab_insert(&st, str_lit("l10"), 10));
  assert(sym_tab_insert(&st, str_lit("l11"), 11));
  assert(sym_tab_insert(&st, str_lit("l12"), 12));

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


  Tree sts = n_sym_tab_new();
  assert(n_sym_tab_insert(&sts, str_lit("l1"), 1));
  assert(n_sym_tab_insert(&sts, str_lit("l2"), 2));
  assert(n_sym_tab_insert(&sts, str_lit("l3"), 3));
  assert(n_sym_tab_insert(&sts, str_lit("l4"), 4));
  assert(n_sym_tab_insert(&sts, str_lit("l5"), 5));
  assert(n_sym_tab_insert(&sts, str_lit("l6"), 6));
  assert(n_sym_tab_insert(&sts, str_lit("l7"), 7));
  assert(n_sym_tab_insert(&sts, str_lit("l8"), 8));
  assert(n_sym_tab_insert(&sts, str_lit("l9"), 9));
  assert(n_sym_tab_insert(&sts, str_lit("l10"), 10));
  assert(n_sym_tab_insert(&sts, str_lit("l11"), 11));
  assert(n_sym_tab_insert(&sts, str_lit("l12"), 12));

  assert(!n_sym_tab_insert(&sts, str_lit("l1"), 1));
  assert(!n_sym_tab_insert(&sts, str_lit("l2"), 2));
  assert(!n_sym_tab_insert(&sts, str_lit("l3"), 3));
  assert(!n_sym_tab_insert(&sts, str_lit("l4"), 4));
  assert(!n_sym_tab_insert(&sts, str_lit("l5"), 5));
  assert(!n_sym_tab_insert(&sts, str_lit("l6"), 6));
  assert(!n_sym_tab_insert(&sts, str_lit("l7"), 7));
  assert(!n_sym_tab_insert(&sts, str_lit("l8"), 8));
  assert(!n_sym_tab_insert(&sts, str_lit("l9"), 9));
  assert(!n_sym_tab_insert(&sts, str_lit("l10"), 10));
  assert(!n_sym_tab_insert(&sts, str_lit("l11"), 11));
  assert(!n_sym_tab_insert(&sts, str_lit("l12"), 12));

  //size_t n;
  assert(n_sym_tab_get(&sts, str_lit("l1"), &n));
  assert(n == 1);
  assert(!n_sym_tab_get(&sts, str_lit("l1_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l2"), &n));
  assert(n == 2);
  assert(!n_sym_tab_get(&sts, str_lit("l2_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l3"), &n));
  assert(n == 3);
  assert(!n_sym_tab_get(&sts, str_lit("l3_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l4"), &n));
  assert(n == 4);
  assert(!n_sym_tab_get(&sts, str_lit("l4_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l5"), &n));
  assert(n == 5);
  assert(!n_sym_tab_get(&sts, str_lit("l5_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l6"), &n));
  assert(n == 6);
  assert(!n_sym_tab_get(&sts, str_lit("l6_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l7"), &n));
  assert(n == 7);
  assert(!n_sym_tab_get(&sts, str_lit("l7_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l8"), &n));
  assert(n == 8);
  assert(!n_sym_tab_get(&sts, str_lit("l8_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l9"), &n));
  assert(n == 9);
  assert(!n_sym_tab_get(&sts, str_lit("l9_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l10"), &n));
  assert(n == 10);
  assert(!n_sym_tab_get(&sts, str_lit("Xl10_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l11"), &n));
  assert(n == 11);
  assert(!n_sym_tab_get(&sts, str_lit("Xl11_"), &n));
  assert(n_sym_tab_get(&sts, str_lit("l12"), &n));
  assert(n == 12);
  assert(!n_sym_tab_get(&sts, str_lit("Xl12_"), &n));
}