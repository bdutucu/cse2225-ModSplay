#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    int key;
    int frequency;
    //frequency=0;
} Node;

int totalcost=0;

typedef struct SplayTree {
    Node* root;
    unsigned long size;
} SplayTree;

int get_height(Node* node) {
    if (node == NULL)
        return 0;
    else {
        int lheight = get_height(node->left);
        int rheight = get_height(node->right);
        if (lheight > rheight)
            return (lheight + 1);
        else
            return (rheight + 1);
    }
}



void printCurrentLevel(struct Node* root, int level) {
    if (root == NULL)
        return;
    if (level == 1)
        printf("%d ", root->key);
    else if (level > 1) {
        printCurrentLevel(root->left, level - 1);
        printCurrentLevel(root->right, level - 1);
    }
}

void printTree( SplayTree* tree ){    
int height= get_height(tree->root);
int i;
for(i=1;i<=height;i++){
    printCurrentLevel(tree->root,i);
    printf("\n");
}
}


void preorderTraversal(Node* node) {
    if (node == NULL)
        return;

    // Visit the current node
    printf("%d ", node->key);

    // Traverse the left subtree
    preorderTraversal(node->left);

    // Traverse the right subtree
    preorderTraversal(node->right);
}

void printPreorder(SplayTree* tree) {
    printf("Preorder Traversal:\n");
    preorderTraversal(tree->root);
    printf("\n");
}


Node* create_node(int key) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (!new_node) {
        perror("Failed to allocate memory for new node");
        exit(EXIT_FAILURE);
    }
    new_node->left = new_node->right = new_node->parent = NULL;
    new_node->key = key;
    new_node->frequency=0;
    return new_node;
}


SplayTree* create_tree() {
    SplayTree* tree = (SplayTree*)malloc(sizeof(SplayTree));
    if (!tree) {
        perror("Failed to allocate memory for splay tree");
        exit(EXIT_FAILURE);
    }
    tree->root = NULL;
    tree->size = 0;
    return tree;
}



void left_rotate(SplayTree* tree, Node* x) {
    if (!x || !x->right) {
        printf("left rotate failed because node is null.");
       return; // Ensure x and x->right are not NULL 
    }
    

    Node* y = x->right;

    
    x->right = y->left; // taşıma işlemi ynin soluna gecicek ya orayı bosaltiyoruz.
    
    if (y->left) y->left->parent = x;

    y->parent = x->parent;

    if (!x->parent) {
        tree->root = y; // Update root if x was the root
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
    // if x is the right child, we do left rotation.
    left_rotate(tree ,x->parent); 
}

else{
    //if x is the left child, we do right rotation.
    right_rotate(tree,x->parent);
}

// initialize costs here.
totalcost++;

}

//if p(x) is not root and x is a left child and p(x) a right child, or vice versa.
void zig_zag(SplayTree* tree, Node* x){

    if(x == x->parent->right && x->parent == x->parent->parent->left){
        left_rotate(tree,x->parent);
        right_rotate(tree,x->parent);
        
    }

    else if(x== x->parent->left && x->parent == x->parent->parent->right){

        right_rotate(tree,x->parent);
        left_rotate(tree,x->parent);
        
    }
        
    totalcost += 2; //Zig-Zag rotation cost

}

//if p(x) is not the root and x and p(x) are both left or right children
void zig_zig(SplayTree* tree, Node* x){

if(x == x->parent->left && x->parent == x->parent->parent->left ){
    right_rotate(tree,x->parent->parent);
    right_rotate(tree, x->parent);
}

else if(x == x->parent->right && x->parent == x->parent->parent->right ){
    left_rotate(tree,x->parent->parent);
    left_rotate(tree, x->parent);
}

 totalcost += 2;
}



void splay(SplayTree* tree, Node* x) {

    while (x->parent) {

        if (!x->parent->parent) {

            if (x->parent->left == x) right_rotate(tree, x->parent);
            else left_rotate(tree, x->parent);

        } else if (x->parent->left == x && x->parent->parent->left == x->parent) {
            right_rotate(tree, x->parent->parent);
            right_rotate(tree, x->parent);
        } else if (x->parent->right == x && x->parent->parent->right == x->parent) {
            left_rotate(tree, x->parent->parent);
            left_rotate(tree, x->parent);
        } else if (x->parent->left == x && x->parent->parent->right == x->parent) {
            right_rotate(tree, x->parent);
            left_rotate(tree, x->parent);
        } else {
            left_rotate(tree, x->parent);
            right_rotate(tree, x->parent);
        }
    }
}

// Replace node u with node v
void replace(SplayTree* tree, Node* u, Node* v) {
    if (!u->parent) tree->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v) v->parent = u->parent;
}

// Find the minimum node in the subtree
Node* subtree_minimum(Node* u) {
    while (u->left) u = u->left;
    return u;
}

// Find the maximum node in the subtree
Node* subtree_maximum(Node* u) {
    while (u->right) u = u->right;
    return u;
}

// Insert a key into the splay tree
void insert(SplayTree* tree, int key) {
    Node* z = tree->root;
    Node* p = NULL;

    while (z) { // z null olursa çıkar
        p = z;
        if (key < z->key) z = z->left; //gelen key z den kucukse z yi null yap
        else z = z->right;
    }

    z = create_node(key);
    z->parent = p;

    if (!p) tree->root = z; //p null ise rootu z yap.

    //p nin soluna veya sagina yerlestiriyoruz. keyi iceren yeni node u yani z yi.
    else if (key < p->key) p->left = z; 
    else p->right = z;

   // splay(tree, z);
    tree->size++;
}

// Find a node with the given key
Node* find(SplayTree* tree, int key) {
    Node* z = tree->root;
    while (z) {
        if (key < z->key) z = z->left;
        else if (key > z->key) z = z->right;
        else return z;
    }
    return NULL;
}

// Delete a node with the given key
void erase(SplayTree* tree, int key) {
    Node* z = find(tree, key);
    if (!z) return;

    splay(tree, z);

    if (!z->left) replace(tree, z, z->right);
    else if (!z->right) replace(tree, z, z->left);
    else {
        Node* y = subtree_minimum(z->right);
        if (y->parent != z) {
            replace(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        replace(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
    }

    free(z);
    tree->size--;
}

// Get the minimum key in the tree
int minimum(SplayTree* tree) {
    Node* min_node = subtree_minimum(tree->root);
    return min_node ? min_node->key : -1; // Handle empty tree case
}

// Get the maximum key in the tree
int maximum(SplayTree* tree) {
    Node* max_node = subtree_maximum(tree->root);
    return max_node ? max_node->key : -1; // Handle empty tree case
}

// Check if the tree is empty
bool is_empty(SplayTree* tree) {
    return tree->root == NULL;
}

// Get the size of the tree
unsigned long get_size(SplayTree* tree) {
    return tree->size;
}

// Free the tree nodes
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



int main(){
     //SplayTree* tree = create_tree();
     SplayTree* tree2= create_tree();

    insert(tree2, 58);
    insert(tree2, 24);
    insert(tree2, 26);
    
    /*insert(tree2, 13);
    insert(tree2, 78);
    insert(tree2, 45);
    insert(tree2, 12);
    insert(tree2, 68);
    insert(tree2, 72);*/
    

    printTree(tree2);

    printf("Printing preorder \n");
    printPreorder(tree2);

    left_rotate(tree2,find(tree2,24));
    printTree(tree2);

    right_rotate(tree2,find(tree2,58));
    printTree(tree2);



    printf("\n check if it is solved?      \n");
    printTree(tree2);
    printf("Printing preorder \n");
    printPreorder(tree2);

     
    
    /*
    
    // Insert some values
    insert(tree, 20);
    insert(tree, 10);
    insert(tree, 30);
    insert(tree, 25);
    insert(tree, 5);
    
    // Print the size of the tree
    printf("Size of tree: %lu\n", get_size(tree));
    
    // Find and print a node
    Node* found = find(tree, 25);
    if (found) {
        printf("Found node with key: %d\n", found->key);
    } else {
        printf("Node with key 25 not found.\n");
    }
    
    // Print the minimum and maximum values in the tree
    printf("Minimum value: %d\n", minimum(tree));
    printf("Maximum value: %d\n", maximum(tree));

    printf("root of the tree %d \n", tree->root->key);
    
    // Delete a node
    erase(tree, 10);
    printf("Size of tree after deleting 10: %lu\n", get_size(tree));
     printf("root of the tree %d \n", tree->root->key);
    
    // Try to find a deleted node
    found = find(tree, 10);
    if (found) {
        printf("Found node with key: %d\n", found->key);
    } else {
        printf("Node with key 10 not found.\n");
    }
    
    // Print the size of the tree again
    printf("Size of tree after deletion: %lu\n", get_size(tree));
    
    // Free the tree memory
    destroy_tree(tree);
    */

  /*printf("Size of tree after deletion: %lu\n", get_size(tree2));
   destroy_tree(tree);
   printf("root of the tree %d \n", tree2->root->key);
   printf("right child of root %d \n", tree->root->right->key);
   printf("right child of root %d \n", tree->root->right->key); */


   destroy_tree(tree2);
    return 0;



}
