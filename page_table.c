#include <stdlib.h>
#include <stdio.h>
#include "page_table.h"

Process* InitProcessPageTable(int initialSize, int maxCapacity, FILE* traceFile) {
   Process* processPtr = malloc(sizeof(Process));

   processPtr->tracefile = traceFile;
   processPtr->processPageTable = malloc(sizeof(PageTable));
   processPtr->processPageTable->maxCapacity = maxCapacity;
   processPtr->processPageTable->capacity = initialSize;
   processPtr->processPageTable->pages = malloc(sizeof(PageTableEntry) * initialSize);

   return processPtr;
}
