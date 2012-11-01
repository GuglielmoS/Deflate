#include "file_stream.h"

#include <stdio.h>

int main(int arg, char *argv[])
{
    File_Stream *fs = File_Stream_new("test");

    int i = 0;
    while (!fs->is_finished) {
        printf("%c", File_Stream_next_byte(fs));
        i++;
    }

    File_Stream_destroy(fs);

    return 0;
}
