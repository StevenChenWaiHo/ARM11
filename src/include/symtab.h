#ifndef AEMU_SYMTAB_H
#define AEMU_SYMTAB_H

#include <stdint.h>

#include "str.h"

//map of string as key to int as value

// Internal
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

size_t height(TreeNode *node);
Tree sym_tab_new();
// If found, returns true, and writes to val
bool sym_tab_get(Tree *st, Str key, size_t *val);
// Returns true if key was added, false if key already existed
bool sym_tab_insert(Tree *st, Str key, size_t val);
void sym_tab_free(Tree st);
void sym_tab_foreach(Tree *st, void (*f)(TreeNode *));

#endif