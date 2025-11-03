#include "../include/decompress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#define HEADER_SIZE 2
#define BYTE_SIZE 8

/**
 * Gets the trash size from the first byte of the file
 * @param file_ptr pointer to the beginning of the file bytes
 * @returns trash size
 */
int trash_size(unsigned char *file_ptr){
  unsigned char byte1 = file_ptr[0];  //byte with the 3 bits of thrash size
  int trash_s = byte1 >> 5; //do the shift bit to get the trash size
  return trash_s;
}

/**
 * Gets the tree size from the first and second byte of the file
 * @param file_ptr pointer to the beginning of the file bytes
 * @returns tree size
 */
int tree_size(unsigned char *file_ptr){
  unsigned char byte1 = file_ptr[0];  
  unsigned char byte2 = file_ptr[1]; 

  unsigned char mask = 0b00011111; 
  int size_byte1 = byte1 & mask; 

  int tree_s = size_byte1 << 8; 
  tree_s = tree_s | byte2;     

  return tree_s;
}

/**
* Creates a node for the decompress tree
* @param byte the byte that will be stored in the node
* @param is_leaf a boolean that receives true if the node is a leaf and false otherwise
* @returns the created node
*/
decomp_node *create_decomp_node(void *byte, bool is_leaf) {
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

/**
* Creates a tree from a file.huff
* @param *file_ptr an array with the file's bytes
* @param size the size of the tree
* @param *i an index that points to the current byte being read
* @returns the root of the tree created
*/
decomp_node *create_decomp_tree(unsigned char *file_ptr, int size, int *i) {
  if ((*i) >= size) {
    return NULL;
  }

  unsigned char byte = file_ptr[*i + HEADER_SIZE]; 
  (*i)++;                         

  decomp_node *node;              
  if (byte == '\\') {             
    byte = file_ptr[*i + HEADER_SIZE]; 
    (*i)++;                            
    node = create_decomp_node(&byte, true);
  } else if (byte != '*') {
    node = create_decomp_node(&byte, true); // leaf
  } else {
    node = create_decomp_node(&byte, false); // internal node
  }
  
  if(!node->leaf) {                                     // Creates subtrees in pre-order
    node->left = create_decomp_tree(file_ptr, size, i);  
    node->right = create_decomp_tree(file_ptr, size, i); 
  }

  return node;
}

/**
 * Checks if the i-th bit of a byte is set (1) or not (0)
 * @param byte the byte to be checked
 * @param i the index of the bit to be checked (0-7)
 * @returns true if the i-th bit is set, false otherwise
 */
bool is_bit_i_set(unsigned char byte, int i){
  unsigned char mask = 128 >> i;
  return mask & byte;
}

/**
 * Decompresses the file buffer using the Huffman tree
 * @param buffer buffer with the compressed file bytes
 * @param root root of the Huffman tree
 * @param trash size of the trash bits
 * @param tree_size size of the Huffman tree
 * @returns buffer with the decompressed file bytes
 */
file_buffer *make_decomp_buffer(file_buffer *buffer, decomp_node *root, int trash, int tree_size){
  file_buffer *dec_file = init_buffer(0);
  decomp_node *curr = root; 
  unsigned char *comp_file = (unsigned char*) buffer->bytes;
  
  for (int i = HEADER_SIZE + tree_size; i < buffer->size; i++) {
    int byte_size = i == buffer->size - 1 ? BYTE_SIZE - trash : BYTE_SIZE;

    for (int j = 0; j < byte_size; j++) {
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

/**
 * Writes the decompressed buffer to a file
 * @param buffer buffer with the decompressed file bytes
 * @param filename name of the file to be created
 */
void write(file_buffer *file, char *filename) {
  FILE *dec_file = fopen(filename, "wb");
  if(dec_file == NULL){
    throw_error("error during the creation of the new file");
  }
  size_t count = fwrite(file->bytes, 1, file->size, dec_file);
  fclose(dec_file);

  if(count < file->size){
    throw_error("error during the write function");
  }
}

/**
 * Decompresses a .huff file
 * @param path path to the .huff file
 */
void decompress(char *path){
  file_buffer *file_path = read_file(path);
  unsigned char *bytes = (unsigned char*)file_path->bytes;

  int trash_s = trash_size(bytes);
  int tree_s = tree_size(bytes);

  int i = 0;
  decomp_node *root = create_decomp_tree(bytes, tree_s, &i);

  file_buffer *out_buffer = make_decomp_buffer(file_path, root, trash_s, tree_s);

  char* basefile = basename(path);
  char out_filename[strlen(basefile) - 4];
  strncpy(out_filename, basefile, strlen(basefile) - 5);
  out_filename[strlen(basefile) - 5] = '\0';
  write(out_buffer, out_filename);
}