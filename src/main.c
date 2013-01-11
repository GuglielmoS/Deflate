#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "deflate.h"

void die_exit(const char *exec_file_name)
{
    fprintf(stderr, "Usage: %s [-d|-f] -i <input_file> [-o <output_file>]\n", exec_file_name);
    fprintf(stderr, "\t c: compress 'input_file' to 'output_file'\n");
    fprintf(stderr, "\t c: compress in fast mode 'input_file' to 'output_file'\n");
    fprintf(stderr, "\t d: decompress 'input_file' to 'output_file'\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    Deflate_Params params; // fast = true
    bool compress = true;

    params.fast = false;
    params.in_file_name = NULL;
    params.out_file_name = NULL;

    int c;
    while ((c = getopt (argc, argv, "fcdi:o:")) != -1) {
        switch (c) {
            case 'c':
                compress = true;
                break;
            case 'f':
                params.fast = true;
                break;
            case 'd':
                compress = false;
                break;
            case 'i':
                params.in_file_name = optarg;
                break;
            case 'o':
                params.out_file_name = optarg;
                break;
            default:
                die_exit(argv[0]);
        }
    }

    if (params.in_file_name == NULL) {
        die_exit(argv[0]);
    }

    if (params.out_file_name == NULL) {
        params.out_file_name = strcat(strdup(params.in_file_name), ".zipped");
    }

    if (compress) {
        Deflate_encode(&params);
    }
    else {
        Deflate_decode(&params);
    }

    return 0;
}
