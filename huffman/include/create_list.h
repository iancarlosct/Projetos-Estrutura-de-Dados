#ifndef _CREATE_LIST_H
#define _CREATE_LIST_H

#include "read_file.h"

#define HASH_SIZE 256

/**
 * struct of a hashmap used to count the frequency of each byte
 */
typedef struct _hash {
  unsigned char keys[HASH_SIZE];
  /*! array that stores the used keys.
    * It is gonna be used later to sort by the frequencies.
    * Unsigned char was used to easily because is a easy way to represent bytes
    * as keys of a hashmap
    */

  int frequencies[HASH_SIZE]; /**< store the respective frequency of each key (byte) */
  int size; /**< current size of the hashmap, correspond to the quantity of keys  */
} frequency_hash;

/**
 * Node of the huffman linked list, that will later become the huffman tree.
 */
typedef struct _node {
  void *value; /**< Value of the original byte stored by the node */
  int frequency; /**< frequency in which this byte appears on the file */
  struct _node *next; /**< Next node on the linked list */
  struct _node *left; /**< Left node on the huffman tree */
  struct _node *right; /**< Right node on the huffman tree */
} huffman_node;

frequency_hash* init_hash();
void destroy_hash(frequency_hash *hash);
huffman_node* create_node(void *byte, int frequency);
void add(frequency_hash *hash, void* byte_key);
void swap_keys(unsigned char *arr, int idx_a, int idx_b);
int compare(frequency_hash *hash, unsigned char key_a, unsigned char key_b);
void sort_keys(frequency_hash *hash, int begin, int end, int (compare)(frequency_hash*,unsigned char,unsigned char));
void populate_frequency(frequency_hash *hash, file_buffer *buffer);
huffman_node* create_huffman_list(frequency_hash *hash);

#endif