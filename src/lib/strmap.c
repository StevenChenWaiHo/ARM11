#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strmap.h"

//use AVL tree to store keys for binary search and automatic balancing when inserting new nodes

static int min(int a, int b) {
  return (a < b) ? a : b;
}

static int max(int a, int b) {
  return (a > b) ? a : b;
}

static int height(TreeNode *node) {
  if (node == NULL) {
    return 0;
  }
  return 1 + max(height(node->left), height(node->right));
}

static int str_cmp(Str str1, Str str2) {
  int len = min(str1.len, str2.len);
  int cmp = strncmp(str1.ptr, str2.ptr, len);
  if (cmp == 0 && str1.len != str2.len) {
    if (str1.len > str2.len) {
      return 1;
    } else {
      return -1;
    }
  } else {
    return cmp;
  }
}

static TreeNode *node_new(Str key, int value) {
  TreeNode *node = (TreeNode *) malloc(sizeof(TreeNode));

  node->key = key;
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  node->height = 0;

  return node;
}

static TreeNode *leftRotate(TreeNode *node) {
  TreeNode *rightNode = node->right;

  node->right = rightNode->left;
  rightNode->left = node;

  node->height = height(node);
  rightNode->height = height(rightNode);

  return rightNode;
}

static TreeNode *rightRotate(TreeNode *node) {
  TreeNode *leftNode = node->left;

  node->left = leftNode->right;
  leftNode->right = node;

  node->height = height(node);
  leftNode->height = height(leftNode);
    
  return leftNode;
}

TreeNode *node_insert(Tree *tree, Str key, int value) {
  TreeNode *node = tree->root;
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
  if (balance > 1 && str_cmp(key, node->left->key)) {
    return rightRotate(node);
  } else if (balance < -1 && str_cmp(key, node->right->key)) {
    return leftRotate(node);
  } else if (balance > 1 && str_cmp(key, node->left->key)) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  } else if (balance < -1 && str_cmp(key, node->right->key)) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }
  return node;
}

void tree_insert(Tree *tree, Str key, int value) {
  tree->root = node_insert(tree, key, value);
}

TreeNode *tree_get(Tree *tree, Str key) {
  TreeNode *node = tree->root;  
  if (node == NULL) {
    //printf("Cannot get value of %s: %s does not exist in the map\n", key.ptr, key.ptr);
    return NULL;
  }
  
  int cmp = str_cmp(key, node->key);
  if (cmp == 0) {
    return node;
  } else if (cmp < 0) {
    return tree_get(node->left, key);
  } else {
    return tree_get(node->right, key);
  }
}
