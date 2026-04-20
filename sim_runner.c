#include <stdio.h>
#include <stdlib.h>

#include "cache_calculations.h"
#include "cache_simulator.h"
#include "page_table.h"
#include "sim_runner.h"
#include "virtual_memory_simulator.h"

int runSimulation(Parameters *parameters, MemoryCalculationResults *memResults,
						CacheOutput *cacheResults,
						MemorySimulationResults *memSimResults, CacheSimulationResults * cacheSimResults, Process **processes) {

	int i;

	// processes = malloc(sizeof(Process *) * parameters->files.numFiles);
	if (processes == NULL) {
		return 1;
	}

	for (i = 0; i < parameters->files.numFiles; i++) {
		processes[i] =
			 InitProcessPageTable(10,
										 memResults->number_physical_pages -
											  memResults->number_pages_for_system,
										 parameters->files.files[i].filePtr,
										 parameters->files.files[i].fileName);

		if (processes[i] == NULL) {
			while (--i >= 0) {
				freeProcessPageTable(processes[i]);
			}
			// free(processes);
			return 1;
		}
	}

	runVirtualMemorySimulation(processes, memResults, parameters->timeSlice,
										memSimResults);

	// Prep processes for cache simulation
	for (i = 0; parameters->files.numFiles; i++) {
		if (parameters->files.files[i].filePtr != NULL) {
			rewind(parameters->files.files[i].filePtr);
		}
	}

	runCacheSimulation(processes, cacheResults, parameters->timeSlice,
							 cacheSimResults);

		 return 0;
}

int freeProcesses(int numFiles, Process **processes) {
	int i;
	for (i = 0; i < numFiles; i++) {
		freeProcessPageTable(processes[i]);
	}

	free(processes);
	return 0;
}
