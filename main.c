#include <stdio.h>
#include <stdlib.h>

int comparisonCost = 0;
int rotationCost = 0;

typedef struct TreeNode {
    int key;
    struct TreeNode* parent;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct SplayTree {
    TreeNode* root;
    int comparisonCost;
    int rotationCost;
} SplayTree;

TreeNode* createTreeNode(int key) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->key = key;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

SplayTree* createSplayTree() {
    SplayTree* tree = (SplayTree*)malloc(sizeof(SplayTree));
    tree->root = NULL;
    tree->comparisonCost = 0;
    tree->rotationCost = 0;
    return tree;
}

// Zig rotation
void zig(SplayTree* tree, TreeNode* x) {
    rotationCost++;  // One zig rotation costs 1 time unit
    TreeNode* p = x->parent;
    if (p->left == x) {
        TreeNode* B = x->right;
        x->parent = NULL;
        x->right = p;
        p->parent = x;
        p->left = B;
        if (B) B->parent = p;
    } else {
        TreeNode* B = x->left;
        x->parent = NULL;
        x->left = p;
        p->parent = x;
        p->right = B;
        if (B) B->parent = p;
    }
    tree->root = x;
}

// Zig-Zig rotation
void zig_zig(SplayTree* tree, TreeNode* x) {
    rotationCost += 2;  // One zig-zig rotation costs 2 time units
    TreeNode* p = x->parent;
    TreeNode* g = p->parent;
    if (p->left == x) {
        TreeNode* B = x->right;
        TreeNode* C = p->right;
        x->parent = g->parent;
        x->right = p;
        p->parent = x;
        p->left = B;
        p->right = g;
        g->parent = p;
        g->left = C;
        if (x->parent) {
            if (x->parent->left == g) x->parent->left = x;
            else x->parent->right = x;
        }
        if (B) B->parent = p;
        if (C) C->parent = g;
    } else {
        TreeNode* B = p->left;
        TreeNode* C = x->left;
        x->parent = g->parent;
        x->left = p;
        p->parent = x;
        p->left = g;
        p->right = C;
        g->parent = p;
        g->right = B;
        if (x->parent) {
            if (x->parent->left == g) x->parent->left = x;
            else x->parent->right = x;
        }
        if (B) B->parent = g;
        if (C) C->parent = p;
    }
    tree->root = x;
}

// Zig-Zag rotation
void zig_zag(SplayTree* tree, TreeNode* x) {
    rotationCost += 2;  // One zig-zag rotation costs 2 time units
    TreeNode* p = x->parent;
    TreeNode* g = p->parent;
    if (p->right == x) {
        TreeNode* B = x->left;
        TreeNode* C = x->right;
        x->parent = g->parent;
        x->left = p;
        x->right = g;
        p->parent = x;
        p->right = B;
        g->parent = x;
        g->left = C;
        if (x->parent) {
            if (x->parent->left == g) x->parent->left = x;
            else x->parent->right = x;
        }
        if (B) B->parent = p;
        if (C) C->parent = g;
    } else {
        TreeNode* B = x->left;
        TreeNode* C = x->right;
        x->parent = g->parent;
        x->left = g;
        x->right = p;
        p->parent = x;
        p->left = C;
        g->parent = x;
        g->right = B;
        if (x->parent) {
            if (x->parent->left == g) x->parent->left = x;
            else x->parent->right = x;
        }
        if (B) B->parent = g;
        if (C) C->parent = p;
    }
    tree->root = x;
}

// Splay operation
void splay(SplayTree* tree, TreeNode* x) {
    while (x->parent != NULL) {
        TreeNode* p = x->parent;
        TreeNode* g = p->parent;
        if (g == NULL) {
            printf("Performing zig on node %d\n", x->key);
            zig(tree, x);
        } else if ((g->left == p && p->left == x) || (g->right == p && p->right == x)) {
            printf("Performing zig-zig on node %d\n", x->key);
            zig_zig(tree, x);
        } else {
            printf("Performing zig-zag on node %d\n", x->key);
            zig_zag(tree, x);
        }
    }
}


// Insert a key
void insert(SplayTree* tree, int key) {
    printf("Inserting key: %d\n", key);
    if (tree->root == NULL) {
        tree->root = createTreeNode(key);
        return;
    }
    TreeNode* curr = tree->root;
    while (curr != NULL) {
        comparisonCost++;  // Count comparison every time we decide to go left or right
        if (key < curr->key) {
            if (curr->left == NULL) {
                TreeNode* newNode = createTreeNode(key);
                curr->left = newNode;
                newNode->parent = curr;
                splay(tree, newNode);
                return;
            }
            curr = curr->left;
        } else if (key > curr->key) {
            if (curr->right == NULL) {
                TreeNode* newNode = createTreeNode(key);
                curr->right = newNode;
                newNode->parent = curr;
                splay(tree, newNode);
                return;
            }
            curr = curr->right;
        } else {
            splay(tree, curr);
            return;
        }
    }
}

// Preorder traversal
void preorderTraversal(TreeNode* node) {
    if (node == NULL) return;
    printf("%d ", node->key);
    preorderTraversal(node->left);
    preorderTraversal(node->right);
}


int main() {
    SplayTree* tree = createSplayTree();

    insert(tree, 7);
    insert(tree, 8);
    insert(tree, 4);
    insert(tree, 6);
    insert(tree, 2);
    insert(tree, 5);
    insert(tree, 3);
    insert(tree, 9);
    insert(tree, 3);
    insert(tree, 5);
    insert(tree, 5);

    printf("Preorder Traversal: ");
    preorderTraversal(tree->root);
    printf("\n");

    // Print total cost
    int totalCost = comparisonCost + rotationCost;
    printf("Total Cost: %d\n", totalCost);

    return 0;
}
