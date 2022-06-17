#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtab.h"

//use AVL tree to store keys for binary search and automatic balancing when inserting new nodes

static size_t min(size_t a, size_t b) {
  return (a < b) ? a : b;
}

static size_t max(size_t a, size_t b) {
  return (a > b) ? a : b;
}

size_t height(TreeNode *node) {
  if (node == NULL) {
    return 0;
  }
  return 1 + max(height(node->left), height(node->right));
}

//return < 0 if str1 < str2, > 0 if str2 < str1, = 0 if str1 = str2
static int str_cmp(Str str1, Str str2) {
  size_t len = min(str1.len, str2.len);
  int cmp = strncmp(str1.ptr, str2.ptr, len);
  if (cmp == 0 && str1.len != str2.len) {
    if (str1.len < str2.len) {
      return -1;
    } else {
      return 1;
    }
  } else {
    return cmp;
  }
}

static TreeNode *node_new(Str key, size_t value) {
  TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));

  node->key = key;
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  node->height = 0;

  return node;
}

static TreeNode *leftRotate(TreeNode *node) {
  TreeNode *rightnode = node->right;

  node->right = rightnode->left;
  rightnode->left = node;

  node->height = height(node);
  rightnode->height = height(rightnode);

  return rightnode;
}

static TreeNode *rightRotate(TreeNode *node) {
  TreeNode *leftnode = node->left;

  node->left = leftnode->right;
  leftnode->right = node;

  node->height = height(node);
  leftnode->height = height(leftnode);
    
  return leftnode;
}

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

static TreeNode *node_get(TreeNode *node, Str key) {
  
  if (node == NULL) {
    //printf("Cannot get value of %s: %s does not exist in the map\n", key.ptr, key.ptr);
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
bool sym_tab_get(Tree *st, Str key, size_t *val) {
  Tree newtree;
  newtree.root = node_get(st->root, key);
  if (newtree.root == NULL) {
    return false;    
  } else {
    *val = newtree.root->value;
    return true;
  }
}

static TreeNode *node_insert(TreeNode *node, Str key, size_t value) {
  if (node == NULL) {
    return(node_new(key, value));
  }

  int cmp = str_cmp(key, node->key);
  if (cmp < 0) {
    node->left = node_insert(node->left, key, value);
  } else if (cmp > 0) {
    node->right = node_insert(node->right, key, value);
  } else {//no equal keys in AVL tree
    return node;
  }
  
  node->height = height(node);
  int balance = 0;
  balance = height(node->left) - height(node->right);

  //4 cases of unbalance
  if (balance > 1 && str_cmp(key, node->left->key) < 0) {
    return rightRotate(node);
  }
  if (balance < -1 && str_cmp(key, node->right->key) > 0) {
    return leftRotate(node);
  }
  if (balance > 1 && str_cmp(key, node->left->key) > 0) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }
  if (balance < -1 && str_cmp(key, node->right->key) < 0) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }
  return node;
}

// Returns true if key was added, false if key already existed
bool sym_tab_insert(Tree *st, Str key, size_t val) {
  size_t x;

  if (sym_tab_get(st, key, &x)) {
    return false;
  }
  st->root = node_insert(st->root, key, val);
  return true;
}

static void node_free(TreeNode *node) {
  if (node->left == NULL) {
    free(node->left);
  } else {
      node_free(node->left);
  }
  if (node->right == NULL) {
    free(node->right);
  } else {
    node_free(node->right);
  }
  free(node);
}

void sym_tab_free(Tree st) { node_free(st.root); }

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