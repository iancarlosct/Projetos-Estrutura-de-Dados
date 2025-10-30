#ifndef _DECOMPRESS_H
#define _DECOMPRESS_H

#include "../include/read_file.h"
#include <stdbool.h>

typedef struct node{
  bool leaf;
  void *value;
  struct node *left;
  struct node *right;
}decomp_node;

decomp_node *create_decomp_node(void *byte, bool is_leaf);
decomp_node *create_decomp_tree(unsigned char *file_buffer, int size, int *i);
bool is_bit_i_set(unsigned char byte, int i);
file_buffer *make_decomp_buffer(file_buffer *buffer, decomp_node *root, int trash, int tree_size);
void decompress(char *path);

#endif