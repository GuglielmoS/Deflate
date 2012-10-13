#ifndef __LZ__
#define __LZ__

#include <stdio.h>

#include "lz_stream.h"

LZ_Stream* LZ_encode(const char *buffer); 
LZ_Stream* LZ_decode(const LZ_Stream *s);

#endif /* __LZ__ */

