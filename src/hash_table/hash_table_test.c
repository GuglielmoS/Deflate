#include <stdio.h>

#include "hash_table.h"

int main(int argc, char *argv[])
{
    Hash_Table ht;

    Hash_Table_init(ht);

    const char *keys[] = {"abc", "bac", "cab", "def", "coj", "boh", "vol"};

    for (size_t i = 0; i < 7; i++) {
        Hash_Table_put(ht, (uint8_t*)keys[i], i+1);
        printf("PUT(\"%s\", %d)\n", keys[i], (int)(i+1));
    }
    putchar('\n');

    for (size_t i = 0; i < 7; i++) {
        List l = Hash_Table_get(ht, (uint8_t*)keys[i]);

        printf("GET(\"%s\") => ", keys[i]);

        List tmp = l;
        while (tmp != NULL) {
            printf("%d ", (int)(tmp->value));
            tmp = tmp->next;
        }
        putchar('\n');

    }
    putchar('\n');

    return 0;
}
