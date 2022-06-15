#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strmap.h"

//test insert to avl tree, remove when done

static void preOrder(TreeNode *root) {
  if (root != NULL) {
    printf("%s: %d\n", root->key.ptr, root->value);
    preOrder(root->left);
    preOrder(root->right);
  }
}

int main() {
  TreeNode *root = NULL;
  Tree tree = {.root = root};
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

  Str str_a = {.ptr = "a", .len = strlen("a")};
  Str str_ab = {.ptr = "ab", .len = strlen("ab")};
  Str str_ac = {.ptr = "ac", .len = strlen("ac")};
  Str str_dd = {.ptr = "dd", .len = strlen("dd")};
  Str str_eee = {.ptr = "eee", .len = strlen("eee")};
  Str str_f = {.ptr = "f", .len = strlen("f")};
 
  tree_insert(&tree, str_a, 1);
  tree_insert(&tree, str_ab, 3);
  tree_insert(&tree, str_ac, 15);
  tree_insert(&tree, str_dd, 220);
  tree_insert(&tree, str_eee, 220);
  tree_insert(&tree, str_f, 40);

 
  /* tree should be
          dd:220
           /  \
        ab:3  eee:220
        /  \     \
      a:1 ac:15  f:40
  */
 
  printf("AVL tree in preorder:\n");
  preOrder(tree.root);
  printf("%ld\n", tree.root->key.len);
  printf("AVL tree of got:\n");

  scanf("%s", input);
  Str str = {.ptr = input, .len = strlen(input)};
  got = tree_get(&tree, str);
  preOrder(got);
  if (got != NULL) {
    printf("%ld\n", got->key.len);
  }

  //memory leak exists
 
  return 0;
}