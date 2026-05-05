#ifndef CACHE_REPLACE
#define CACHE_REPLACE

#include "cpu_cache.h"

/* return index of cache replaced*/
int roundRobinReplace(Cache *cachePtr, unsigned int index, unsigned int tag,
							 unsigned int offset);
int randomReplace(Cache *cachePtr, unsigned int index, unsigned int tag,
						unsigned int offset);

#endif
