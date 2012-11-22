#ifndef __FILE_STREAM__
#define __FILE_STREAM__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define FS_BUFFER_SIZE 8192
//32768

typedef struct {
    FILE    *fd;                     // file handler (file descriptor)
    uint8_t  buffer[FS_BUFFER_SIZE]; // temporary buffer
    size_t   buf_pos;                // current position in the buffer
    size_t   n_available_bytes;      // number of available bytes
    bool     is_finished;            // true if the file is finished
} File_Stream;

// it holds the information about a File_Stream
// is useful because it permits to save the context of a File_Stream in
// certain moment, thus restore it when is needed
typedef struct {
    size_t buf_pos;
    size_t n_available_bytes;
    bool   is_finished;
} File_Stream_Context;

File_Stream* File_Stream_new(const char *file_name);
uint8_t      File_Stream_next_byte(File_Stream *fs);
bool         File_Stream_is_finished(File_Stream *fs);
void         File_Stream_destroy(File_Stream *fs);
void         File_Stream_force_reload(File_Stream *fs);
void         File_Stream_Context_save(File_Stream *fs, File_Stream_Context *c);
void         File_Stream_Context_restore(File_Stream *fs, File_Stream_Context *c);

#endif /* __FILE_STREAM__ */
