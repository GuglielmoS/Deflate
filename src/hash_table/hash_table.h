#include <stddef.h>
#include <stdint.h>

#ifndef __HASH_TABLE__
#define __HASH_TABLE__

/**
 * Defines the Hash_Table data structure for storing
 * list of integer.
 */

#define HASH_TABLE_SIZE 31

typedef uint8_t Hash_Value;
typedef uint8_t Hash_Key[3];

typedef Hash_Value Hash_Table[HASH_TABLE_SIZE];

/** Hash_Table accessors **/
void       Hash_Table_put(Hash_Table ht, Hash_Key key, Hash_Value val);
Hash_Value Hash_Table_get(Hash_Table ht, Hash_Key key);


#endif /* __HASH_TABLE__ */
