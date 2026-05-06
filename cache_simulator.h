#ifndef CACHE_SIMULATOR_H
#define CACHE_SIMULATOR_H

#include "cache_calculations.h"
#include "cmd_parser.h"
#include "cpu_cache.h"
#include "error.h"

typedef struct {
	int totalAccesses;
	int totalAddresses;
	int instructionBytes;
	int destBytes;
	int compulsoryMisses;
	int conflictMisses;
	int capacityMisses;
	int totalCycles;
	int totalInstructions;
} CacheSimulationResults;

MissType runCacheSimulation(Cache *cachePtr, CacheOutput *cacheParameters,
									 CacheSimulationResults *results,
									 unsigned int phyAddr, char instType,
									 int instSize, int isInstruction,
									 ReplacementPolicy policy, int blockSize);

#endif
