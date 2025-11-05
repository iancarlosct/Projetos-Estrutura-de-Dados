#include <stdio.h>
#include <stdlib.h>

// color is 0 if RED, 1 if BLACK
typedef enum { RED, BLACK } color; 

// node struct
typedef struct node {
  int item;
  color color;
  struct node *left, *right, *parent;
} node;

// Red-Black Tree struct: holds the root and the unique NIL sentinel
typedef struct RedBlackTree{
  node *root;
  node *NIL;
} RBT;

/*
* Allocate memory to node ---
*  Sets the node's color to RED (all the new nodes in a RBT are initially RED) ---
*  Left and right point to NIL (the unique sentinel shared by the whole tree)
* @param tree to which the node will be added 
* @param item value to be stored in the new node
* @return memory address of new node
*/
node *createnode(RBT *tree, int item)
{
  node *new_node = (node*) malloc(sizeof(node));
  new_node->item = item;
  new_node->color = RED;
  new_node->left = new_node->right = tree->NIL;
  new_node->parent = NULL;
  return new_node;
}

/*
* Initializes a new Red-Black Tree with a NIL sentinel --- 
*  Sets the NIL's color to BLACK  ---
*  NIL's left and right point to NIL (self-referencing sentinel) ---
*  Initial root of tree is NIL
* @return new_tree memory address 
*/
RBT *createtree()
{
  RBT *new_tree = (RBT*) malloc(sizeof(RBT));
  new_tree->NIL = (node*) malloc(sizeof(node));
  new_tree->NIL->color = BLACK;
  new_tree->NIL->parent = NULL;
  new_tree->NIL->left = new_tree->NIL->right = new_tree->NIL;
  new_tree->root = new_tree->NIL;
  return new_tree;
}

/*
* Performs a left rotation around node x --- 
*  The right child of x (y) moves up, and x becomes y's left child ---
*  Preserves the BST property but changes tree structure
* @param tree that will be rotated
* @param x pivot node
*/
void left_rotate(RBT *tree, node *x)
{
  node *y = x->right; // get the right child
  x->right = y->left; // move y's left subtree to be x's right subtree
  if(y->left != tree->NIL){
    y->left->parent = x;
  }

  y->parent = x->parent;

  if(x->parent == NULL){
    tree->root = y;                  // if x is the root, set y to root
  } else if (x == x->parent->left){
    x->parent->left = y;             // if x is the left child, set y to the left child of x's parent
  } else {
    x->parent->right = y;            // if x is the right child, set y to the right child of x's parent
  }

  y->left = x; //places x to the left of y, ending the rotation
  x->parent = y;
}

/*
* Performs a right rotation around node x ---
*  The left child of x (y) moves up, and x becomes y's right child ---
*  Preserves the BST property but changes tree structure
* @param tree that will be rotated
* @param x pivot node
*/
void right_rotate(RBT *tree, node *x)
{
  node* y = x->left;
  x->left = y->right;
  if(y->right != tree->NIL){
    y->right->parent = x;
  }

  y->parent = x->parent;

  if(x->parent == NULL){
    tree->root = y;
  } else if(x == x->parent->right){
    x->parent->right = y;
  } else {
    x->parent->left = y;
  }

  y->right = x;
  x->parent = y;
}