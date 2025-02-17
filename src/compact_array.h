#ifndef COMPACT_ARRAY_H
#define COMPACT_ARRAY_H

// Generic structure and methods for compact arrays
typedef struct CompactArray {
    void* array;
    unsigned int itemBytes; // how many bytes is one item
    unsigned int length; // maximum index of array
    unsigned int freeIndex; // first index that is free of items
} CompactArray;

CompactArray makeCompactArray(void* array, unsigned int itemBytes, unsigned int length);
int compactAddItem(CompactArray* compactArray, void* item);
void compactRemoveItem(CompactArray* compactArray, unsigned int index);

#endif