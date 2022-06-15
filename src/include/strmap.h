#ifndef AEMU_STRMAP_H
#define AEMU_STRMAP_H

#include <stddef.h>

#include "str.h"

//map of string as key to int as value using AVL tree

typedef struct Node {
	Str key;
	int value;
	struct Node *left;
	struct Node *right;
	int height;
} TreeNode;

typedef struct {
	TreeNode *root;
} Tree;

void tree_insert(Tree *tree, Str key, int value);
//Node *tree_delete(Node *node, Str key);
TreeNode *tree_get(Tree *tree, Str key);
void tree_free(Tree *tree);

#endif