#include "include/compress.h"
#include <stdlib.h>
#include <stdio.h>

file_buffer* make_body(file_buffer* buffer, char *table[HASH_SIZE], int *trash) {
  unsigned char byte = 0;
  int filled = 0;
  char *new_code;
  int code_len;
  file_buffer *body = init_buffer(0);
  for (int i = 0; i < buffer->size; i++) {
    unsigned char  idx = *(unsigned char*)(buffer->bytes + i);
    new_code = table[idx];
    // add bytes according to path
    code_len = strlen(new_code);
    for (int c = 0; c < code_len; c++) {
      byte |= ((new_code[c] == '1') << (BYTE_SIZE - filled - 1));
      filled++;
      if (filled == BYTE_SIZE) {
        add_to_buffer(body, &byte);
        byte = 0;
        filled = 0;
      }
    }
  }
  // check if there is still content to be appended
  if (filled != 0) {
    (*trash) = BYTE_SIZE - filled;
    add_to_buffer(body, &byte);
  } else {
    trash = 0;
  }

  return body;
}

void store_tree(huffman_node *tree, file_buffer *buffer, int *idx) {
  if (tree == NULL) return;
  if (tree->left == NULL && tree->right == NULL) {
    if (*(unsigned char*)tree->value == '*' || *(unsigned char*)tree->value == '\\') {
      memset((buffer->bytes + 2 + *idx), '\\', 1);
      (*idx)++;
    }
    memset((buffer->bytes + 2 + *idx), *(unsigned char*)tree->value, 1);
  } else {
    memset((buffer->bytes + 2 + *idx), '*', 1);
  }
  (*idx)++;
  store_tree(tree->left, buffer, idx);
  store_tree(tree->right, buffer, idx);
}

file_buffer* make_header(huffman_node *tree, int tree_size, int trash_size) {
  file_buffer *header = init_buffer(2 + tree_size);
  // trash on the first three bytes
  for (int i = 0; i < TRASH_SIZE; i++) {
    if (trash_size > 0) {
      *(unsigned char*)(header->bytes) |= ((trash_size % 2) << i);
      trash_size /= 2;
    }
  }
  // tree size on the other 13 bytes
  // put value on the next byte
  for (int i = 0; i < BYTE_SIZE; i++) {
    if (tree_size > 0) {
      *(unsigned char*)(header->bytes + 1) |= ((tree_size % 2) << i);
      tree_size /= 2;
    }
  }
  // the leftovers put on the trash byte
  *(unsigned char*)(header->bytes) <<= BYTE_SIZE - TRASH_SIZE;
  *(unsigned char*)(header->bytes) |= tree_size;
  // store tree
  int idx = 0;
  store_tree(tree, header, &idx);

  return header;
}

void write_file(file_buffer *header, file_buffer *body) {
  FILE *fptr = fopen("file.huff", "wb");
  fwrite(header->bytes, 1, header->size, fptr);
  fwrite(body->bytes, 1, body->size, fptr);
  fclose(fptr);
}

void compress(char* path) {
  // creating huffman tree and table for compressing
  file_buffer *buffer = read_file(path);
  frequency_hash *frequency = init_hash();
  populate_frequency(frequency, buffer);
  int escape = 0, tree_size = (frequency->size * 2) - 1;
  escape += (frequency->frequencies['*'] > 0);
  escape += (frequency->frequencies['\\'] > 0);
  huffman_node *huff_tree = create_huffman_list(frequency);
  destroy_hash(frequency);
  huff_tree = create_huffman_tree(huff_tree);
  char *table[HASH_SIZE] = {NULL};
  create_huffman_table(table, huff_tree, "");

  int trash;
  file_buffer *body = make_body(buffer, table, &trash);
  file_buffer *header = make_header(huff_tree, tree_size + escape, trash);

  write_file(header, body);
}