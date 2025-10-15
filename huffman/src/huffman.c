#include "../include/read_file.h"
#include "../include/create_tree.h"
#include "../include/compress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void print_table(char *table[HASH_SIZE]) {
  printf("Table of huffman codes:\n");
  for (int i = 0; i < HASH_SIZE; i++) {
    if (table[i] != NULL) {
      printf("'%c': %s\n", i, table[i]);
    }
  }
}

void print_freq(huffman_node *head) {
  huffman_node *curr = head;
  while (curr != NULL) {
    printf("(%d,%d)", *(unsigned char*)curr->value, curr->frequency);
    if (curr->next == NULL) printf("\n");
    else printf(" -> ");
    curr = curr->next;
  }
}

void print_buffer(file_buffer *buffer) {
  for (int i = 0; i < buffer->size; i++) {
    printf("%c", ((unsigned char*)buffer->bytes)[i]);
  }
}

void print_frequency(frequency_hash *hash) {
  for (int i = 0; i < hash->size; i++) {
    printf("%c %d\n", hash->keys[i], hash->frequencies[hash->keys[i]]);
  }
}

int main() {
  char test[] = "/home/nicolas/Documents/DANFEs/12-2024.pdf";
  // char test[] = "/home/nicolas/Pictures/STScI-01GA6KKWG229B16K4Q38CH3BXS.png";
  // char test[] = "/home/nicolas/projects/estrutura_de_dados/Projetos-Estrutura-de-Dados/huffman/arthur.jpeg";
  // char test[] = "/home/nicolas/projects/estrutura_de_dados/Projetos-Estrutura-de-Dados/test2.txt";
  compress(test);
  return 0;
}