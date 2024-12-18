#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode {
    int key;
    int frequency;
    struct TreeNode* parent;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct SplayTree {
    TreeNode* root;
    int comparisonCost;
    int rotationCost;
} SplayTree;

typedef struct ModSplayTree {
    TreeNode* root;
    int comparisonCost;
    int rotationCost;
} ModSplayTree;

TreeNode* createTreeNode(int key) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->key = key;
    newNode->frequency = 0;//new
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

ModSplayTree* createModSplayTree() {
    ModSplayTree* tree = (ModSplayTree*)malloc(sizeof(ModSplayTree));
    tree->root = NULL;
    tree->comparisonCost = 0;
    tree->rotationCost = 0;
    return tree;
}

// Zig rotation
void zig(SplayTree* tree, TreeNode* x) {
    tree->rotationCost++;  // One zig rotation costs 1 time unit
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
    tree->rotationCost += 2;  // One zig-zig rotation costs 2 time units
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
    tree->rotationCost += 2;  // One zig-zag rotation costs 2 time units
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

// Insert a key for SplayTree
void insert(SplayTree* tree, int key) {
    printf("Inserting key: %d\n", key);
    if (tree->root == NULL) {
        tree->root = createTreeNode(key);
        return;
    }
    TreeNode* curr = tree->root;
    while (curr != NULL) {
        tree->comparisonCost++;
        //comparisonCost++;  // Count comparison every time we decide to go left or right
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

// Helper function to perform preorder traversal and compare frequencies
void preOrderTraversalForFreqComparison(TreeNode* node, TreeNode* nodeToSplay, bool* isHighestFreq) {
    if (node == NULL) {
        return;
    }

    // Compare the frequencies
    if (node != nodeToSplay && node->frequency > nodeToSplay->frequency) {
        *isHighestFreq = false;
        return;
    }

    // Continue traversing the tree in preorder
    preOrderTraversalForFreqComparison(node->left, nodeToSplay, isHighestFreq);
    preOrderTraversalForFreqComparison(node->right, nodeToSplay, isHighestFreq);
}


// Insert a key for ModSplayTree
void insertModSplayTree(ModSplayTree* tree, int key) {
    printf("Inserting key: %d\n", key);

    // Case 1: Tree is empty, insert the root node.
    if (tree->root == NULL) {
        tree->root = createTreeNode(key);
        return;
    }

    TreeNode* curr = tree->root;
    TreeNode* nodeToSplay = NULL;

    // Case 2: Traverse the tree to insert the node like a normal BST
    while (curr != NULL) {
        tree->comparisonCost++;  // Count comparisons
        if (key < curr->key) {
            if (curr->left == NULL) {
                TreeNode* newNode = createTreeNode(key);
                curr->left = newNode;
                newNode->parent = curr;
                return;  // Exit after insertion
            }
            curr = curr->left;
        } else if (key > curr->key) {
            if (curr->right == NULL) {
                TreeNode* newNode = createTreeNode(key);
                curr->right = newNode;
                newNode->parent = curr;
                return;  // Exit after insertion
            }
            curr = curr->right;
        } else { // Case 3: Key already exists, increment the frequency
            curr->frequency++;  // Increment frequency
            printf("Incrementing frequency of node %d to %d\n", curr->key, curr->frequency);
            nodeToSplay = curr;  // This node will be considered for splaying
            break;  // Exit the loop after finding the key
        }
    }

    // Case 4: Traverse the entire tree to check if this node has the highest frequency
    if (nodeToSplay != NULL) {
        bool isHighestFreq = true;

        // Pre-order traversal to compare frequencies with all nodes in the tree
        preOrderTraversalForFreqComparison(tree->root, nodeToSplay, &isHighestFreq);

        // If the current node has the highest frequency, perform splay
        if (isHighestFreq) {
            printf("Splaying node with key %d\n", nodeToSplay->key);
            splay(tree, nodeToSplay); // Perform splay operation to bring this node to root
        }
    }
}


// Preorder traversal for SplayTree
void preorderTraversal(TreeNode* node) {
    if (node == NULL) return;
    printf("%d ", node->key);
    preorderTraversal(node->left);
    preorderTraversal(node->right);
}

// Preorder traversal for ModSplayTree
void preorderTraversalModSplayTree(TreeNode* node) {
    if (node == NULL) return;
    printf("(%d,%d) ", node->key, node->frequency);
    preorderTraversalModSplayTree(node->left);
    preorderTraversalModSplayTree(node->right);
}


int main() {
    SplayTree* splayTree = createSplayTree();
    ModSplayTree* modSplayTree = createModSplayTree();

    insert(splayTree, 7);
    insert(splayTree, 8);
    insert(splayTree, 4);
    insert(splayTree, 6);
    insert(splayTree, 2);
    insert(splayTree, 5);
    insert(splayTree, 3);
    insert(splayTree, 9);
    insert(splayTree, 3);
    insert(splayTree, 5);
    insert(splayTree, 5);

    insertModSplayTree(modSplayTree, 7);
    insertModSplayTree(modSplayTree, 8);
    insertModSplayTree(modSplayTree, 4);
    insertModSplayTree(modSplayTree, 6);
    insertModSplayTree(modSplayTree, 2);
    insertModSplayTree(modSplayTree, 5);
    insertModSplayTree(modSplayTree, 3);
    insertModSplayTree(modSplayTree, 9);
    insertModSplayTree(modSplayTree, 3);
    insertModSplayTree(modSplayTree, 5);
    insertModSplayTree(modSplayTree, 5);

    printf("Splay tree\n");
    printf("Tree (pre-order traversal): ");
    preorderTraversal(splayTree->root);
    printf("\n");

    int totalCostSplayTree = splayTree->comparisonCost + splayTree->rotationCost;
    printf("Cost: %d\n", totalCostSplayTree);

    printf("\n");

    printf("Mod-Splay tree\n");
    printf("Tree (pre-order traversal): ");
    preorderTraversalModSplayTree(modSplayTree->root);
    printf("\n");

    int totalCostModSplayTree = modSplayTree->comparisonCost + modSplayTree->rotationCost;
    printf("Cost: %d\n", totalCostModSplayTree);


    return 0;
}
