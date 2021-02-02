#include "memorymgr.h"
#include "gc.h"
#include <stdio.h>
#include <stdlib.h>

int* heapFirstBlock;
int* heapLastBlock;

void gc();
void search(int* current, int* topBound, int* bottomBound);
void *stackBottom();

void *stackBottom()
{
  unsigned long bottom;
  FILE *statfp = fopen("/proc/self/stat", "r");
  fscanf(statfp,
    "%*d %*s %*c %*d %*d %*d %*d %*d %*u "
    "%*u %*u %*u %*u %*u %*u %*d %*d "
    "%*d %*d %*d %*d %*u %*u %*d "
    "%*u %*u %*u %lu", &bottom);
    fclose(statfp);
    return (void *) bottom;
  }
  
void gc()
{
    heapFirstBlock = firstBlock();
    heapLastBlock = lastBlock();

    int top = 0;
    int* topPointer = &top;
    int* bottomPointer = (int*) stackBottom();
    int* currentPointer = topPointer;

    while (currentPointer < bottomPointer)
    {
        search((int*) *currentPointer, heapLastBlock, heapFirstBlock);
        currentPointer = currentPointer + 1;
    }

    int* sweeper = heapFirstBlock;
    while (sweeper != NULL && sweeper < heapLastBlock)
    {
        if (*sweeper % 4 == 3)
            *sweeper = *sweeper - 2;
        else if (isAllocated(sweeper))
            *sweeper = *sweeper - 1;

        sweeper = nextBlock(sweeper);
    }

    coalesce();
}


void search(int* current, int* topBound, int* bottomBound)
{
    if ((current < heapLastBlock && current > heapFirstBlock) && (current < topBound && current > bottomBound))
    {
        int* inBlockBottomBound = heapFirstBlock;
        int* inBlockTopBound;

        while (inBlockBottomBound != NULL && inBlockBottomBound < heapLastBlock)
        {
            inBlockTopBound = nextBlock(inBlockBottomBound);

            if (current < inBlockTopBound && current > inBlockBottomBound && *inBlockBottomBound % 4 != 3)
            {
                *inBlockBottomBound = *inBlockBottomBound + 2;

                int* heapDrill = (int*) &inBlockBottomBound;
                while (heapDrill < inBlockTopBound)
                    search(heapDrill, inBlockBottomBound, inBlockTopBound);
            }
            else
                inBlockBottomBound = inBlockTopBound;
        }
    }
}
