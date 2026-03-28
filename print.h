#ifndef PRINT_H
#define PRINT_H




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
);

#endif