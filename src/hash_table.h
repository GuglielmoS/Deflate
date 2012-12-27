#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "list.h"

/**
 * Defines the Hash_Table data structure for storing
 * list of integer.
 */

#define HASH_TABLE_SIZE 24593
//3079
//6151
//12289

#define HASH_TABLE_MAX_LIST_LEN 10

typedef uint8_t Hash_Value;
typedef uint8_t Hash_Key[3];

typedef List Hash_Table[HASH_TABLE_SIZE];

/** Helpers **/
void Hash_Table_init(Hash_Table ht);

/** Hash_Table accessors **/
void Hash_Table_put(Hash_Table ht, Hash_Key key, Hash_Value value);
List Hash_Table_get(Hash_Table ht, Hash_Key key);

/** Hash_Table destructors **/
void Hash_Table_destroy(Hash_Table ht);

#endif /* __HASH_TABLE__ */
