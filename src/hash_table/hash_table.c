#include "hash_table.h"

#include <string.h>

/**
 * Private functions.
 */
size_t Hash_Table_hash_key(Hash_Key key)
{
    size_t h, a = 31415, b = 27183;

    h = 0;
    for (size_t i = 0; i < 3; i++) {
        h = (a*h + key[i]) % HASH_TABLE_SIZE;
        a = a*b % (HASH_TABLE_SIZE-1);
    }

    return h;
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
    List_add(&ht[Hash_Table_hash_key(key)], value);
}

/**
 * Return the list of values identified by 'key'.
 */
List Hash_Table_get(Hash_Table ht, Hash_Key key)
{
    return ht[Hash_Table_hash_key(key)];
}
