#ifndef CPU_CACHE_H
#define CPU_CACHE_H

#include "cache_calculations.h"
#include "error.h"
#include "page_table.h"

typedef struct {
	int tag;
	int validbit;
	int dirtybit;
} CacheBlock;

typedef struct {
	int associativity;
	int rows;
	int tagSize;
	int indexSize;
	CacheBlock **cacheBlocks;
} Cache;

Cache *initCache(CacheInput cacheInputParameters, CacheOutput cacheCalcResults);
MissType readCache(Cache *cachePtr, int phyAddr, int *cacheCol);
int flushCache(Cache *cachePtr,
					PageTable *processPtr); /* flush all cache entries associated
													 with a process */
int freeCache(Cache *cachePtr);

#endif
