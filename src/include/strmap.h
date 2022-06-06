#ifndef AEMU_STRMAP_H
#define AEMU_STRMAP_H

#include <stddef.h>

//map of string as key to int as value

typedef struct Node {
	char *key;
	size_t keylen;
	int value;
	struct Node *left;
	struct Node *right;
	int height;
} Node_t;

int max(int a, int b);
int height(Node_t *node);
Node_t *newNode(char *key, int value);
Node_t *leftRotate(Node_t *node);
Node_t *rightRotate(Node_t *node);
int getBalanceFactor(Node_t *node);
Node_t *insert(Node_t *node, char *key, int value);
Node_t *delete(Node_t *node, char *key);
Node_t *get(Node_t *node, char *key);
void preOrder(Node_t *root);

#endif