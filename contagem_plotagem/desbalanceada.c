#include <stdlib.h>

typedef struct _binary_treeNode {
  int value;
  struct _binary_treeNode *left;
  struct _binary_treeNode *right;
} binary_tree;


binary_tree* create_node(int value) {
  binary_tree *node = (binary_tree*)malloc(sizeof(binary_tree));
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void dispose_bst(binary_tree *root) {
  if (root == NULL) return;
  dispose_bst(root->left);
  dispose_bst(root->right);
  free(root);
}

int add_bst(binary_tree *root, int value) {
  if (value > root->value) {
    if (root->right == NULL) {
      binary_tree *new_node = create_node(value);
      root->right = new_node;
      return 1;
    }
    return add_bst(root->right, value);
  } else if (value < root->value) {
    if (root->left == NULL) {
      binary_tree *new_node = create_node(value);
      root->left = new_node;
      return 1;
    }
    return add_bst(root->left, value);
  } else {
    return 0;
  }
}

int bst_search(binary_tree* root, int value, int *count) {
  if (root == NULL) return 0;
  (*count)++;
  if (root->value == value) return 1;
  if (value > root->value) return bst_search(root->right, value, count);
  else return bst_search(root->left, value, count);
}