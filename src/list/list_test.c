#include "list.h"

#include <stdio.h>

int main(int argc, char *argv[])
{
    List l = NULL;

    for (size_t i = 0; i < 10; i++) {
        List_add(&l, i);
    }

    List tmp = l;
    while (tmp != NULL) {
        printf("%d ", (int)(tmp->value));
        tmp = tmp->next;
    }
    putchar('\n');

    return 0;
}
