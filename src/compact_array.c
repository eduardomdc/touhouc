#include "compact_array.h"
#include <string.h>

CompactArray makeCompactArray(void* array, unsigned int itemBytes, unsigned int length){
    CompactArray compact;
    compact.array = array;
    compact.itemBytes = itemBytes;
    compact.length = length;
    compact.freeIndex = 0;
    return compact;
}

void compactAddItem(CompactArray* compactArray, void* item){
    void* freeSlot = compactArray->array+compactArray->itemBytes*compactArray->freeIndex;
    memcpy(freeSlot, item, compactArray->itemBytes);
}