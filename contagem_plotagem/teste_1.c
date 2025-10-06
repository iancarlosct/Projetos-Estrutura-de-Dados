#include "avl.c"
#include "desbalanceada.c"
#include <stdio.h>

#define TEST_CASES 1000
#define LINE_SIZE 100
#define RANDOM_STATE 12

binary_tree* build_binary_search_tree(int begin, int end, binary_tree *tree) {
  int m = (begin + end) / 2;
  if (begin > end) return NULL;
  if (tree == NULL) tree = create_node(m);
  tree->left = build_binary_search_tree(begin, m-1, tree->left);
  tree->right = build_binary_search_tree(m+1, end, tree->right);
  return tree;
}

avl_tree* build_avl_tree(int begin, int end, avl_tree *tree) {
  int m = (begin + end) / 2;
  if (begin > end) return NULL;
  if (tree == NULL) tree = add_avl(tree, m);
  tree->left = build_avl_tree(begin, m-1, tree->left);
  tree->right = build_avl_tree(m+1, end, tree->right);
  return tree;
}

int main() {
  srand(RANDOM_STATE);
  FILE *fptr = fopen("teste_balanceado.csv", "w");
  char line[LINE_SIZE];
  binary_tree *bst;
  avl_tree *avl;
  int count_bst, count_avl, to_search;
  fputs("quantidade,arvore de busca binaria, arvore avl\n", fptr);
  for (int i = 0; i < TEST_CASES; i++) {
    bst = build_binary_search_tree(0, i, NULL);
    avl = build_avl_tree(0, i, NULL);
    to_search = rand() % (i+1);
    count_bst = 0;
    count_avl = 0;
    avl_search(avl, to_search, &count_avl);
    bst_search(bst, to_search, &count_bst);
    sprintf(line, "%d,%d,%d\n", i+1, count_bst, count_avl);
    fputs(line, fptr);
    dispose_bst(bst);
    dispose_avl(avl);
  }
  fclose(fptr);
}