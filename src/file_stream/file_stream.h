#ifndef __FILE_STREAM__
#define __FILE_STREAM__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define FS_BUFFER_SIZE 8192

typedef struct {
    FILE    *fd;                     // file handler (file descriptor)
    uint8_t  buffer[FS_BUFFER_SIZE]; // temporary buffer
    size_t   buf_pos;                // current position in the buffer
    size_t   n_available_bytes;      // number of available bytes
    bool     is_finished;            // true if the file is finished
} File_Stream;

File_Stream* File_Stream_new(const char *file_name);
uint8_t      File_Stream_next_byte(File_Stream *fs);
bool         File_Stream_is_finished(File_Stream *fs);
void         File_Stream_destroy(File_Stream *fs);

#endif /* __FILE_STREAM__ */
