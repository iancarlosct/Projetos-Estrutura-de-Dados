#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../include/read_file.h"

/**
 * Initialize file_buffer with a given size
 * @param size size of the file stored in the buffer
 */
file_buffer* init_buffer(int size) {
  file_buffer *buffer = (file_buffer*)malloc(sizeof(file_buffer));
  buffer->size = size;
  buffer->bytes = malloc(size);
  return buffer;
}

/**
 * Deallocate file_buffer memory
 * @param buffer file_buffer object pointer to be deallocated
 */
void destroy_buffer(file_buffer *buffer) {
  free(buffer->bytes);
  free(buffer);
}

/**
 * Read a given file and stores its information on the file_buffer passed as argument
 * @param file_path path to file to be compressed
 * @return buffer file_buffer object that stores the bytes of the file
 */
file_buffer* read_file(char *file_path) {
  FILE *fptr;
  size_t file_size, read_result;
  fptr = fopen(file_path, "rb"); 
  if (fptr == NULL) throw_error("File does not exists"); 
  // get file size
  if (fseek(fptr, 0, SEEK_END) != 0) { 
    throw_error("Error during fseek on read_file");
  }
  file_size = ftell(fptr);
  if (file_size == -1) {
    throw_error("Error getting the file size");
  }
  rewind(fptr);
  
  file_buffer *buffer = init_buffer(file_size);
  if (buffer->bytes == NULL) {
    throw_error("Error while allocating memory for buffer");
  }
  read_result = fread(buffer->bytes, 1, file_size, fptr);
  if (ferror(fptr)) {
    throw_error("Error reading file");
  }
  if (fclose(fptr) == EOF) {
    throw_error("Error closing file");
  }
  return buffer;
}

/**
 * Show error message on stderr and stdout and finished the execution of the program
 * @param message text that will be exibited on the screen
 */
void throw_error(char message[]) {
  perror(message);
  printf("%s\n", strerror(errno));
  exit(EXIT_FAILURE);
}