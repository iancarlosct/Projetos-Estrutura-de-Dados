#include "../include/read_file.h"
#include "../include/create_list.h"
#include <stdio.h>
#include <stdlib.h>


/** Create a new huffman node that unites two nodes
 * @param first first node
 * @param second second node
 * @return new huffman node that unites the two nodes
 */
huffman_node* create_united_node(huffman_node *first, huffman_node *second) {
  huffman_node* united_node = (huffman_node*)malloc(sizeof(huffman_node));
  united_node->value = NULL;
  united_node->frequency = first->frequency + second->frequency;
  united_node->next = first->next = second->next = NULL;
  united_node->left = first;
  united_node->right = second;
  return united_node;
}

/** Insert a huffman node in the huffman linked list in the correct position
 * @param head head of the huffman linked list
 * @param new_node node to be inserted
 * @return head of the huffman linked list
 */
huffman_node* insert_huffman_node(huffman_node *head, huffman_node *new_node) {
  if (head == NULL || new_node->frequency < head->frequency) {
    return new_node;
  }
  huffman_node *curr = head;
  while (curr->next != NULL && curr->next->frequency <= new_node->frequency) {
    curr = curr->next;
  }
  new_node->next = curr->next;
  curr->next = new_node;
  return head;
}

/** 
 *Create huffman tree from the huffman linked list
 * @param head head of the huffman linked list
 * @return root of the huffman tree
 */
huffman_node* create_huffman_tree(huffman_node *head) {
  while (head != NULL && head->next != NULL) {
    huffman_node *first = head;
    huffman_node *second = head->next;
    huffman_node *united_node = create_united_node(first, second);
    head = second->next;
    head = insert_huffman_node(head, united_node);
  }
  return head;
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
  printf("%d\n", huffman_tree_root->frequency); 
  return 0;
}