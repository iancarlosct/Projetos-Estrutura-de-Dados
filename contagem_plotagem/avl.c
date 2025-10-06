#include <stdlib.h>

typedef struct _TreeNode {
  int value;
  int height;
  struct _TreeNode *left;
  struct _TreeNode *right;
} avl_tree;

int max(int a, int b) {
  return (a > b) ? a : b;
}

int compute_height(avl_tree *tree) {
  if (tree == NULL) return -1;
  return 1 + max(compute_height(tree->left), compute_height(tree->right));
}


avl_tree* create_tree(int value, avl_tree *left, avl_tree *right) {
  int left_height = left == NULL ? 0 : left->height;
  int right_height = right == NULL ? 0 : right->height;
  avl_tree *tree = (avl_tree*)malloc(sizeof(avl_tree));
  tree->value = value;
  tree->height = max(compute_height(left), compute_height(right)) + 1;
  tree->left = left;
  tree->right = right;
  return tree;
}

void dispose_avl(avl_tree *tree) {
  if (tree == NULL) return;
  dispose_avl(tree->left);
  dispose_avl(tree->right);
  free(tree);
}

int balance_factor(avl_tree *tree) {
  if (tree == NULL) return 0;
  if (tree->left != NULL && tree->right != NULL) {
    return tree->left->height - tree->right->height;
  } else if (tree->left != NULL && tree->right == NULL) {
    return tree->left->height + 1;
  } else {
    return -tree->right->height-1;
  }
}

avl_tree* rotate_left(avl_tree *tree) {
  avl_tree *subtree = NULL;
  if (tree->right == NULL) return tree;
  subtree = tree->right;
  if (subtree->right != NULL) {
    tree->right = subtree->left;
    subtree->left = tree;
  }
  subtree->height = compute_height(subtree);
  tree->height = compute_height(tree);
  return subtree;
}

avl_tree* rotate_right(avl_tree *tree) {
  avl_tree *subtree = NULL;
  if (tree->left == NULL) return tree;
  subtree = tree->left;
  if (subtree->left != NULL) {
    tree->left = subtree->right;
    subtree->right = tree;
  }
  subtree->height = compute_height(subtree);
  tree->height = compute_height(tree);
  return subtree;
}

avl_tree* add_avl(avl_tree *tree, int value) {
  if (tree == NULL) return create_tree(value, NULL, NULL);
  if (value > tree->value) {
    tree->right = add_avl(tree->right, value);
  } else if (value < tree->value) {
    tree->left = add_avl(tree->left, value);
  } else {
    return tree;
  }
  tree->height = compute_height(tree);

  int balance = balance_factor(tree);
  if (abs(balance) == 2) {
    if (balance == 2) {
      if (balance_factor(tree->left) == -1) {
        tree->left = rotate_left(tree->left);
      }
      tree = rotate_right(tree);
    } else if (balance == -2) {
      if (balance_factor(tree->right) == 1) {
        tree->right = rotate_right(tree->right);
      }
      tree = rotate_left(tree);
    }
  }
  return tree;
}

int avl_search(avl_tree* root, int value, int *count) {
  if (root == NULL) return 0;
  (*count)++;
  if (root->value == value) return 1;
  if (value > root->value) return avl_search(root->right, value, count);
  else return avl_search(root->left, value, count);
}