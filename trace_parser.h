#ifndef TRACE_PARSER_H
#define TRACE_PARSER_H

#include <stdint.h>
#include <stdio.h>

typedef struct {
    char operation;              /* 'R' or 'W' */
    uint32_t virAddr;            /* virtual address */
    int instructionComplete;     
} TraceEntry;

int getNextTraceEntry(FILE *file, TraceEntry *entry);

#endif
