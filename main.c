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

/*
 * This array should only contain unique, positive non-zero 
 * integers.
 */
int keys[] = {2, 8, 23, 4, 5, 1, 19, 11};

int keys_length = sizeof(keys)/sizeof(keys[0]); /* Length of keys array */

typedef struct Node Node;

struct Node {
    int     key;
    int     height;
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

Node* avl_insertion(Node* root, int key)
{
    if (root == NULL){
        root = createNode(key);
        return root;
    }

    if (root->key > key)
        root->left = avl_insertion(root->left, key);
    else if (root->key < key)
        root->right = avl_insertion(root->right, key);

    /* Balancing and rotation here */

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
