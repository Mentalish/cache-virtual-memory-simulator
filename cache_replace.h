#ifndef CACHE_REPLACE
#define CACHE_REPLACE

#include "cpu_cache.h"

/* return index of cache replaced*/
int roundRobinReplace(Cache *cachePtr, int index, int tag, int offset);
int randomReplace(Cache *cachePtr, int index, int tag, int offset);

#endif
