#include "read_file.h"

#define HASH_INIT_SIZE 255

typedef struct _hash {
  unsigned char *keys;
  int *frequencies;
  int size;
} frequency_hash;

typedef struct _node {
  unsigned char value;
  int frequency;
  struct _node *next;
  struct _node *left;
  struct _node *right;
} huffman_node;


frequency_hash* init_hash() {
  frequency_hash *hash = (frequency_hash*)malloc(sizeof(frequency_hash));
  hash->frequencies = calloc(sizeof(int), HASH_INIT_SIZE);
  hash->keys = calloc(sizeof(unsigned char), HASH_INIT_SIZE);
  hash->size = 0;
  return hash;
}

void destroy_hash(frequency_hash *hash) {
  free(hash->frequencies);
  free(hash->keys);
  free(hash);
}

huffman_node* create_node(unsigned char value, int frequency) {
  huffman_node *new_node = (huffman_node*)malloc(sizeof(huffman_node));
  new_node->value = value;
  new_node->frequency = frequency;
  new_node->next = new_node->right = new_node->left = NULL;
  return new_node;
}

void add(frequency_hash *hash, unsigned char key) {
  if (hash->frequencies[key] == 0) {
    hash->keys[hash->size++] = key;
  }
  hash->frequencies[key]++;
}

void swap(unsigned char *arr, int a, int b) {
  unsigned char temp = arr[a];
  unsigned char ca = arr[a], cb = arr[b];
  arr[a] = arr[b];
  arr[b] = temp;
}

int compare(frequency_hash *hash, int a, int b) {
  int freq_a = hash->frequencies[a];
  int freq_b = hash->frequencies[b];
  if (freq_a > freq_b) return 1;
  if (freq_a < freq_b) return -1;
  return 0;
}

void sort_keys(frequency_hash *hash, int begin, int end, int(compare)(frequency_hash*, int, int)) {
  // make frequency hash void
  if (begin >= end || begin < 0) return;
  unsigned char pivot = hash->keys[end];
  int i = begin;
  for (int j = begin; j < end; j++) {
    if (compare(hash, hash->keys[j], pivot) <= 0) {
      swap(hash->keys, i, j);
      i++;
    }
  }
  swap(hash->keys, i, end);
  sort_keys(hash, begin, i - 1, compare);
  sort_keys(hash, i + 1, end, compare); 
}

void init_frequency_hash(frequency_hash *hash, file_buffer *buffer) { // maybe make buffer const
  for (int i = 0; i < buffer->size; i++) {
    add(hash, buffer->buff[i]);
  }
  sort_keys(hash, 0, hash->size-1, compare);
}

huffman_node* create_huffman_list(frequency_hash *hash) {
  huffman_node *head = create_node(hash->keys[0], hash->frequencies[hash->keys[0]]);
  huffman_node *curr = head, *new_node;
  unsigned char key;
  for (int i = 1; i < hash->size; i++) {
    key = hash->keys[i];
    new_node = create_node(key, hash->frequencies[key]);
    curr->next = new_node;
    curr = curr->next;
  }
  return head;
}

void print_freq(huffman_node *head) {
  huffman_node *curr = head;
  while (curr != NULL) {
    printf("(%c,%d)", curr->value, curr->frequency);
    if (curr->next == NULL) printf("\n");
    else printf(" -> ");
    curr = curr->next;
  }
}

int main() {
  char path[FILENAME_MAX];
  file_buffer buffer;
  frequency_hash *hash = init_hash();
  huffman_node *head;
  scanf("%s", path);
  read_file(path, &buffer);
  init_frequency_hash(hash, &buffer);
  head = create_huffman_list(hash);
  destroy_hash(hash);
  print_freq(head);
  return 0;
}