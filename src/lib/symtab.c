#include <stdlib.h>

#include "symtab.h"


Tree sym_tab_new() {
  Tree st;
  TreeNode *root = malloc(sizeof(TreeNode));
  Str key;
  key.ptr = NULL;
  root->left = root->right = NULL;
  key.len = 0;
  root->height = root->value = 0;
  root->key = key;
  st.root = root;
  return st;
}
// If found, returns true, and writes to val
bool sym_tab_get(Tree *st, Str key, size_t *val) {
  Tree newtree = tree_get(st, key);
  if (newtree.root == NULL) {
    return false;    
  } else {
    *val = newtree.root->value;
    return true;
  }
}
// Returns true if key was added, false if key already existed
bool sym_tab_insert(Tree *st, Str key, size_t val) {
  size_t x;

  if (sym_tab_get(st, key, &x)) {
    return false;
  }
  tree_insert(st, key, val);
  return true;
}
void sym_tab_free(Tree st) { tree_free(&st); }

static void sym_tab_foreach_node (TreeNode *node, void (*f)(TreeNode *)) {
  if (node != NULL) {
  sym_tab_foreach_node(node->left, f);
  sym_tab_foreach_node(node->right, f);
  f(node);
  }
}

void sym_tab_foreach(Tree *st, void (*f)(TreeNode *)) {
  sym_tab_foreach_node(st->root, f);
}