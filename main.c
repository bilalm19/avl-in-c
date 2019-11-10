/*
 * The following is an implementation of an avl tree in C.
 * The keys will be assigned in compile time. No user input
 * will be taken. The final tree will be printed at the end.
 * 
 * Note: This implementation does not aim to be optimal. There
 * are better ways to code an avl tree's insertion and deletion
 * algorithm, but we will not go there.
 */

#include <stdio.h>
#include <stdlib.h>

#define max(x, y) (x >= y ? x : y) /* Expression to get max */

/*
 * This array should only contain unique, positive non-zero integers.
 * The key should also not be greater than 100 so that checking for
 * duplicates is quick and easy (see sanity_check function below).
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
    /* 
     * rotate n to the left
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
    /* 
     * rotate n to the right
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

Node* balance(Node* n)
{
    /* Get balance factor of node */
    int balance_factor = get_balance_factor(n);

    if (balance_factor < -1) {
        /* Get balance factor of node's left child */
        int balance_factor_left = get_balance_factor(n->left);

        /* left-left case */
        if (balance_factor_left <= 0)
            return right_rotate(n);
        
        /* left-right case */
        if (balance_factor_left > 0) {
            n->left = left_rotate(n->left);
            return right_rotate(n);
        }
    }
    if (balance_factor > 1) {
        /* Get balance factor of node's right child */
        int balance_factor_right = get_balance_factor(n->right);
    
        /* right-right case */
        if (balance_factor_right >= 0)
            return left_rotate(n);

        /* right-left case */
        if (balance_factor_right < 0) {
            n->right = right_rotate(n->right);
            return left_rotate(n);
        }
    }
    return n;
}

Node* replace_and_balance(Node* target, Node* decendent)
{
    if (decendent->left != NULL) {
        /* This node will have a new left child after balancing */
        decendent->left = replace_and_balance(target, decendent->left);
        
        return balance(decendent);
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
Node* avl_delete_node(Node* root, int key)
{
    if (root->key > key)
        root->left = avl_delete_node(root->left, key); 
    if (root->key < key)
        root->right = avl_delete_node(root->right, key);
    if (root->key == key) {
        if (root->right == NULL) {
            /* root does not have a replacement node */
            Node* save_left = root->left;
            free(root);
            return save_left;
        }
        /* New right child of target node after rebalancing */
        Node* new_child = replace_and_balance(root, root->right);
        root->right = new_child;    
    }
    
    return balance(root);
}

Node* update_key(Node* root, int key, int new_key) {
    /* Will not work if key does not exist and program will crash */
    root = avl_delete_node(root, key);
    root = avl_insertion(root, new_key);

    return root;
}

#define HASH_SIZE 101
int sanity_check()
{
    /* 
     * Check for duplicate keys, numbers less than 1 or numbers greater
     * than 100. Having numbers less than 101 makes it easy to use hash
     * tables while utilizing less space. 
     */

    int hash_table[HASH_SIZE]; /* Use hash table to find duplicate keys */

    for (int i = 0; i < keys_length; i++) {
        if (keys[i] < 1) {
            printf("Error! Key cannot be less than 1.\n");
            return 1;
        }
        if (keys[i] > 100) {
            printf("Error! Key cannot be greater than 100.\n");
            return 1;
        }

        int hash = keys[i]%HASH_SIZE;
        if (hash_table[hash] == keys[i]) {
            printf("Error! Duplicate keys.\n");
            return 1;
        }
        hash_table[hash] = keys[i];
    }

    return 0;
}

int main()
{
    if (sanity_check() == 1)
        return 1;

    Node* root;

    /* Insert for all keys in keys array*/
    for (int i = 0; i < keys_length; i++) {
        root = avl_insertion(root, keys[i]);
    }

    print_tree(root);

    printf("\n/***START DELETION***/\n");
    for (int i = 0; i < keys_length; i++) {
        /* Note that root can change so we may need to reassign it */
        root = avl_delete_node(root, keys[i]);
        print_tree(root);
        printf("\n");
    }

    return 0;
}
