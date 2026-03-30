#ifndef PRINT_H
#define PRINT_H
#include "cache_calculations.h"
#include "cmd_parser.h"
#include "memory_calculations.h"

void printCalculationResults(int teamNumber, Parameters *params,
									  CacheOutput cacheCalc,
									  MemoryCalculationResults memResults);

#endif
