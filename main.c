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
int keys[] = {2,8,23,4,5,1,19,11};

int keys_length = sizeof(keys)/sizeof(keys[0]); /* Length of keys array */

typedef struct Node Node;

struct Node {
    int     key;
    int     height;
    Node*   left;
    Node*   right;
};

void print_tree(Node* root){
    /* Printing is done in a pre-order traversal way */

    if (root==NULL)
        return;

    printf("%d\n", root->key);
    print_tree(root->left);
    print_tree(root->right);
}

/* Function Declaration for abstract function's use */
void avl_insert_left(Node* root, Node* child);
void avl_insert_right(Node* root, Node* child);

void avl_insertion(Node* root, Node* child){
    /* An abstract function */
    if (child->key < root->key)
        avl_insert_left(root, child);
    if (child->key > root->key)
        avl_insert_right(root, child);
}

void avl_insert_left(Node* root, Node* child){
    if (root->left == NULL)
        root->left = child;
    else
        avl_insertion(root->left, child);
}

void avl_insert_right(Node* root, Node* child){
    if (root->right == NULL)
        root->right = child;
    else
        avl_insertion(root->right, child);
}

void create_tree(Node* root, int key){
    if (root->key == 0)
        root->key = key;
    
    Node* child = malloc(sizeof(Node));
    child->key = key;

    avl_insertion(root, child);
}

int main(){
    Node* root = malloc(sizeof(Node));
    /* Initialize the root key to 0 to indicate nothing is assigned */
    root->key = 0;
    
    /* Insert for all keys in keys array*/
    for (int i=0; i<keys_length;i++){
        create_tree(root, keys[i]);
    }

    print_tree(root);
}