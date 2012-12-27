#include "util.h"

void die_error(char *msg)
{
    fprintf(stderr, "%s", msg);
    exit(EXIT_FAILURE);
}
