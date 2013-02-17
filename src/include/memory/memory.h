/***
* Purpose:
*       This file include the function declarations 
*       of base operations for memory management
*
*******************************************************************************/

#include <windows.h>

#define ERROR_NULL_HANDLER                100
#define ERROR_INVALID_HEAP                101
#define ERROR_MEM_DOES_NOT_BELONG_TO_HEAP 102
#define SYSTEM_ERROR_DESTROY_HEAP         2

#define ALLOC_SIZE                        10000

HANDLE CreateHeap(int size);
int DeleteHeap(HANDLE heapHandler);

char* StackAlloc(int n);
void StackFree(char* memory);

void* MemoryAlloc(unsigned int size);
void* MemoryAlloc_Heap(HANDLE heapHandler, unsigned int size);

void* MemoryRealloc(void* memory, unsigned int newSize);
void* MemoryRealloc_Heap(HANDLE heapHandler, void* memory, unsigned int newSize);

int MemoryFree(void* memory);
int MemoryFree_Heap(HANDLE heapHandler, void* memory);
void MemorySet(char* memory, char symbol, int count);