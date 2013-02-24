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

/* The minimal size of memory, ehich is taken from operational system.
 * This is an optimization. Taking memory from an operational system is 
 * an expensive operation. So we need to minimize frequency of those calls
 * This is used in our own implementation of malloc: LocalMalloc
 */
#define MIN_ALLOC                         1

/* the largest type for the current operational system */
typedef double Align;
/* We use union for correct calculating of appropriate size of memory
 * For example, if we need to allocate two arrays 
 * long ArrLong[10] and int ArrInt[10] with a possibility to recycle
 * memory. The easiest way to do this is to calculate memory according
 * to the most restrictive type. We need memory allocated for int ArrInt[10]
 * to be reused for long ArrLong[10]. We use union to keep the size of Header
 * fit for the most restrictive type in operational system
 * 10 * sizeof(Header) - this memory can be fit for char[10], int[10], long[10]
 * and easily recycled.
 * sizeof(union Header) = max{sizeof(union Header*)+sizeof(unsigned),sizeof(Align)}
 * 1. sizeof(Align) = sizeof(double) = 8
 * 2. sizeof(pointer) = 4;
 * 3. sizeof(unsigned) = 4;
 * 4. sizeof(struct headInfo) = 4 + 4 = 8
 * 5. sizeof(Header) = 8
 */

struct headInfo
{
    union header* next;
    unsigned size;
};

typedef struct headInfo HeadInfo;

union header
{
	HeadInfo info;
	Align x;
};

typedef union header Header;

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

Header* NewMemoryChunkFromOperSyst(unsigned newSize);
void LocFree(void* memory);
void* LocMalloc(unsigned nbytes);