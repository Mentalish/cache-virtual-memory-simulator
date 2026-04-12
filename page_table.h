#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include <stdbool.h>
#include <stdio.h>

#define MAX_VIRTUAL_PAGES 524288

typedef struct PageTableEntry {
	int phyAddr; /* physical page number */
	int virAddr; /* virtual page number */
	bool validBit;
	bool dirtyBit;
	struct PageTableEntry *prev;
	struct PageTableEntry *next;
} PageTableEntry;

typedef struct {
	int numPages;
	int capacity;
	int maxCapacity;
	int numPagesValid;
	PageTableEntry *pages;
} PageTable;

typedef struct {
	char *fileName;
	FILE *tracefile;
	int numPagesAtTermination;
	PageTable *processPageTable;
} Process;

Process *InitProcessPageTable(int initialSize, int maxCapacity, FILE *traceFile,
										char *name);
int insertPage(int virAddr, int phyAddr, int validBit, PageTable *pageTablePtr);
int addPage(int virAddr, int phyAddr, PageTable *pageTablePtr);
bool invalidatePagebyPhyAddr(int phyAddr, PageTable *pageTablePtr);
int freeProcessPageTable(Process *processPtr);
PageTableEntry *searchPageByVir(PageTable *pageTablePtr, int virAddr);
PageTableEntry *searchPageByPhyAddr(PageTable *pageTablePtr, int phyAddr);
PageTableEntry *findFirstValidPage(PageTable *pageTablePtr);
PageTableEntry *popPage(PageTable *pageTablePtr);

#endif
