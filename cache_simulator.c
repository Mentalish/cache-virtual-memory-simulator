#include "cache_simulator.h"
#include "address_parser.h"
#include "cpu_cache.h"
#include "error.h"

MissType runCacheSimulation(Cache *cachePtr, CacheOutput *cacheParameters,
									 CacheSimulationResults *results, int phyAddr,
									 char instType) {
	MissType missType;
	int cacheCol = 0;
	int tag;
	int index;
	int offset;

	parseAddress(phyAddr, &tag, &index, &offset, cacheParameters->tag_size,
					 cacheParameters->index_size);
	missType = readCache(cachePtr, phyAddr, &cacheCol);
	if (instType == 'R') {
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
