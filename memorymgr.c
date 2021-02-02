#include "memorymgr.h"
#include <stdio.h>
#include <stdlib.h>

int* fB;
int* lB;
int heapSize;

int isAllocated(int *p){
    return p == NULL || *p % 2 == 0 ? 0 : 1;
}  

int *nextBlock(int *p){
    return p != NULL && p + (*p / 4) < lB ? p + (*p / 4) : NULL;
}

int *firstBlock(){
    return fB;
}

int *lastBlock(){
    return lB;
}


void initmemory(int size){
    size += (size % 4) + 12;
    size += size % 8;

    int* heap = (int*) malloc(size);

    fB = heap + 1;
    lB = heap + ((size / 4) - 1);

    *(fB) = heapSize = size - 8;
    *(lB) = 5;
}

void *myalloc(int length){
    length += (length % 4) + 4;
    length += length % 8;

    if (length > heapSize)
        return NULL;

    int* openBlock = fB;

    while (isAllocated(openBlock) || *openBlock < length){
        openBlock = nextBlock(openBlock);

        if (openBlock == NULL)
            return NULL;
    }

    *(openBlock + (length / 4)) = *openBlock - length;

    *openBlock = length + 1;
    return openBlock + 1;
}

void myfree(void *ptr){
    int* pointer = ((int*) ptr) - 1;
    if (isAllocated(pointer))
        (*pointer)--;
}

void coalesce(){
    int* currentBlock = fB;

    while (nextBlock(currentBlock) != NULL){
        if (!isAllocated(currentBlock) && !isAllocated(nextBlock(currentBlock)))
            *currentBlock += *nextBlock(currentBlock);
        else
            currentBlock = nextBlock(currentBlock);
    }
}

void printallocation(){
    int count = 0;
    int* currentBlock = fB;

     if (isAllocated(currentBlock))
        printf("Block %d: size %2d --------> allocated\n", count, *currentBlock - 1);
    else
        printf("Block %d: size %2d --------> unallocated\n", count, *currentBlock);

    while (nextBlock(currentBlock) != NULL){
        currentBlock = nextBlock(currentBlock);
        count++;

        if (isAllocated(currentBlock))
            printf("Block %d: size %2d --------> allocated\n", count, *currentBlock - 1);
        else
            printf("Block %d: size %2d --------> unallocated\n", count, *currentBlock);
    }

    printf("\n");
}
