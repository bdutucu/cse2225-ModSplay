#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    int key;
    int frequency;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct SplayTree {
    Node* root;
    int comparisonCost;
    int rotationCost;
} SplayTree;


Node* createTreeNode(int key) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->frequency = 0;
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

void free_tree(Node* node) {
    if (node) {
        free_tree(node->left);
        free_tree(node->right);
        free(node);
    }
}

void destroy_tree(SplayTree* tree) {
    free_tree(tree->root);
    free(tree);
}

void left_rotate(SplayTree* tree, Node* x) {
    if (!x || !x->right) {
        printf("left rotate failed because node is null.");
       return; 
    }
    
    Node* y = x->right;

    x->right = y->left; // we are ensuring the left of y is empty because we are going to use there.
    
    if (y->left) y->left->parent = x;

    y->parent = x->parent;

    if (!x->parent) {
        tree->root = y; // updating root if x was the root
    } else if (x == x->parent->left) {
        //x in parentının yeni sol cocugu y.
        x->parent->left = y;
    } else {
        //x in parentının yeni sag cocugu y.
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void right_rotate(SplayTree* tree, Node* x) {
    if (!x || !x->left) return; // Ensure x and x->left are not NULL

    Node* y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;

    y->parent = x->parent;
    if (!x->parent) {
        tree->root = y; // Update root if x was the root
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->right = x;
    x->parent = y;
}

//if p(x) the parent of the root, is the tree root, rotate the edge x with p(x).
void zig(SplayTree* tree, Node* x){

if(x == x->parent->left){
    // if x is the left child, we do right rotation.
    right_rotate(tree ,x->parent); 
}

else{
    //if x is the right child, we do left rotation.
    left_rotate(tree,x->parent);
}


tree->rotationCost++;

}

void zig_zag(SplayTree* tree, Node* x){

    if(x == x->parent->right && x->parent == x->parent->parent->left){
        left_rotate(tree,x->parent);
        right_rotate(tree,x->parent);
        
    }

    else if(x== x->parent->left && x->parent == x->parent->parent->right){

        right_rotate(tree,x->parent);
        left_rotate(tree,x->parent);
        
    }
        
    tree->rotationCost+=2;

}

void zig_zig(SplayTree* tree, Node* x){

if(x == x->parent->left && x->parent == x->parent->parent->left ){
    right_rotate(tree,x->parent->parent);
    right_rotate(tree, x->parent);
}

else if(x == x->parent->right && x->parent == x->parent->parent->right ){
    left_rotate(tree,x->parent->parent);
    left_rotate(tree, x->parent);
}

 tree->rotationCost+=2;
}

void splay(SplayTree* tree, Node* x) {
    while (x->parent != NULL) {
        Node* p = x->parent;
        Node* g = p->parent;
        if (g == NULL) {
            zig(tree, x);
        } else if ((g->left == p && p->left == x) || (g->right == p && p->right == x)) {
            zig_zig(tree, x);
        } else {
            zig_zag(tree, x);
        }
    }
}


void insert(SplayTree* tree, int key) {
    if (tree->root == NULL) {
        tree->root = createTreeNode(key);
        return;
    }
    Node* curr = tree->root;
    while (curr != NULL) {

        tree->comparisonCost++;
      
        if (key < curr->key) {
            if (curr->left == NULL) {
                Node* newNode = createTreeNode(key);
                curr->left = newNode;
                newNode->parent = curr;
                splay(tree, newNode);
                return;
            }
            curr = curr->left;
        } else if (key > curr->key) {
            if (curr->right == NULL) {
                Node* newNode = createTreeNode(key);
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

int maxFreq = 0;

void insertModSplayTree(SplayTree* Modtree, int key) {
    if (Modtree->root == NULL) {
        Modtree->root = createTreeNode(key);
        return;
    }

    Node* curr = Modtree->root;
    Node* nodeToSplay = NULL;

    while (curr != NULL) {
        Modtree->comparisonCost++;  
        if (key < curr->key) {
            if (curr->left == NULL) {
                Node* newNode = createTreeNode(key);
                curr->left = newNode;
                newNode->parent = curr;
                return;  
            }
            curr = curr->left;
        } else if (key > curr->key) {
            if (curr->right == NULL) {
                Node* newNode = createTreeNode(key);
                curr->right = newNode;
                newNode->parent = curr;
                return;  
            }
            curr = curr->right;
        } else { 
            curr->frequency++;  
            if (curr->frequency > maxFreq) {
                maxFreq = curr->frequency; 
                nodeToSplay = curr;       
            }
            break;  
        }
    }

    if (nodeToSplay != NULL) {
        splay(Modtree, nodeToSplay);  
    }
}


// Preorder traversal for SplayTree
void preorderTraversal(Node* node, FILE* outputFile) {
    if (node == NULL) return;

    fprintf(outputFile, "%d ", node->key);  // Write to the file
    preorderTraversal(node->left, outputFile);
    preorderTraversal(node->right, outputFile);
}

void preorderTraversalModSplayTree(Node* node, FILE* outputFile) {
    if (node == NULL) return;

    fprintf(outputFile, "(%d,%d) ", node->key, node->frequency);
    preorderTraversalModSplayTree(node->left, outputFile);
    preorderTraversalModSplayTree(node->right, outputFile);
}

//we fill the output file by calling this function
void displayResult(SplayTree* splayTree, SplayTree* modSplayTree) {

    FILE* outputFile = fopen("output.txt", "w");

    if (outputFile == NULL) {
        perror("Error opening output file");
        return;
    }

    fprintf(outputFile, "Splay tree\n");
    fprintf(outputFile, "Tree (pre-order traversal): ");
    preorderTraversal(splayTree->root, outputFile);  
    int totalCostSplayTree = splayTree->comparisonCost + splayTree->rotationCost;
    fprintf(outputFile, "\nCost: %d \n", totalCostSplayTree);

    fprintf(outputFile, "Mod-Splay tree \n");
    fprintf(outputFile, "Tree (pre-order traversal): ");
    preorderTraversalModSplayTree(modSplayTree->root, outputFile);
    int totalCostModSplayTree = modSplayTree->comparisonCost + modSplayTree->rotationCost;
    fprintf(outputFile, " \nCost: %d \n", totalCostModSplayTree);
    fclose(outputFile);
}


void processInputFile(SplayTree* splayTree, SplayTree* modSplayTree, const char* inputFile) {
    FILE* file = fopen(inputFile, "r");
    if (!file) {
        perror("Error opening input file");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), file)) {
        char* token = strtok(buffer, ", \n");
        while (token!= NULL) {
            int key = atoi(token);
            insert(splayTree, key);
            insertModSplayTree(modSplayTree, key);
            token = strtok(NULL, ", \n");
        }
    }

    fclose(file);
}


int main() {
    SplayTree* splayTree = createSplayTree();
    SplayTree* modSplayTree = createSplayTree();

    const char* inputFile = "input.txt";

    processInputFile(splayTree,modSplayTree,inputFile);
    displayResult(splayTree,modSplayTree);

    destroy_tree(splayTree);
    destroy_tree(modSplayTree);
    
    return 0;
}
