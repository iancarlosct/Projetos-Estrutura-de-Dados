#include "../include/read_file.h"
#include "../include/create_list.h"
#include <stdio.h>
#include <stdlib.h>

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
  return 0;
}