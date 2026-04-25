#include "cache_simulator.h"
#include "cpu_cache.h"
#include "error.h"

MissType runCacheSimulation(Cache *cachePtr, CacheOutput *cacheParameters,
									 CacheSimulationResults *results, int phyAddr,
									 char instType) {
	MissType missType;
	int cacheCol = 0;
	CacheBlock *affectedBlock;

	if (instType == 'R') {
		missType = readCache(cachePtr, phyAddr, &cacheCol);

		switch (missType) {
		case CONFLICT:
			break;
		case COMPULSORY:
			break;
		case CAPACITY:
			break;
		case NO_MISS:
			break;
		}

	} else {
		missType = writeCache(cachePtr, phyAddr);

		switch (missType) {
		case CONFLICT:
			break;
		case COMPULSORY:
			break;
		case CAPACITY:
			break;
		case NO_MISS:
			break;
		}
	}
	return 0;
}
