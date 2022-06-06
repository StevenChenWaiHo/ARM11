#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strmap.h"

int max(int a, int b) {
  return (a > b) ? a :b;
}

int height(Node_t *node) {
  if (node == NULL) {
    return 0;
  }
  return 1 + max(height(node->left), height(node->right));
}

Node_t *newNode(char *key, int value) {
  Node_t *node = (Node_t *) malloc(sizeof(Node_t));

  node->key = key;
  node->keylen = strlen(key);
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  node->height = 0;

  return node;
}

Node_t *leftRotate(Node_t *node) {
  Node_t *rightNode = node->right;

  node->right = rightNode->left;
  rightNode->left = node;

  node->height = height(node);
  rightNode->height = height(rightNode);

  return rightNode;
}

Node_t *rightRotate(Node_t *node) {
  Node_t *leftNode = node->left;

  node->left = leftNode->right;
  leftNode->right = node;

  node->height = height(node);
  leftNode->height = height(leftNode);
    
  return leftNode;
}

int getBalanceFactor(Node_t *node) {
  if (node == NULL) {
    return 0;
  }
  return height(node->left) - height(node->right);
}

Node_t *insert(Node_t *node, char *key, int value) {
  if (node == NULL) {
    return(newNode(key, value));
  }

  int cmp = strcmp(key, node->key);
  if (cmp < 0) {
    node->left = insert(node->left, key, value);
  } else if (cmp > 0) {
    node->right = insert(node->right, key, value);
  } else {//no equal keys in AVL tree
    return node;
  }
  
  node->height = height(node);
  int balance = getBalanceFactor(node);

  //4 cases of unbalance
  if (balance > 1 && key < node->left->key) {
    return rightRotate(node);
  }
  if (balance < -1 && key > node->right->key) {
    return leftRotate(node);
  }
  if (balance > 1 && key > node->left->key) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
  }
  if (balance < -1 && key< node->right->key) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
  }
  return node;
}

Node_t *get(Node_t *node, char *key) {
  
  if (node == NULL) {
    printf("Cannot get value of %s: %s does not exist in the map\n", key, key);
    return NULL;
  }
  
  int cmp = strcmp(key, node->key);
  if (cmp == 0) {
    return node;
  } else if (cmp < 0) {
    return get(node->left, key);
  } else {
    return get(node->right, key);
  }
}

void preOrder(Node_t *root) {
  if (root != NULL) {
    printf("%s: %d\n", root->key, root->value);
    preOrder(root->left);
    preOrder(root->right);
  }
}