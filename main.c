/*
 * The following is an implementation of an avl tree in C.
 * The keys will be assigned in compile time. No user input
 * will be taken. The final tree will be printed at the end.
 * 
 * Note: This implementation does not aim to be optimal. There
 * are better ways to code an avl tree's insertion and deletion
 * algorithm, but we will not go there.
 */


// TODO:
// Deletion
// Update
// Check duplicate keys and range of keys

#include <stdio.h>
#include <stdlib.h>

#define max(x, y) (x >= y ? x : y) /* Expression to get max */

/*
 * This array should only contain unique, positive non-zero 
 * integers.
 */
int keys[] = {2, 8, 23, 4, 5, 1, 19, 11};

int keys_length = sizeof(keys)/sizeof(keys[0]); /* Length of keys array */

typedef struct Node Node;

struct Node {
    int     key;
    int     balance_factor;
    Node*   left;
    Node*   right;
};

void print_tree(Node* root)
{
    /* Printing is done in a pre-order traversal way */

    if (root == NULL)
        return;

    printf("%d\n", root->key);
    print_tree(root->left);
    print_tree(root->right);
}

Node* createNode(int key)
{
    Node* node = malloc(sizeof(Node));
    node->key = key;
    return node;
}

int getHeight(Node* n)
{
    if (n == NULL)
        return 0;

    return 1+max(getHeight(n->left), getHeight(n->right));
}

Node* left_rotate(Node* n)
{
    /* rotate n to the left
     * n->right will become n's parent
     * n's old right child's left will become n->right
     * n's new parent's left child will be n
     */

    Node* n_new_parent = n->right; /* n's right will be n's new parent */
    
    /* n's old right child's left will become n's right child... */
    n->right = n->right->left;
    /* ... and n will become the left child of n's new parent */
    n_new_parent->left = n;

    return n_new_parent;
}

Node* right_rotate(Node* n)
{
    /* rotate n to the right
     * n->left will become n's parent
     * n's old left child's right will be n->left
     * n's new parent's left child will be n
     */

    Node* n_new_parent = n->left; /* n's left will be n's new parent */
    
    /* n's old left child's right will become n's left child... */
    n->left = n->left->right;
    /* ... and n will become the right child of n's new parent */
    n_new_parent->right = n;

    return n_new_parent;
}

Node* avl_insertion(Node* root, int key)
{
    if (root == NULL){
        root = createNode(key);
        return root;
    }

    if (root->key > key)
        root->left = avl_insertion(root->left, key);
    if (root->key < key)
        root->right = avl_insertion(root->right, key);

    root->balance_factor = getHeight(root->right) - getHeight(root->left);

    /* left-left case*/
    if (root->balance_factor < -1 && root->left->key > key) {
        return right_rotate(root);
    }

    /* right-right case */
    if (root->balance_factor > 1 && root->right->key < key) { 
        return left_rotate(root);
    }
    
    /* left-right case */
    if (root->balance_factor < -1 && root->left->key < key) {
        right_rotate(root);
        return left_rotate(root);
    }

    /* right-left case */
    if (root->balance_factor > 1 && root->right->key < key) {
        left_rotate(root);
        return right_rotate(root);
    }

    return root;
}

int main()
{
    Node* root;

    /* Insert for all keys in keys array*/
    for (int i = 0; i < keys_length; i++) {
        root = avl_insertion(root, keys[i]);
    }

    print_tree(root);
}
