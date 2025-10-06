#include "avl.c"
#include "desbalanceada.c"
#include <stdio.h>

#define LINE_SIZE 100
#define RANDOM_STATE 7
#define TEST_CASES 1000

int main() {
  srand(RANDOM_STATE);
  FILE *fptr = fopen("teste_sequencia_crescente.csv", "w");
  char line[LINE_SIZE];
  binary_tree *bst = NULL;
  avl_tree *avl = NULL;
  int count_bst, count_avl, to_search;
  fputs("quantidade,arvore de busca binaria,arvore avl\n", fptr);
  for (int i = 0; i < TEST_CASES; i++) {
    if (bst == NULL) {
      bst = create_node(i);
    } else {
      add_bst(bst, i);
    }
    avl = add_avl(avl, i);
    to_search = rand() % (i+1);
    count_avl = 0;
    count_bst = 0;
    avl_search(avl, to_search, &count_avl);
    bst_search(bst, to_search, &count_bst);
    sprintf(line, "%d,%d,%d\n", i+1, count_bst, count_avl);
    fputs(line, fptr);
  }
  dispose_avl(avl);
  dispose_bst(bst);
  fclose(fptr);
  return 0;
}