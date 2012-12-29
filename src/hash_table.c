#include "hash_table.h"

/**
 * Private functions.
 */

/**
 * Universal hashing function.
 *
 * Taken from "Algorithms in C" by R. Sedgewick
 *
uint32_t Hash_Table_hash_key(Hash_Key key)
{
    uint32_t h, a = 31415, b = 27183;

    h = 0;
    for (uint32_t i = 0; i < 3; i++) {
        h = (a*h + key[i]) % HASH_TABLE_SIZE;
        a = a*b % (HASH_TABLE_SIZE-1);
    }

    return h;
}
*/

/**
 * Public functions.
 */


/**
 * Set to zero the Hash_Table content. 
 */
void Hash_Table_init(Hash_Table ht)
{
    // setting all pointers to NULL
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        Limited_List_init(&ht[i], HASH_TABLE_MAX_LIST_LEN);
    }
}

/**
 * Put in the hash table the value 'val' identified by the key 'key'.
 */
void Hash_Table_put(Hash_Table ht, Hash_Key key, Hash_Value value)
{
    Limited_List_add(&ht[GET_HASH(key)], value);
}

void Hash_Table_reset(Hash_Table ht)
{
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        if (ht[i].cur_size > 0) {
            Limited_List_destroy(&ht[i]);
        }
    }
}
