#include "compact_array.h"
#include <string.h>
#include <stdio.h>

CompactArray makeCompactArray(void* array, unsigned int itemBytes, unsigned int length){
    CompactArray compact;
    compact.array = array;
    compact.itemBytes = itemBytes;
    compact.length = length;
    compact.freeIndex = 0;
    return compact;
}

int compactAddItem(CompactArray* compactArray, void* item){
    if (compactArray->freeIndex >= compactArray->length){
        return 1;
    }
    void* freeSlot = compactArray->array+compactArray->itemBytes*compactArray->freeIndex;
    memcpy(freeSlot, item, compactArray->itemBytes);
    compactArray->freeIndex++;
    return 0;
}

void compactRemoveItem(CompactArray* compactArray, unsigned int index){
    if (index == compactArray->freeIndex-1){
        if (compactArray->freeIndex > 0){
            compactArray->freeIndex--;
        }
        return;
    }
    void* lastSlot = compactArray->array+compactArray->itemBytes*(compactArray->freeIndex-1);
    void* removedSlot = compactArray->array+compactArray->itemBytes*index;
    memcpy(removedSlot, lastSlot, compactArray->itemBytes);
    if (compactArray->freeIndex > 0){
        compactArray->freeIndex--;
    }
}