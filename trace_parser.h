#ifndef TRACE_PARSER_H
#define TRACE_PARSER_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
	char operation;              /* 'R' or 'W' */
	uint32_t virAddr;            /* virtual address */
	int instructionComplete;     /* used for time quantum */
	int instructionSize;         /* EIP size, data accesses = 4 */
	int isInstruction;           /* 1 for EIP, 0 for src/dst */
} TraceEntry;

int getNextTraceEntry(FILE *file, TraceEntry *entry);

#endif
