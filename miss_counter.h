#ifndef MISS_COUNTER_H
#define MISS_COUNTER_H

#include "cpu_cache.h"
#include "cache_simulator.h"

void processMiss(MissType missType, char instType, int accessBytes,
                 CacheSimulationResults *results);

#endif
