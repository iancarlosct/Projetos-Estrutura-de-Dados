#include "../include/create_list.h"
#include <string.h>
#include <stdlib.h>


frequency_hash* init_hash() {
  /**
   * Initializes a frequency_hash
   * @return empty frequency_hash
   */
  frequency_hash *hash = (frequency_hash*)malloc(sizeof(frequency_hash));
  if (hash == NULL) throw_error("Error during frequency_hash memory allocation");
  for (int i = 0; i < HASH_SIZE; i++) hash->frequencies[i] = 0;
  hash->size = 0;
  return hash;
}

void destroy_hash(frequency_hash *hash) {
  /**
   * Dealocates frequency hash from memory
   * @param hash frequency hash that will be destroyed
   */
  free(hash);
}

huffman_node* create_node(void *byte, int frequency) {
  /**
   * Creates a huffman node
   * @param byte pointer to a byte from a file
   * @param frequency frequency of that byte in the file
   * @return huffman_node with next, left and right poiting to NULL
   */
  huffman_node *new_node = (huffman_node*)malloc(sizeof(huffman_node));
  if (new_node == NULL) {
    throw_error("Error during huffman_node memory allocation");
  }
  new_node->value = malloc(1);
  if (new_node->value == NULL) {
    throw_error("Error during huffman node value memory allocation");
  }
  memcpy(new_node->value, byte, 1);
  new_node->frequency = frequency;
  new_node->next = new_node->right = new_node->left = NULL;
  return new_node;
}

void add(frequency_hash *hash, void* byte_key) {
  /**
   * Adds one to the frequency value of a byte and push it to the keys array if
   * it is not there yet.
   * @param hash frequency hash
   * @param key byte read as unsigned char to easily represet it as a key
   */
  unsigned char key = *(unsigned char*)byte_key;
  if (hash->frequencies[key] == 0) {
    hash->keys[hash->size++] = key;
  }
  hash->frequencies[key]++;
}

void swap_keys(unsigned char *arr, int idx_a, int idx_b) {
  /**
   * swap position of elements in an array of unsigned char, used to swap keys
   * in the frequency hash keys array.
   * @param arr array of unsigned char, supposedly the keys array
   * @param idx_a index of the first element to be swapped
   * @param idx_b index of the second element to be swapped
   */
  unsigned char temp = arr[idx_a];
  arr[idx_a] = arr[idx_b];
  arr[idx_b] = temp;
}

int compare(frequency_hash *hash, unsigned char key_a, unsigned char key_b) {
  /** 
   * Compare the frequency of two bytes using the frequency hash
   * @param hash hash table with the frequencies
   * @param key_a byte represented as an unsigned char
   * @param key_b byte represented as an unsigned char
   * @return 0 if they are equal; 1 if key_a has a higher frequency; -1 otherwise
  */
  int freq_a = hash->frequencies[key_a];
  int freq_b = hash->frequencies[key_b];
  if (freq_a > freq_b) return 1;
  if (freq_a < freq_b) return -1;
  return 0;
}

void sort_keys(frequency_hash *hash, int begin, int end, int(compare)(frequency_hash*, unsigned char, unsigned char)) {
  /**
   * Function that uses quick sort algorithm to sort the bytes by frequency
   * using the hash table keys
   * @param hash hash table with the frequencies
   * @param begin initial index, presumably starts with 0
   * @param end final index, presumably its initial value is hash->size - 1
   * @param compare pointer to function used to compare the frequencies between
   * two bytes, represented as unsigned char
   */
  if (begin >= end || begin < 0) return;
  unsigned char pivot = hash->keys[end];
  int i = begin;
  for (int j = begin; j < end; j++) {
    if (compare(hash, hash->keys[j], pivot) <= 0) {
      swap_keys(hash->keys, i, j);
      i++;
    }
  }
  swap_keys(hash->keys, i, end);
  sort_keys(hash, begin, i - 1, compare);
  sort_keys(hash, i + 1, end, compare); 
}

void populate_frequency(frequency_hash *hash, file_buffer *buffer) {
  /**
   * Generate the frequencies using the file buffer and stores it on the
   * frequency hash, after that the keys are sorted in ascending order of frequency.
   * @param hash frequency hash, empty at this time
   * @param buffer struct with all the bytes of the file
   */
  for (int i = 0; i < buffer->size; i++) {
    add(hash, (buffer->bytes + i));
  }
  sort_keys(hash, 0, hash->size-1, compare);
}

huffman_node* create_huffman_list(frequency_hash *hash) {
  /**
   * Create huffman linked list
   * @param hash frequency hash with the keys sorted by ascending order of frequency
   * @return head of the huffman linked list
   */
  huffman_node *head = create_node(&hash->keys[0], hash->frequencies[hash->keys[0]]);
  huffman_node *curr = head, *new_node;
  unsigned char key;
  for (int i = 1; i < hash->size; i++) {
    key = hash->keys[i];
    new_node = create_node(&key, hash->frequencies[key]);
    curr->next = new_node;
    curr = curr->next;
  }
  return head;
}
