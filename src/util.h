#ifndef __UTIL__
#define __UTIL__

#include <stdio.h>
#include <stdlib.h>

/**
 * Prints msg on the stderr and exit from the process
 * by using the exit function provided by stdlib.h.
 */
void die_error(char *msg);

#endif /* __UTIL__ */
