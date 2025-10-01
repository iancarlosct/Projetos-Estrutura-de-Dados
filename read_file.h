#include <stdlib.h>
#include <stdio.h>

typedef struct _buffer {
  unsigned char *buff;
  int size;
} file_buffer;

void read_file(char *filename, file_buffer *buffer) {
  // error treatment
  FILE *fptr;
  size_t file_size;
  fptr = fopen(filename, "rb"); // if fails
  // get file size
  fseek(fptr, 0, SEEK_END); // search about this functions
  file_size = ftell(fptr);
  rewind(fptr);
  
  buffer->size = file_size;
  buffer->buff = (unsigned char*)malloc(sizeof(unsigned char)*file_size);
  fread(buffer->buff, 1, file_size, fptr);
  fclose(fptr);
}