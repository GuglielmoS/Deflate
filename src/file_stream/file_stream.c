#include <stdio.h>
#include <stdlib.h>

#include "file_stream.h"

void File_Stream_load_buffer(File_Stream *fs)
{
    fs->buf_pos = 0;
    fs->n_available_bytes = fread(fs->buffer, sizeof(uint8_t), BUFSIZ, fs->fd);
    fs->is_finished = false;

    if (fs->n_available_bytes == 0) {
        fs->is_finished = true;
    }
}

File_Stream* File_Stream_new(const char *file_name)
{
    File_Stream *fs = (File_Stream*)malloc(sizeof(File_Stream));

    if (fs == NULL) {
        fprintf(stderr, "[ERROR-File_Stream_new] Malloc failed!\n"); 
        exit(EXIT_FAILURE);
    }
    else {
        fs->fd = fopen(file_name, "rb");;
        if (fs->fd == NULL) {
            fprintf(stderr, "[ERROR-File_Stream_new] fopen failed on %s\n", file_name);
            exit(EXIT_FAILURE);
        }
        File_Stream_load_buffer(fs);

        return fs;
    }
}

uint8_t File_Stream_next_byte(File_Stream *fs)
{
    if (!fs->is_finished) {
        if (fs->n_available_bytes > 0) {
            uint8_t read_byte = fs->buffer[fs->buf_pos++];

            // if we have to read the next file block
            fs->n_available_bytes--;
            if (fs->n_available_bytes == 0) {
                File_Stream_load_buffer(fs);
            }

            return read_byte;
        }
        else {
            fprintf(stderr, "[ERROR-File_Stream_next_byte] this shouldn't be"
                             "never happend!\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "[ERROR-File_Stream_next_byte] File ended!\n");
        exit(EXIT_FAILURE);
    }
}

void File_Stream_destroy(File_Stream *fs)
{
    fclose(fs->fd);
    free(fs);
}
