#include "virtual_memory_simulator.h"
#include "page_table.h"
#include "trace_parser.h"
#include <stdlib.h>

#define PAGE_OFFSET_BITS 12
#define PAGE_OFFSET_MASK 0xFFF
#define INITIAL_EVICT_PROCESS 0

static int findVictimProcess(Process **processes, int *finishedArray,
									  int numProcesses, int startIndex) {
	int checked = 0;
	int idx = startIndex;

	while (checked < numProcesses) {
		if (!finishedArray[idx] && processes[idx] != NULL &&
			 processes[idx]->processPageTable != NULL &&
			 processes[idx]->processPageTable->numPages > 0) {
			return idx;
		}

		idx = (idx + 1) % numProcesses;
		checked++;
	}

	return -1;
}

int findAnyInvalidPhyAddr(Process **processes, int *finishedArray,
								  int numProcesses) {
	int i;
	for (i = 0; i < numProcesses; i++) {
		if (finishedArray[i] && processes[i] != NULL) {
			PageTable *pageTablePtr = processes[i]->processPageTable;

			PageTableEntry *oldPage;
			oldPage = popPage(pageTablePtr);
			if (oldPage != NULL) {
				int addr = oldPage->phyAddr;
				free(oldPage);
				return addr;
			}
		}
	}

	return -1;
}

int countValidPages(Process **processes, int *finishedArray, int numProcesses) {
	int i;
	int validPages = 0;
	for (i = 0; i < numProcesses; i++) {
		if (!finishedArray[i] && processes[i] != NULL) {
			validPages += processes[i]->processPageTable->numPagesValid;
		}
	}

	return validPages;
}

int runVirtualMemorySimulation(Process **processes,
										 MemoryCalculationResults *pgTableParameters,
										 int timeSlice,
										 MemorySimulationResults *results) {
	int numProcesses;
	int *finishedArray;
	int finishedCount;
	int nextEvictProcess;

	unsigned long long totalPhysicalPages;
	unsigned long long systemPages;
	unsigned long long pagesAvailableToUser;
	unsigned long long freePagesRemaining;
	unsigned long long nextFreePhysicalPage;
	if (processes == NULL || pgTableParameters == NULL || results == NULL) {
		return 0;
	}

	numProcesses = pgTableParameters->num_trace_files;

	if (numProcesses <= 0) {
		return 0;
	}

	finishedArray = calloc(numProcesses, sizeof(int));
	if (finishedArray == NULL) {
		return 0;
	}

	totalPhysicalPages = pgTableParameters->number_physical_pages;
	systemPages = pgTableParameters->number_pages_for_system;
	pagesAvailableToUser = totalPhysicalPages - systemPages;
	freePagesRemaining = pagesAvailableToUser;
	nextFreePhysicalPage = systemPages;
	int physicalPageNumber = (int)nextFreePhysicalPage;
	nextEvictProcess = INITIAL_EVICT_PROCESS;
	finishedCount = 0;

	results->physicalPagesUsedBySystem = systemPages;
	results->pagesAvaibletoUser = pagesAvailableToUser;
	results->virtualPagesMapped = 0;
	results->pageHits = 0;
	results->pagesFromFree = 0;
	results->pageFaults = 0;

	while (finishedCount < numProcesses) {
		int processIndex;

		for (processIndex = 0; processIndex < numProcesses; processIndex++) {
			Process *currentProcess;
			PageTable *currentTable;
			int instructionsExecutedThisSlice;

			if (finishedArray[processIndex]) {
				continue;
			}

			currentProcess = processes[processIndex];
			if (currentProcess == NULL ||
				 currentProcess->processPageTable == NULL) {
				finishedArray[processIndex] = 1;

				finishedCount++;
				continue;
			}

			currentTable = currentProcess->processPageTable;
			instructionsExecutedThisSlice = 0;

			while (timeSlice == -1 || instructionsExecutedThisSlice < timeSlice) {
				TraceEntry entry;
				int virtualPageNumber;
				int offset;
				PageTableEntry *entryPage;

				if (!getNextTraceEntry(currentProcess->tracefile, &entry)) {
					//freePagesRemaining +=
						 //(unsigned long long)currentTable->numPagesValid;
					finishedArray[processIndex] = 1;
					finishedCount++;
					break;
				}

				virtualPageNumber = (int)(entry.virAddr >> PAGE_OFFSET_BITS);
				offset = (int)(entry.virAddr & PAGE_OFFSET_MASK);

				entryPage = searchPageByVir(currentTable, virtualPageNumber);

				if (entryPage != NULL) {
					unsigned int physicalAddress;

					results->pageHits++;

					physicalPageNumber = entryPage->phyAddr;
					physicalAddress =
						 ((unsigned int)physicalPageNumber << PAGE_OFFSET_BITS) |
						 (unsigned int)offset;
					(void)physicalAddress;
				} else {
					results->virtualPagesMapped++;

					if (freePagesRemaining > 0) {

						if (nextFreePhysicalPage < totalPhysicalPages) {
							physicalPageNumber = (int)nextFreePhysicalPage;
							nextFreePhysicalPage++;
						} else {
							physicalPageNumber = findAnyInvalidPhyAddr(
								 processes, finishedArray, numProcesses);
						}

						addPage(virtualPageNumber, physicalPageNumber, currentTable);
						results->pagesFromFree++;
						freePagesRemaining--;
						processes[processIndex]->numPagesAtTermination++;
					} else {
						int victimProcessIndex;
						Process *victimProcess;
						PageTable *victimTable;
						results->pageFaults++;
						victimProcessIndex = findVictimProcess(
							 processes, finishedArray, numProcesses, nextEvictProcess);

						if (victimProcessIndex < 0) {
							free(finishedArray);
							return 0;
						}

						victimProcess = processes[victimProcessIndex];
						victimTable = victimProcess->processPageTable;

						PageTableEntry *newPage = findFirstValidPage(victimTable);
						if (newPage != NULL) {
							physicalPageNumber =
								 newPage->phyAddr; // find first valid page

							invalidatePagebyPhyAddr(physicalPageNumber, victimTable);
							addPage(virtualPageNumber, physicalPageNumber,
									  currentTable);
							processes[processIndex]->numPagesAtTermination++;
						}

						nextEvictProcess = (victimProcessIndex + 1) % numProcesses;
					}
				}

				if (entry.instructionComplete) {
					instructionsExecutedThisSlice++;
				}
			}
		}
	}

	free(finishedArray);
	return 1;
}
