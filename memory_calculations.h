
#ifndef MEMORY_CALCULATIONS_H
#define MEMORY_CALCULATIONS_H

#include <stdint.h>

typedef struct {
    uint32_t physical_memory_mb;
    double os_percent;
    uint32_t num_trace_files;

    uint64_t number_physical_pages;
    uint64_t number_pages_for_system;
    uint32_t page_table_entry_size_bits;
    uint64_t total_ram_for_page_tables_bytes;
} MemoryCalculationResults;

int calculate(uint32_t physical_memory_mb,
              double os_percent,
              uint32_t num_trace_files,
              MemoryCalculationResults *results);

#endif
