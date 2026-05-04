#include "print.h"
#include "page_table.h"
#include <stdio.h>

#define MAX_VIRTUAL_PAGES 524288

void printHeader(int teamNumber) {
	printf("Cache Simulator - CS 3853 - Team %d\n\n", teamNumber);
}

void printTraceFiles(Parameters *params) {
	int i;

	printf("Trace File(s):\n");

	for (i = 0; i < params->files.numFiles; i++) {
		printf("        %s\n", params->files.files[i].fileName);
	}

	printf("\n");
}

void printCacheInput(Parameters *params) {
	const char *policy =
		 (params->replacementPolicy == RR) ? "Round Robin" : "Random";

	printf("***** Cache Input Parameters *****\n\n");
	printf("%-30s %d KB\n", "Cache Size:", params->cacheSize);
	printf("%-30s %d bytes\n", "Block Size:", params->blockSize);
	printf("%-30s %d\n", "Associativity:", params->associativity);
	printf("%-30s %s\n", "Replacement Policy:", policy);
	printf("%-30s %d MB\n", "Physical Memory:", params->physicalMemory);
	printf("%-30s %.1f%%\n",
			 "Percent Memory Used by System:",
			 (double)params->physicalMemoryOS);
	printf("%-30s %d\n\n",
			 "Instructions / Time Slice:",
			 params->timeSlice);
}

void printCacheCalculated(CacheOutput calc) {
	printf("***** Cache Calculated Values *****\n\n");
	printf("%-30s %llu\n", "Total # Blocks:", calc.total_blocks);
	printf("%-30s %u bits\n", "Tag Size:", calc.tag_size);
	printf("%-30s %u bits\n", "Index Size:", calc.index_size);
	printf("%-30s %llu\n", "Total # Rows:", calc.total_rows);
	printf("%-30s %llu bytes\n",
			 "Overhead Size:",
			 calc.overhead_size_bytes);
	printf("%-30s %.2f KB  (%llu bytes)\n",
			 "Implementation Memory Size:",
			 calc.implementation_memory_size_kb,
			 calc.implementation_memory_size_bytes);
	printf("%-30s $%.2f @ $0.07 per KB\n\n",
			 "Cost:",
			 calc.cost);
}

void printPhysicalMemory(MemoryCalculationResults mem) {
	printf("***** Physical Memory Calculated Values *****\n\n");
	printf("%-30s %llu\n",
			 "Number of Physical Pages:",
			 mem.number_physical_pages);
	printf("%-30s %llu\n",
			 "Number of Pages for System:",
			 mem.number_pages_for_system);
	printf("%-30s %u bits\n",
			 "Size of Page Table Entry:",
			 mem.page_table_entry_size_bits);
	printf("%-30s %llu bytes\n",
			 "Total RAM for Page Table(s):",
			 mem.total_ram_for_page_tables_bytes);
}

void printCalculationResults(int teamNumber, Parameters *params,
									  CacheOutput cacheCalc,
									  MemoryCalculationResults memResults) {
	printHeader(teamNumber);
	printTraceFiles(params);
	printCacheInput(params);
	printCacheCalculated(cacheCalc);
	printPhysicalMemory(memResults);
}

void printVirMemorySimulationResults(MemorySimulationResults simResults,
												 Process **processes,
												 int numProcesses) {
	int i;

	printf("\n\n***** VIRTUAL MEMORY SIMULATION RESULTS *****\n\n");
	printf("%-30s %llu\n",
			 "Physical Pages Used By SYSTEM:",
			 simResults.physicalPagesUsedBySystem);
	printf("%-30s %llu\n",
			 "Pages Available To User:",
			 simResults.pagesAvaibletoUser);
	printf("%-30s %llu\n",
			 "Virtual Pages Mapped:",
			 simResults.pageHits +
				  simResults.pagesFromFree +
				  simResults.pageFaults);

	printf("    ---------------------------------\n");
	printf("    %-26s %llu\n",
			 "Page Table Hits:",
			 simResults.pageHits);
	printf("    %-26s %llu\n",
			 "Pages From Free:",
			 simResults.pagesFromFree);
	printf("    %-26s %llu\n",
			 "Total Page Faults:",
			 simResults.pageFaults);

	printf("\n\nPage Table Usage Per Process:");
	printf("\n-----------------------------\n");

	for (i = 0; i < numProcesses; i++) {
		printf("\n[%d] %s:",
				 i,
				 processes[i]->fileName);
		printf("\n    Used Page Table Entries: %d (%.2f %%)",
				 processes[i]->numPagesAtTermination,
				 (processes[i]->numPagesAtTermination /
				  (float)MAX_VIRTUAL_PAGES) *
					  100);
		printf("\n    Page Table Wasted: %llu bytes",
				 2 * (unsigned long long)(MAX_VIRTUAL_PAGES -
												  processes[i]->numPagesAtTermination));
		printf("\n");
	}

	printf("\n");
}
void printCacheSimulationResults(CacheSimulationResults results, CacheOutput cacheCalc) {
    int totalMisses;
    int totalHits;
    double hitRate;
    double missRate;
    int unusedBlocks;
    double unusedKB;
    double wasteCost;
    double unusedPercent;

    totalMisses = results.compulsoryMisses +
                  results.conflictMisses +
                  results.capacityMisses;

    totalHits = results.totalAccesses - totalMisses;

    if (results.totalAccesses > 0) {
        hitRate = ((double)totalHits * 100.0) / results.totalAccesses;
        missRate = ((double)totalMisses * 100.0) / results.totalAccesses;
    } else {
        hitRate = 0.0;
        missRate = 0.0;
    }

    unusedBlocks = cacheCalc.total_blocks - results.compulsoryMisses;

    if (unusedBlocks < 0) {
        unusedBlocks = 0;
    }

    unusedKB = ((double)unusedBlocks *
               ((double)cacheCalc.implementation_memory_size_bytes /
                (double)cacheCalc.total_blocks)) / 1024.0;

    wasteCost = unusedKB * 0.07;

    if (cacheCalc.implementation_memory_size_kb > 0) {
        unusedPercent = (unusedKB / cacheCalc.implementation_memory_size_kb) * 100.0;
    } else {
        unusedPercent = 0.0;
    }

    printf("\nMILESTONE #3: - Cache Simulation Results\n\n");

    printf("***** CACHE SIMULATION RESULTS *****\n\n");

    printf("%-24s %d\n", "Total Cache Accesses:", results.totalAccesses);
    printf("--- Instruction Bytes:   %d\n", results.instructionBytes);
    printf("--- SrcDst Bytes:        %d\n", results.destBytes);

    printf("%-24s %d\n", "Cache Hits:", totalHits);
    printf("%-24s %d\n", "Cache Misses:", totalMisses);
    printf("--- Compulsory Misses:   %d\n", results.compulsoryMisses);
    printf("--- Conflict Misses:     %d\n", results.conflictMisses);
    printf("--- Capacity Misses:     %d\n", results.capacityMisses);

    printf("\n\n***** *****  CACHE HIT & MISS RATE:  ***** *****\n\n");

    printf("%-24s %.4f%%\n", "Hit  Rate:", hitRate);
    printf("%-24s %.4f%%\n", "Miss Rate:", missRate);

    printf("%-24s %.2f KB / %.2f KB = %.2f%%  Waste: $%.2f/chip\n",
           "Unused Cache Space:",
           unusedKB,
           cacheCalc.implementation_memory_size_kb,
           unusedPercent,
           wasteCost);

    printf("%-24s %d / %llu\n",
           "Unused Cache Blocks:",
           unusedBlocks,
           cacheCalc.total_blocks);
}
