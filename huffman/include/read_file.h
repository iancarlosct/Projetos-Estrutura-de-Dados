#ifndef _READ_FILE_H
#define _READ_FILE_H

#define HEADER_SIZE 2
#define BYTE_SIZE 8
/**
 * struct to store the file bytes and its size
 */
typedef struct _buffer {
  void *bytes; /**< store the bytes of the file read. */
  int size; /**< size of the file */
} file_buffer;

file_buffer* init_buffer(int size);
void add_to_buffer(file_buffer *buffer, void* byte);
void destroy_buffer(file_buffer *buffer);
file_buffer* read_file(char file_path[]);
void throw_error(char message[]);

#endif