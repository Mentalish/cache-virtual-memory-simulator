#include "page_table.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

Process *InitProcessPageTable(int initialSize, int maxCapacity, FILE *traceFile,
										char *name) {
	Process *processPtr = malloc(sizeof(Process));

	/*check if allocation failed*/
	if (processPtr == NULL) {
		return NULL;
	}

	processPtr->tracefile = traceFile;
	processPtr->fileName = name;
	processPtr->numPagesAtTermination = 0;
	processPtr->processPageTable = malloc(sizeof(PageTable));

	/*check if allocation failed*/
	if (processPtr->processPageTable == NULL) {
		free(processPtr);
		return NULL;
	}

	processPtr->processPageTable->numPages = 0;
	processPtr->processPageTable->maxCapacity = maxCapacity;
	processPtr->processPageTable->capacity = initialSize;
	processPtr->processPageTable->pages = NULL;

	return processPtr;
}

int shiftPageTable(PageTable *pageTablePtr, int index) {
	PageTableEntry temp = pageTablePtr->pages[index];
	int i;

	if (index < 0 || index >= pageTablePtr->numPages + 1) {
		return 1;
	}
	for (i = index; i < pageTablePtr->numPages - 1; i++) {
		pageTablePtr->pages[i] = pageTablePtr->pages[i + 1];
	}

	pageTablePtr->pages[pageTablePtr->numPages - 1] = temp;
	return 0;
}

int addPage(int virAddr, int phyAddr, PageTable *pageTablePtr) {
	if (pageTablePtr == NULL) {
		return 1;
	}

	if (pageTablePtr->numPages >= pageTablePtr->maxCapacity) {
		return 1; /* capacity miss */
	}

	PageTableEntry *newPage = malloc(sizeof(PageTableEntry));

	newPage->phyAddr = phyAddr;
	newPage->virAddr = virAddr;
	newPage->validBit = true;
	newPage->dirtyBit = false;
	newPage->next = NULL;

	if (pageTablePtr->pages == NULL) { /* first page*/
		pageTablePtr->pages = newPage;
	} else {
		PageTableEntry *currPage = pageTablePtr->pages;

		while (currPage->next != NULL) {
			if (currPage->phyAddr == phyAddr) {
				currPage->validBit = true;
				free(newPage);
				return 1;
			}
			currPage = currPage->next;
		}

		if (currPage->phyAddr == phyAddr) {
			pageTablePtr->numPagesValid++;
			currPage->validBit = true;
			free(newPage);
			return 1;
		}

		currPage->next = newPage;
	}

	pageTablePtr->numPages++;
	pageTablePtr->numPagesValid++;
	return 0;
}
bool invalidatePagebyPhyAddr(int phyAddr, PageTable *pageTablePtr) {

	if (pageTablePtr->pages == NULL) {
		return false; /* no page to invalidate */
	}

	PageTableEntry *currPage = pageTablePtr->pages;

	while (currPage != NULL) {
		if (currPage->phyAddr == phyAddr) {
			currPage->validBit = false;
			pageTablePtr->numPagesValid--;
			return true;
		}
		currPage = currPage->next;
	}

	return false;
}
int freeProcessPageTable(Process *processPtr) {
	if (processPtr == NULL) {
		return 0;
	}

	/*free page table*/
	PageTableEntry *currPage = processPtr->processPageTable->pages;
	PageTableEntry *next;
	while (currPage != NULL) {
		next = currPage->next;
		free(currPage);

		currPage = next;
	}

	free(processPtr);
	return 0;
}

PageTableEntry *searchPageByVir(PageTable *pageTablePtr, int virAddr) {
	if (pageTablePtr == NULL) {
		return NULL;
	}

	PageTableEntry *currPage = pageTablePtr->pages;

	while (currPage != NULL) {
		if (currPage->virAddr == virAddr && currPage->validBit) {
			return currPage;
		}

		currPage = currPage->next;
	}

	return NULL; /* not found */
}

PageTableEntry *searchPageByPhyAddr(PageTable *pageTablePtr, int phyAddr) {
	if (pageTablePtr == NULL) {
		return NULL;
	}

	PageTableEntry *currPage = pageTablePtr->pages;

	while (currPage != NULL) {
		if (currPage->phyAddr == phyAddr) {
			return currPage;
		}

		currPage = currPage->next;
	}

	return NULL; /* not found */
}

PageTableEntry *findFirstValidPage(PageTable *pageTablePtr) {
	if (pageTablePtr == NULL) {
		return NULL;
	}

	PageTableEntry *currPage = pageTablePtr->pages;

	while (currPage != NULL) {
		if (currPage->validBit == true) {
			return currPage;
		}

		currPage = currPage->next;
	}

	return NULL; /* not found */
}

PageTableEntry *popPage(PageTable *pageTablePtr) {
	if (pageTablePtr->pages == NULL) {
		return NULL;
	}

	PageTableEntry *oldPage = pageTablePtr->pages;

	pageTablePtr->pages = pageTablePtr->pages->next;

	return oldPage;
}
