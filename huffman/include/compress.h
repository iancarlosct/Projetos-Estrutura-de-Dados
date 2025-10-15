#ifndef _H_COMPRESS
#define _H_COMPRESS

#define BYTE_SIZE 8
#define TRASH_SIZE 3

#include "read_file.h"
#include "create_tree.h"

void store_tree(huffman_node *tree, file_buffer *buffer, int *idx);
file_buffer* make_body(file_buffer *buffer, char *table[HASH_SIZE], int *trash);
file_buffer* make_header(huffman_node *tree, int tree_size, int trash_size);
void write_file(file_buffer *header, file_buffer *body);
void compress(char *path);

#endif