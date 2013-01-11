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

#define HASH_TABLE_SIZE 8192

#define HASH_TABLE_MAX_LIST_LEN 20

// hash function
#define GET_HASH(key) ((((uint32_t)(*key)&0xffffff)*0x6b43a9b5)>>19)

/** USEFUL MACROS **/
#define HTABLE_GET(ht,k) ht[GET_HASH(k)].first

typedef uint8_t Hash_Value;
typedef uint8_t Hash_Key[3];

typedef Limited_List Hash_Table[HASH_TABLE_SIZE];

/** Helpers **/
void Hash_Table_init(Hash_Table ht);

/** Hash_Table accessors **/
void Hash_Table_put(Hash_Table ht, Hash_Key key, Hash_Value value);
List Hash_Table_get(Hash_Table ht, Hash_Key key);

/** Hash_Table destructors **/
void Hash_Table_reset(Hash_Table ht);

#endif /* __HASH_TABLE__ */
