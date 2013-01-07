#include "deflate.h"

int main(int argc, char *argv[])
{
    Deflate_Params params = {true}; // fast = true

    if (argc == 2) {
        Deflate_encode(argv[1], "compressed_result", &params);
    }
    else if (argc == 3) {
        Deflate_encode(argv[1], argv[2], &params);
    }

    return 0;
}
