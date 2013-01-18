#include "hash_table.h"

/**
 * Sets to zero the Hash_Table content.
 */
void Hash_Table_init(Hash_Table ht)
{
    // setting all pointers to NULL
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        Limited_List_init(&ht[i], HASH_TABLE_MAX_LIST_LEN);
    }
}

/**
 * Deletes all the Hash_Table elements.
 */
void Hash_Table_reset(Hash_Table ht)
{
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        if (ht[i].cur_size > 0) {
            Limited_List_destroy(&ht[i]);
        }
    }
}
