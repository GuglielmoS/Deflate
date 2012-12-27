#include "deflate.h"

int main(int argc, char *argv[])
{
    if (argc == 2) {
        Deflate_encode(argv[1], "compressed_result");
    }
    else if (argc == 3) {
        Deflate_encode(argv[1], argv[2]);
    }

    return 0;
}
