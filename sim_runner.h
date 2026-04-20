#ifndef SIM_RUNNER_H
#define SIM_RUNNER_H

#include "cache_calculations.h"
#include "cache_simulator.h"
#include "cmd_parser.h"
#include "memory_calculations.h"
#include "page_table.h"
#include "virtual_memory_simulator.h"

int runSimulation(Parameters *parameters, MemoryCalculationResults *memResults,
						CacheOutput *cacheResults,
						MemorySimulationResults *memSimResults,
						CacheSimulationResults *cacheSimResults, Process **Processes);

int freeProcesses(int numFiles, Process **processes);

#endif
