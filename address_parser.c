#include "address_parser.h"

int parseAddress(int phyAddr, int *tag, int *index, int *offset, int tagSize, int indexSize) {
    unsigned int address = (unsigned int)phyAddr;

    int offsetSize = 32 - tagSize - indexSize;

    unsigned int offsetMask = (1U << offsetSize) - 1U;
    unsigned int indexMask = (1U << indexSize) - 1U;

    *offset = address & offsetMask;
    *index = (address >> offsetSize) & indexMask;
    *tag = address >> (offsetSize + indexSize);

    return 0;
}
