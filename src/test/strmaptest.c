#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strmap.h"

//test insert to avl tree, remove when done

/*static void preOrder(TreeNode *root) {
  if (root != NULL) {
    printf("%s: %d\n", root->key.ptr, root->value);
    preOrder(root->left);
    preOrder(root->right);
  }
}*/

static Str str_a = {.ptr = "a", .len = strlen("a")};
static Str str_ab = {.ptr = "ab", .len = strlen("ab")};
static Str str_ac = {.ptr = "ac", .len = strlen("ac")};
static Str str_dd = {.ptr = "dd", .len = strlen("dd")};
static Str str_eee = {.ptr = "eee", .len = strlen("eee")};
static Str str_f = {.ptr = "f", .len = strlen("f")};

static TreeNode *testtree_get() {

  TreeNode *node_a = malloc(sizeof(TreeNode));
  node_a->key = str_a;
  node_a->value = 1;
  node_a->left = NULL;
  node_a->right = NULL;
  node_a->height = 1;
  TreeNode *node_ac = malloc(sizeof(TreeNode));
  node_ac->key = str_ac;
  node_ac->value = 15;
  node_ac->left = NULL;
  node_ac->right = NULL;
  node_ac->height = 1;
  TreeNode *node_f = malloc(sizeof(TreeNode));
  node_f->key = str_f;
  node_f->value = 40;
  node_f->left = NULL;
  node_f->right = NULL;
  node_f->height = 1;
  TreeNode *node_ab = malloc(sizeof(TreeNode));
  node_ab->key = str_ab;
  node_ab->value = 3;
  node_ab->left = node_a;
  node_ab->right = node_ac;
  node_ab->height = 2;
  TreeNode *node_eee = malloc(sizeof(TreeNode));
  node_eee->key = str_eee;
  node_eee->value = 220;
  node_eee->left = NULL;
  node_eee->right = node_f;
  node_eee->height = 2;
  TreeNode *node_dd = malloc(sizeof(TreeNode));
  node_dd->key = str_dd;
  node_dd->value = 220;
  node_dd->left = node_ab;
  node_dd->right = node_eee;
  node_dd->height = 3;

  return node_dd;

}

int main() {
  TreeNode *root = NULL;
  //Tree tree = {.root = root};
  TreeNode *got = NULL;
  char input[10];

  /*char **keys = {"a", "ab", "ac", "dd", "eee", "f", NULL};
  int vals[] = {1, 3, 15, 220, 220, 40};
  Str strs[10];
  for (int i = 0; keys[i]; i++) {
    strs[i].ptr = keys[i];
    strs[i].len = strlen(keys[i]);
  }

  for (int i = 0; vals[i]; i++) {
    tree = tree_insert(tree, strs[i], vals[i]);
  }*/
  TreeNode *testtree = testtree_get();
 
  root = tree_insert(root, str_a, 1);
  assert(tree_get(root, str_a)->value == tree_get(testtree, str_a)->value);
  root = tree_insert(root, str_ab, 3);
  root = tree_insert(root, str_ac, 15);
  assert(tree_get(root, str_a)->value == tree_get(testtree, str_a)->value);
  assert(tree_get(root, str_ab)->value == tree_get(testtree, str_ab)->value);
  assert(tree_get(root, str_ac)->value == tree_get(testtree, str_ac)->value);
  root = tree_insert(root, str_dd, 220);
  root = tree_insert(root, str_eee, 220);
  root = tree_insert(root, str_f, 40);
  assert(tree_get(root, str_dd)->value == tree_get(testtree, str_dd)->value);
  assert(tree_get(root, str_eee)->value == tree_get(testtree, str_eee)->value);
  assert(tree_get(root, str_f)->value == tree_get(testtree, str_f)->value);


 
  /* tree should be
          dd:220
           /  \
        ab:3  eee:220
        /  \     \
      a:1 ac:15  f:40
  */

  /*
  preOrder(testtree);
  printf("AVL tree in preorder:\n");
  preOrder(root);
  printf("%ld %d\n", root->key.len, root->height);
  printf("AVL tree of got:\n");

  scanf("%s", input);
  Str str = {.ptr = input, .len = strlen(input)};
  got = tree_get(root, str);
  preOrder(got);
  if (got != NULL) {
    printf("%ld %d\n", got->key.len, got->height);
  }
  */

  //memory leak exists
 
  return 0;
}