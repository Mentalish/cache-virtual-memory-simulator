#ifndef PRINT_H
#define PRINT_H




typedef struct {
    int cacheSizeKB;
    int blockSize;
    int associativity;
    char replacementPolicy[10];
    int physicalMemoryMB;
    double percentUsed;
    int instructionsPerSlice;
} CacheInput;


typedef struct {
    int totalBlocks;
    int tagBits;
    int indexBits;
    int totalRows;
    int overheadSize;
    double implementationSizeKB;
    int implementationSizeBytes;
    double cost;
} CacheCalculated;




void printCalculationResults(
    int teamNumber,
    char traceFiles[][100],
    int numFiles,
    CacheInput cacheInput,
    CacheCalculated cacheCalc,
    MemoryCalculationResults memResults
);

#endif