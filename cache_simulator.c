#include "cache_simulator.h"
#include "trace_parser.h"

int runCacheSimulation(Process **processes, CacheOutput *cacheParameters,
							  int timeSlice, int numFiles,
							  CacheSimulationResults *results) {
	int numProcesses;
	int finishedCount = 0;
	int instructionsExecutedThisSlice = 0;
	if (processes == NULL || cacheParameters == NULL || results == NULL) {
		return 0;
	}

	numProcesses = numFiles;

	if (numProcesses <= 0) {
		return 0;
	}

	while (finishedCount < numProcesses) {
		int processIndex;

		for (processIndex = 0; processIndex < numProcesses; processIndex++) {
			Process *currentProcess;
			currentProcess = processes[processIndex];

			if (currentProcess == NULL ||
				 currentProcess->processPageTable == NULL) {
				finishedCount++;
				continue;
			}

			while (timeSlice == -1 || instructionsExecutedThisSlice < timeSlice) {
				TraceEntry entry;
				if (!getNextTraceEntry(currentProcess->tracefile, &entry)) {
					break;
				}
				instructionsExecutedThisSlice++;
			}
		}
	}

	return 0;
}
