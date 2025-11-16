// bst.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

BSTNode *bst_insert(BSTNode *root, const char *key, void *data) {
    if (!root) {
        BSTNode *n = (BSTNode *)malloc(sizeof(BSTNode));
        if (!n) {
            return NULL;
        }
        strncpy(n->key, key, sizeof(n->key) - 1);
        n->key[sizeof(n->key) - 1] = '\0';
        n->data = data;
        n->left = n->right = NULL;
        return n;
    }
    int cmp = strcmp(key, root->key);
    if (cmp < 0) {
        root->left = bst_insert(root->left, key, data);
    } else if (cmp > 0) {
        root->right = bst_insert(root->right, key, data);
    } else {
        root->data = data;
    }
    return root;
}

BSTNode *bst_search(BSTNode *root, const char *key) {
    if (!root) {
        return NULL;
    }
    int cmp = strcmp(key, root->key);
    if (cmp == 0) {
        return root;
    }
    if (cmp < 0) {
        return bst_search(root->left, key);
    }
    return bst_search(root->right, key);
}

void bst_free(BSTNode *root) {
    if (!root) {
        return;
    }
    bst_free(root->left);
    bst_free(root->right);
    free(root);
}
