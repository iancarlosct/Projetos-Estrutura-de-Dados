#include <stdio.h>
#include "desbalanceada.c"
#include "avl.c"

#define RANDOM_STATE 11072004
#define LINE_SIZE 100
#define TEST_CASES 1000

void create_trees(int n, int items[n], binary_tree **bst, avl_tree **avl) {
  int curr_size = 0;
  int num;
  while (curr_size < n) {
    num = rand();
    if (*bst == NULL) {
      (*bst) = create_node(num);
      (*avl) = add_avl(*avl, num);
      items[curr_size++] = num;
    } else {
      if (!add_bst(*bst, num)) continue;
      (*avl) = add_avl(*avl, num);
      items[curr_size++] = num;
    }
  }
}

int main() {
  srand(RANDOM_STATE);
  binary_tree *bst = NULL;
  avl_tree *avl = NULL;
  char line[LINE_SIZE];
  FILE *fptr = fopen("teste_random.csv", "w");
  fputs("quantidade,arvore de busca binaria,arvore avl\n", fptr);
  int count_avl, count_bst, to_search;
  for (int i = 0; i < TEST_CASES; i++) {
    int items[i+1];
    create_trees(i+1, items, &bst, &avl);
    count_avl = count_bst = 0;
    to_search = items[rand() % (i+1)];
    bst_search(bst, to_search, &count_bst);
    avl_search(avl, to_search, &count_avl);
    sprintf(line, "%d,%d,%d\n", i+1, count_bst, count_avl);
    fputs(line, fptr);
    dispose_bst(bst);
    dispose_avl(avl);
    bst = NULL;
    avl = NULL;
  }
  fclose(fptr);
  return 0;
}