#include<stdio.h>
#include<stdlib.h>

// Node structure with frequency counter
struct node {
    int key;
    int frequency;
    struct node *left, *right;
};

// Total cost variable
int totalCost = 0;

// Helper function to create a new node
struct node* newNode(int key) {
    struct node* node = (struct node*) malloc(sizeof(struct node));
    node->key = key;
    node->frequency = 0;
    node->left = node->right = NULL;
    return node;
}

// Right rotation
struct node *rightRotate(struct node *x) {
    struct node *y = x->left;
    x->left = y->right;
    y->right = x;
    totalCost += 1; // Zig rotation cost
    return y;
}

// Left rotation
struct node *leftRotate(struct node *x) {
    struct node *y = x->right;
    x->right = y->left;
    y->left = x;
    totalCost += 1; // Zig rotation cost
    return y;
}

// Splay operation with cost tracking
struct node *splay(struct node *root, int key) {
    if (root == NULL || root->key == key)
        return root;

    totalCost += 1; // Comparison cost
    if (root->key > key) { // Key in left subtree
        if (root->left == NULL)
            return root;

        totalCost += 1; // Comparison cost
        if (root->left->key > key) { // Zig-Zig
            root->left->left = splay(root->left->left, key);
            root = rightRotate(root);
            totalCost += 2; // Zig-Zig rotation cost
        } else if (root->left->key < key) { // Zig-Zag
            root->left->right = splay(root->left->right, key);
            if (root->left->right) {
                root->left = leftRotate(root->left);
                totalCost += 2; // Zig-Zag rotation cost
            }
        }
        return root->left ? rightRotate(root) : root;
    } else { // Key in right subtree
        if (root->right == NULL)
            return root;

        totalCost += 1; // Comparison cost
        if (root->right->key > key) { // Zag-Zig
            root->right->left = splay(root->right->left, key);
            if (root->right->left) {
                root->right = rightRotate(root->right);
                totalCost += 2; // Zag-Zig rotation cost
            }
        } else if (root->right->key < key) { // Zag-Zag
            root->right->right = splay(root->right->right, key);
            root = leftRotate(root);
            totalCost += 2; // Zag-Zag rotation cost
        }
        return root->right ? leftRotate(root) : root;
    }
}

// Insert operation
struct node* insert(struct node* root, int key) {
    if (root == NULL) {
        printf("Inserted %d: 0 comparisons, Total cost: %d\n", key, totalCost);
        return newNode(key);
    }

    struct node *prev = NULL, *curr = root;
    int comparisons = 0;
    while (curr) {
        prev = curr;
        comparisons++;
        if (key < curr->key)
            curr = curr->left;
        else if (key > curr->key)
            curr = curr->right;
        else { // Key already in tree
            curr->frequency++;
            printf("Key %d exists, frequency incremented. Total cost: %d\n", key, totalCost);
            return root;
        }
    }

    totalCost += comparisons; // Add comparison cost
    struct node* newNodePtr = newNode(key);
    if (key < prev->key)
        prev->left = newNodePtr;
    else
        prev->right = newNodePtr;

    printf("Inserted %d: %d comparisons, Total cost: %d\n", key, comparisons, totalCost);
    return root;
}

// Search operation with conditional splay
struct node* search(struct node* root, int key) {
    int comparisons = 0;
    struct node *curr = root;

    while (curr!= NULL && curr->key != key) {
        comparisons++;
       // printf("toplam comparison %d \n", comparisons);
        if (key < curr->key){
            curr = curr->left;
           // printf("sola kaydim \n");
        }
            
        else{
            curr = curr->right;
            //printf("saga kaydim \n");
        }
            
    }
    
    //artık esit ya da null.
    
    if (curr && curr->key == key) {
        comparisons++;
        totalCost += comparisons; // Add comparison cost
        curr->frequency++;
        printf("Key %d found with frequency %d: %d comparisons, Total cost: %d\n", key, curr->frequency, comparisons, totalCost);

        struct node *maxFreqNode = root;
        int maxFreq = 0;

        // Check max frequency
        while (root) {
            if (root->frequency > maxFreq)
                maxFreq = root->frequency;
            root = (key < root->key) ? root->left : root->right;
        }

        if (curr->frequency > maxFreq) {
            printf("Performing splay for key %d as it has maximum frequency %d.\n", key, maxFreq);
            return splay(root, key);
        }
    } else {
        printf("Key %d not found: %d comparisons, Total cost: %d\n", key, comparisons, totalCost);
    }

    
    return root;
}

// Pre-order traversal
void preOrder(struct node *root) {
    if (root != NULL) {
        printf("%d(f=%d) ", root->key, root->frequency);
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main() {
    struct node *root = NULL;

    root = insert(root, 3);
    root = insert(root, 8);
    root = insert(root, 12);
    root = insert(root, 1);
    root = insert(root, 4);
    root = insert(root, 5);
    root = insert(root, 7);
    root = insert(root, 10);
    

    printf("\nPreorder traversal of the final tree:\n");
    preOrder(root);
    printf("\nTotal cost: %d\n", totalCost);

    return 0;
}