#include "../include/read_file.h"
#include "../include/create_tree.h"
#include <stdio.h>
#include <stdlib.h>


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
    printf("(%c,%d)", *(unsigned char*)curr->value, curr->frequency);
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
  char test[] = "../test.txt";
  file_buffer *buffer = read_file(test);
  frequency_hash *hash = init_hash();
  populate_frequency(hash, buffer);
  huffman_node *head = create_huffman_list(hash);
  destroy_hash(hash);
  destroy_buffer(buffer);
  print_freq(head);
  huffman_node *huffman_tree_root = create_huffman_tree(head);
  printf("Sum of all frequencys: %d\n", huffman_tree_root->frequency); 
  char *table[HASH_SIZE] = {NULL};
  create_huffman_table(table, huffman_tree_root, "");
  print_table(table);
  return 0;
}