#ifndef AEMU_STRMAP_H
#define AEMU_STRMAP_H

#include <stddef.h>

#include "str.h"

//map of string as key to int as value

typedef struct Node {
	Str key;
	size_t value;
	struct Node *left;
	struct Node *right;
	size_t height;
} TreeNode;

typedef struct {
	TreeNode *root;
} Tree;

Tree *tree_insert(Tree *tree, Str key, size_t value);
//Node *tree_delete(Node *node, Str key);
Tree tree_get(Tree *tree, Str key);
void tree_free(Tree *tree);

#endif
