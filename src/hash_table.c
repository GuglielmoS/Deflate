#include "hash_table.h"

/**
 * Sets to zero the Hash_Table content.
 */
void Hash_Table_init(Hash_Table ht, size_t max_list_size)
{
    // setting all pointers to NULL
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        ht[i].values = NULL;
        ht[i].max_size = max_list_size;
    }
}

/**
 * Deletes all the Hash_Table elements.
 */
void Hash_Table_reset(Hash_Table ht)
{
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        Limited_List_destroy(&ht[i]);
    }
}
