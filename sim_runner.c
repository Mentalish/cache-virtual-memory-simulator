#include <stdlib.h>

#include "cache_calculations.h"
#include "cache_simulator.h"
#include "page_table.h"
#include "sim_runner.h"
#include "trace_parser.h"
#include "virtual_memory_simulator.h"

#define INITIAL_EVICT_PROCESS 0

int runSimulation(Parameters *parameters, MemoryCalculationResults *memResults,
						CacheOutput *cacheResults,
						MemorySimulationResults *memSimResults,
						CacheSimulationResults *cacheSimResults,
						Process **processes) {

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

	// simulation loop
	MemoryState state = {0};
	int numProcesses = parameters->files.numFiles;
	int instructionsExecutedThisSlice;
	int timeSlice = parameters->timeSlice;

	state.finishedArray = calloc(numProcesses, sizeof(int));
	if (state.finishedArray == NULL) {
		return 0;
	}

	state.totalPhysicalPages = memResults->number_physical_pages;
	state.systemPages = memResults->number_pages_for_system;
	state.pagesAvailableToUser = state.totalPhysicalPages - state.systemPages;
	state.freePagesRemaining = state.pagesAvailableToUser;
	state.nextFreePhysicalPage = state.systemPages;
	state.nextEvictProcess = INITIAL_EVICT_PROCESS;
	state.finishedCount = 0;

	memSimResults->physicalPagesUsedBySystem = state.systemPages;
	memSimResults->pagesAvaibletoUser = state.pagesAvailableToUser;
	memSimResults->virtualPagesMapped = 0;
	memSimResults->pageHits = 0;
	memSimResults->pagesFromFree = 0;
	memSimResults->pageFaults = 0;

	if (processes == NULL || cacheResults == NULL || memSimResults == NULL) {
		return 1;
	}

	numProcesses = memResults->num_trace_files;

	if (numProcesses <= 0) {
		return 0;
	}

	while (state.finishedCount < numProcesses) {
		int processIndex;
		PageTable *currentTable;
      PageTableEntry *effectedPage;

		for (processIndex = 0; processIndex < numProcesses; processIndex++) {
			Process *currentProcess;
			currentProcess = processes[processIndex];

			if (currentProcess == NULL ||
				 currentProcess->processPageTable == NULL) {
				state.finishedCount++;
				continue;
			}

			currentTable = currentProcess->processPageTable;
			instructionsExecutedThisSlice = 0;

			while (timeSlice == -1 || instructionsExecutedThisSlice < timeSlice) {
				TraceEntry entry;
				MemoryReturnStatus memStatus;

				// get next instruction
				if (!getNextTraceEntry(currentProcess->tracefile, &entry)) {
					state.freePagesRemaining +=
						 (unsigned long long)currentTable->numPages;
					state.finishedArray[processIndex] = 1;
					state.finishedCount++;
					break;
				}

				memStatus = runVirtualMemorySimulation(
					 processes, processIndex, memResults, parameters->timeSlice,
					 memSimResults, &state, entry, numProcesses);

				switch (memStatus) {
				case PROC_SKIP:
					continue;
					break;
				case PROC_FINISHED: // invalidate cache entries
					break;
				case ERR:
					return 1;
					break;
				case SUCCESS:
					// do nothing
					break;
				}

				if (entry.instructionComplete) {
					instructionsExecutedThisSlice++;
				}
			}
		}
	}
	free(state.finishedArray);
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
