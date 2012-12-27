#include "hash_table.h"

/**
 * Private functions.
 */
uint32_t Hash_Table_hash_key(Hash_Key key)
{
    /*
    uint32_t h, a = 31415, b = 27183;

    h = 0;
    for (uint32_t i = 0; i < 3; i++) {
        h = (a*h + key[i]) % HASH_TABLE_SIZE;
        a = a*b % (HASH_TABLE_SIZE-1);
    }

    return h;
*/
    // multiplicative hashing
    return (((uint32_t)(*key)&0xffffff)*0x6b43a9b5)>>19;
}

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
        ht[i] = NULL;
    }
}

/**
 * Put in the hash table the value 'val' identified by the key 'key'.
 */
void Hash_Table_put(Hash_Table ht, Hash_Key key, Hash_Value value)
{
    List *cur_list = &ht[Hash_Table_hash_key(key)];
    List_add(cur_list, value);
    List_keep_only_last_n_values(cur_list, HASH_TABLE_MAX_LIST_LEN);
}

/**
 * Return the list of values identified by 'key'.
 */
List Hash_Table_get(Hash_Table ht, Hash_Key key)
{
    return ht[Hash_Table_hash_key(key)];
}

void Hash_Table_destroy(Hash_Table ht)
{
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++) {
        if (ht[i] != NULL) {
            List_destroy(&ht[i]);
        }
    }
}
