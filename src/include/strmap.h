#ifndef AEMU_STRMAP_H
#define AEMU_STRMAP_H

#include <stddef.h>

#include "str.h"

//map of string as key to int as value

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

TreeNode *tree_insert(TreeNode *node, Str key, int value);
//Node *tree_delete(Node *node, Str key);
TreeNode *tree_get(TreeNode *node, Str key);
//void tree_free(Tree *tree);

#endif
