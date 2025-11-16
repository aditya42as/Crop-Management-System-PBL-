// bst.h
#ifndef BST_H
#define BST_H

typedef struct BSTNode {
    char key[100];
    void *data;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode *bst_insert(BSTNode *root, const char *key, void *data);
BSTNode *bst_search(BSTNode *root, const char *key);
void bst_free(BSTNode *root);

#endif
