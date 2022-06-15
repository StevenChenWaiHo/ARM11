#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strmap.h"

//use AVL tree to store keys for binary search and automatic balancing when inserting new nodes

static int min(int a, int b) {
  return (a < b) ? a : b;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}

int height(TreeNode *node) {
  if (node == NULL) {
    return 0;
  }
  return 1 + max(height(node->left), height(node->right));
}

//return < 0 if str1 < str2, > 0 if str2 < str1, = 0 if str1 = str2
static int str_cmp(Str str1, Str str2) {
  int len = min(str1.len, str2.len);
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

static TreeNode *node_insert(TreeNode *node, Str key, int value) {
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

Tree *tree_insert(Tree *tree, Str key, int value) {
  Tree *newtree = malloc(sizeof(Tree));
  newtree->root = node_insert(tree->root, key, value);
  return newtree;
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

Tree *tree_get(Tree *tree, Str key) {
  Tree *newtree = malloc(sizeof(Tree));
  newtree->root = node_get(tree->root, key);
  return newtree;
}
