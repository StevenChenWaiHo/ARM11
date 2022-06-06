#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strmap.h"

//test insert to avl tree

int main() {
  Node_t *root = NULL;
  Node_t *got = NULL;
  char str[10];
 
  root = insert(root, "a", 1);
  root = insert(root, "ab", 3);
  root = insert(root, "ac", 15);
  root = insert(root, "dd", 220);
  root = insert(root, "eee", 220);
  root = insert(root, "f", 40);

 
  /* tree should be
           d:220
           /  \
        ab:3  e:220
        /  \     \
      a:1 ac:15  f:40
  */
 
  printf("AVL tree in preorder:\n");
  preOrder(root);
  printf("%ld\n", root->keylen);
  printf("AVL tree of got:\n");

  scanf("%s", str);
  got = get(root, str);
  preOrder(got);
  if (got != NULL) {
    printf("%ld\n", got->keylen);
  }

  //memory leak exists
 
  return 0;
}