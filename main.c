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
int keys[] = {22, 2, 13, 35, 28, 17, 11, 42, 24, 25};

int keys_length = sizeof(keys)/sizeof(keys[0]); /* Length of keys array */

typedef struct Node Node;

struct Node {
    int     key;
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

int get_balance_factor(Node* n)
{
    return (getHeight(n->right) - getHeight(n->left));
}

Node* avl_insertion(Node* root, int key)
{
    if (root == NULL) {
        root = createNode(key);
        return root;
    }

    if (root->key > key)
        root->left = avl_insertion(root->left, key);
    if (root->key < key)
        root->right = avl_insertion(root->right, key);

    int balance_factor = get_balance_factor(root);

    /* left-left case*/
    if (balance_factor < -1 && root->left->key > key)
        return right_rotate(root);

    /* right-right case */
    if (balance_factor > 1 && root->right->key < key) 
        return left_rotate(root);
    
    /* left-right case */
    if (balance_factor < -1 && root->left->key < key) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }

    /* right-left case */
    if (balance_factor > 1 && root->right->key > key) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

Node* replace_and_balance(Node* target, Node* decendent)
{
    if (decendent == NULL)
        return NULL; /* There is no replacement for the target node */

    if (decendent->left != NULL) {
        /* This node will have a new left child after balancing */
        decendent->left = replace_and_balance(target, decendent->left);
        
        int balance_factor = get_balance_factor(decendent);
        int balance_factor_left = get_balance_factor(decendent->left);
        int balance_factor_right = get_balance_factor(decendent->right);

        /* left-left case */
        if (balance_factor < -1 &&
            balance_factor_left <= 0)
            return right_rotate(decendent);
        
        /* left-right case */
        if (balance_factor < -1 &&
            balance_factor_left > 0) {
            decendent->left = left_rotate(decendent->left);
            return right_rotate(decendent);
        }

        /* right-right case */
        if (balance_factor > 1 &&
            balance_factor_right >= 0)
            return left_rotate(decendent);

        /* right-left case */
        if (balance_factor > 1 &&
            balance_factor_right < 0) {
            decendent->right = right_rotate(decendent->right);
            return left_rotate(decendent);
        }
    }

    if (decendent->left == NULL) {
        /* decendent is the replacement node and will be removed */
        target->key = decendent->key;
        Node* right_child = decendent->right;
        free(decendent);
        return right_child;
    }
}

/* Delete node with key equal argument's key value */
void avl_delete_node(Node* root, int key)
{
    if (root->key > key)
        avl_delete_node(root->left, key);
    if (root->key < key)
        avl_delete_node(root->right, key);
    if (root->key == key) {
        Node* new_child = replace_and_balance(root, root->right);
        
    }
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
