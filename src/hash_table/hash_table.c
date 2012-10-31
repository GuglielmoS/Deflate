#include "hash_table.h"

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
 * Put in the hash table the value 'val' identified by the key 'key'.
 */
void Hash_Table_put(Hash_Table ht, Hash_Key key, Hash_Value val)
{
    ht[Hash_Table_hash_key(key)] = val;
}

/**
 * Return the value identified by 'key'.
 */
Hash_Value Hash_Table_get(Hash_Table ht, Hash_Key key)
{
    return ht[Hash_Table_hash_key(key)];
}
