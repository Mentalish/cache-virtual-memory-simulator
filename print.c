#include <stdio.h>
#include "print.h"



void printHeader(int teamNumber) {
    printf("Cache Simulator - CS 3853 - Team 12%d\n", teamNumber);
}

void printTraceFiles(char traceFiles[][100], int numFiles) {
    printf("Trace File(s):\n");
    for (int i = 0; i < numFiles; i++) {
        printf("%s\n", traceFiles[i]);
    }
}

void printCacheInput(
    int cacheSizeKB,
    int blockSize,
    int associativity,
    char *replacementPolicy,
    int physicalMemoryMB,
    double percentUsed,
    int instructionsPerSlice
) {
    printf("***** Cache Input Parameters *****\n");
    printf("Cache Size: %d KB\n", cacheSizeKB);
    printf("Block Size: %d bytes\n", blockSize);
    printf("Associativity: %d\n", associativity);
    printf("Replacement Policy: %s\n", replacementPolicy);
    printf("Physical Memory: %d MB\n", physicalMemoryMB);
    printf("Percent Memory Used by System: %.1f%%\n", percentUsed);
    printf("Instructions / Time Slice: %d\n", instructionsPerSlice);
}

void printCacheCalculated(
    int totalBlocks,
    int tagBits,
    int indexBits,
    int totalRows,
    int overheadSize,
    double implementationSizeKB,
    int implementationSizeBytes,
    double cost
) {
    printf("***** Cache Calculated Values *****\n");
    printf("Total # Blocks: %d\n", totalBlocks);
    printf("Tag Size: %d bits\n", tagBits);
    printf("Index Size: %d bits\n", indexBits);
    printf("Total # Rows: %d\n", totalRows);
    printf("Overhead Size: %d bytes\n", overheadSize);
    printf("Implementation Memory Size: %.2f KB (%d bytes)\n",
           implementationSizeKB, implementationSizeBytes);
    printf("Cost: $%.2f @ $0.07 per KB\n", cost);
}

void printPhysicalMemory(MemoryCalculationResults memResults) {
    printf("***** Physical Memory Calculated Values *****\n");
    printf("Number of Physical Pages: %llu\n",
           memResults.number_physical_pages);
    printf("Number of Pages for System: %llu\n",
           memResults.number_pages_for_system);
    printf("Size of Page Table Entry: %u bits\n",
           memResults.page_table_entry_size_bits);
    printf("Total RAM for Page Table(s): %llu bytes\n",
           memResults.total_ram_for_page_tables_bytes);
}



void printCalculationResults(
    int teamNumber,
    char traceFiles[][100],
    int numFiles,

    int cacheSizeKB,
    int blockSize,
    int associativity,
    char *replacementPolicy,
    int physicalMemoryMB,
    double percentUsed,
    int instructionsPerSlice,

    int totalBlocks,
    int tagBits,
    int indexBits,
    int totalRows,
    int overheadSize,
    double implementationSizeKB,
    int implementationSizeBytes,
    double cost,

    MemoryCalculationResults memResults
) {
    printHeader(teamNumber);
    printTraceFiles(traceFiles, numFiles);

    printCacheInput(
        cacheSizeKB,
        blockSize,
        associativity,
        replacementPolicy,
        physicalMemoryMB,
        percentUsed,
        instructionsPerSlice
    );

    printCacheCalculated(
        totalBlocks,
        tagBits,
        indexBits,
        totalRows,
        overheadSize,
        implementationSizeKB,
        implementationSizeBytes,
        cost
    );

    printPhysicalMemory(memResults);
}