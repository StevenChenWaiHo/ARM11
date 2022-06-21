#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"

// use AVL tree to store keys for binary search and automatic balancing when
// inserting new nodes

static size_t max(size_t a, size_t b) { return (a > b) ? a : b; }

size_t height(SymTabEntry *node) {
  if (node == NULL) {
    return 0;
  }
  return 1 + max(height(node->left), height(node->right));
}

static SymTabEntry *node_new(Str key, size_t value) {
  SymTabEntry *node = (SymTabEntry *)malloc(sizeof(SymTabEntry));

  node->key = key;
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  node->height = 0;

  return node;
}

static SymTabEntry *rotate_left(SymTabEntry *node) {
  SymTabEntry *rightnode = node->right;

  node->right = rightnode->left;
  rightnode->left = node;

  node->height = height(node);
  rightnode->height = height(rightnode);

  return rightnode;
}

static SymTabEntry *rotate_right(SymTabEntry *node) {
  SymTabEntry *leftnode = node->left;

  node->left = leftnode->right;
  leftnode->right = node;

  node->height = height(node);
  leftnode->height = height(leftnode);

  return leftnode;
}

SymTab sym_tab_new() {
  SymTab st;
  /*
  SymTabEntry *root = malloc(sizeof(SymTabEntry));
  Str key;
  key.ptr = NULL;
  root->left = root->right = NULL;
  key.len = 0;
  root->height = root->value = 0;
  root->key = key;
  st.root = root;
  */
  st.root = NULL;
  return st;
}

static SymTabEntry *node_get(SymTabEntry *node, Str key) {

  if (node == NULL) {
    // printf("Cannot get value of %s: %s does not exist in the map\n", key.ptr,
    // key.ptr);
    return NULL;
  }

  int cmp = str_cmp(key, node->key);
  if (cmp == 0) {
    return node;
  } else if (cmp < 0) {
    return node_get(node->left, key);
  } else {
    return node_get(node->right, key);
  }
}

// If found, returns true, and writes to val
bool sym_tab_get(SymTab *st, Str key, size_t *val) {
  SymTabEntry *node = node_get(st->root, key);
  if (node) {
    *val = node->value;
  }
  return node;
}

static SymTabEntry *node_insert(SymTabEntry *node, Str key, size_t value) {
  if (node == NULL) {
    return (node_new(key, value));
  }

  int cmp = str_cmp(key, node->key);
  if (cmp < 0) {
    node->left = node_insert(node->left, key, value);
  } else if (cmp > 0) {
    node->right = node_insert(node->right, key, value);
  } else { // no equal keys in AVL tree
    return node;
  }

  node->height = height(node);
  int balance = 0;
  balance = height(node->left) - height(node->right);

  // 4 cases of unbalance
  if (balance > 1 && str_cmp(key, node->left->key) < 0) {
    return rotate_right(node);
  }
  if (balance < -1 && str_cmp(key, node->right->key) > 0) {
    return rotate_left(node);
  }
  if (balance > 1 && str_cmp(key, node->left->key) > 0) {
    node->left = rotate_left(node->left);
    return rotate_right(node);
  }
  if (balance < -1 && str_cmp(key, node->right->key) < 0) {
    node->right = rotate_right(node->right);
    return rotate_left(node);
  }
  return node;
}

// Returns true if key was added, false if key already existed
bool sym_tab_insert(SymTab *st, Str key, size_t val) {
  size_t x;

  if (sym_tab_get(st, key, &x)) {
    return false;
  }
  st->root = node_insert(st->root, key, val);
  return true;
}

static void node_free(SymTabEntry *node) {
  if (node == NULL)
    return;
  node_free(node->left);
  node_free(node->right);
  free(node);
}

void sym_tab_free(SymTab st) { node_free(st.root); }

static void sym_tab_foreach_node(SymTabEntry *node, void (*f)(SymTabEntry *)) {
  if (node != NULL) {
    sym_tab_foreach_node(node->left, f);
    sym_tab_foreach_node(node->right, f);
    f(node);
  }
}

void sym_tab_foreach(SymTab *st, void (*f)(SymTabEntry *)) {
  sym_tab_foreach_node(st->root, f);
}