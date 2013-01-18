#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "deflate.h"

void die_exit(const char *exec_file_name)
{
    fprintf(stderr, "Usage: %s [-d|-f] -i <input_file> [-o <output_file>]\n", exec_file_name);
    fprintf(stderr, "\t c: compress 'input_file' to 'output_file'\n");
    fprintf(stderr, "\t f: compress in fast mode 'input_file' to 'output_file'\n");
    fprintf(stderr, "\t d: decompress 'input_file' to 'output_file'\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    // default extension of the output compressed file
    static const char *DF_FILE_EXT = ".dzip";

    // used for creating the output file name in case of compression
    char *out_fname = NULL;

    // compression/decompression parameters
    Deflate_Params params;
    bool compress = true;

    // sets the default parameter values
    params.fast = false;
    params.in_file_name = NULL;
    params.out_file_name = NULL;

    // goes through the user parameter and sets the relative
    // preferences in the data structure
    int c;
    while ((c = getopt(argc, argv, "fcdi:o:")) != -1) {
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

    // if the user doesn't provide the input file
    if (params.in_file_name == NULL) {
        die_exit(argv[0]);
    }

    // if the output file name isn't provided by the user,
    // we assume that it will be the input file name with the
    // extension defined in 'DF_FILE_EXT'.
    if (params.out_file_name == NULL) {
        size_t in_len = strlen(params.in_file_name),
               ext_len = strlen(DF_FILE_EXT);

        out_fname = (char*)malloc(in_len + ext_len + 1);
        for (int i = 0; i < in_len; i++) {
            out_fname[i] = params.in_file_name[i];
        }
        for (int i = 0; i < ext_len; i++) {
            out_fname[in_len + i] = DF_FILE_EXT[i];
        }
        out_fname[in_len + ext_len] = 0x00;

        params.out_file_name = out_fname;
    }

    // compression/decompression routines call
    if (compress) {
        Deflate_encode(&params);
    }
    else {
        Deflate_decode(&params);
    }

    // realese the memory
    if (out_fname)
        free(out_fname);

    return 0;
}
