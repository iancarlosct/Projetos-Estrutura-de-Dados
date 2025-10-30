#include "../include/decompress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE 2
#define BYTE_SIZE 8

int trash_size(unsigned char *file_ptr){
  unsigned char byte1 = file_ptr[0];  //byte with the 3 bits of thrash size
  int trash_s = byte1 >> 5; //do the shift bit to get the trash size
  return trash_s;
}

int tree_size(unsigned char *file_ptr){
  unsigned char byte1 = file_ptr[0];  //get the first byte with 3 bits for trash size and 5 bits for tree size
  unsigned char byte2 = file_ptr[1]; //get the second byte with 8 bits for tree size

  unsigned char mask = 31; //make a mask with the binary 00011111
  int size_byte1 = byte1 & mask; //take the five final bits of the byte 1 (bits of tree size)

  int t_size = size_byte1 << 8; //shift the bits to the left, for give space to the bits of byte2
  t_size = t_size | byte2;     //concatenate the bits of the byte1 and byte2

  return t_size;
}

/*
* Creates a node for the decompress tree
* @param byte the byte that will be stored in the node
* @param is_leaf a boolean that receives true if the node is a leaf and false otherwise
*/
decomp_node *create_dec_node(void *byte, bool is_leaf) {
  decomp_node *new_node = (decomp_node*)malloc(sizeof(decomp_node));
  if(new_node == NULL) {
    throw_error("Error during decompress node memory allocation");
  }
  new_node->value = malloc(1);
  if(new_node->value == NULL) {
    throw_error("Error during decompress node value memory allocation");
  }
  memcpy(new_node->value, byte, 1);

  new_node->left = new_node->right = NULL;
  new_node->leaf = is_leaf;
  
  return new_node;
}

/*
* Creates a tree from a file.huff
* @param *file_ptr an array with the file's bytes
* @param size the size of the tree
* @param *i an index that points to the current byte being read
*/
decomp_node *create_dec_tree(unsigned char *file_ptr, int size, int *i) {
  if ((*i) >= size) {
    return NULL;
  }

  unsigned char byte = file_ptr[*i]; // takes the actual byte
  (*i)++;                           // increments the index

  decomp_node *node;                // new node
  if (byte == '\\') {              // treatment to the escape character
    byte = file_ptr[(*i)++];      // store the next byte in the node and increment the index
    node = create_dec_node(&byte, true);
  } else if (byte != '*') {
    node = create_dec_node(&byte, true); // leaf
  } else {
    node = create_dec_node(&byte, false); // internal node
  }
  
  if(!node->leaf) {                                     // Creates subtrees in pre-order
    node->left = create_dec_tree(file_ptr, size, i);   // recursively calls the function for the left subtree
    node->right = create_dec_tree(file_ptr, size, i); // same for the right
  }

  return node;
}

bool is_bit_i_set(unsigned char byte, int i){
  unsigned char mask = 128 >> i;
  return mask & byte;
}

file_buffer *decompress(file_buffer *buffer, decomp_node *root, int trash, int tree_size){
  file_buffer *dec_file = init_buffer(1);
  decomp_node *curr = root; 
  unsigned char *comp_file = (unsigned char*) buffer->bytes;
  
  for (int i = HEADER_SIZE + tree_size; i < buffer->size; i++) {
    int b_size = i == buffer->size - 1 ? BYTE_SIZE - trash : BYTE_SIZE;

    for (int j = 0; j < b_size; j++) {
      bool bit = is_bit_i_set(comp_file[i], j);

      curr = bit ? curr->right : curr->left;

      if(curr->leaf){
        add_to_buffer(dec_file, curr->value);
        curr = root;
      }
    }
  }

  return dec_file;
}

void write(file_buffer *file) {
  FILE *dec_file = fopen("new_file", "wb");
  if(dec_file == NULL){
    throw_error("error during the creation of the new file");
  }
  size_t count = fwrite(file->bytes, 1, file->size, dec_file);
  fclose(dec_file);

  if(count < file->size){
    throw_error("error during the write function");
  }
}
