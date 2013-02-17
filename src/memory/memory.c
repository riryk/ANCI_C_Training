
#include <windows.h>
#include "memory.h"

/* Total central array of memory taken from operation system */
static char allocBuffer[ALLOC_SIZE];

/* A pointer to free position in the array. 
 * We divide allocBuffer[0,...,ALLOC_SIZE - 1] into 2 subarrays: 
 * allocBuffer[0,...,freePosition - 1, freePosition,...,ALLOC_SIZE - 1]
 */
static char* freePosition = allocBuffer;

/***
*char* StackAlloc(int n) - allocate a new n bytes of memory from the storage
*
*Exit:
*       NOT NULL                    success
*       NULL                        failure
*
*******************************************************************************/
char* StackAlloc(int n)
{
    char* startOfBuffer = allocBuffer;
	char* endOfBuffer = allocBuffer + ALLOC_SIZE;

	/* The position of the pointer if there are enough free space in the array */
	char* newFreePosition = freePosition + n;

	/* There are no free position where to take a new memory block,
	 * if the newFreePosition will exceed the end of our buffer.
	 */
	if (newFreePosition > endOfBuffer)
	{
	    return NULL;
	}

	/* Alloc memory by moving freePosition pointer for n position right */
    freePosition += n;

    /* Return a pointer to a new memory  */
	return freePosition - n;
}

/***
*void StackFree(char* memory) - frees memory allocated by StackAlloc
*
*Entry:
*       char*  memory      - a pointer to allocated memory by StackAlloc
*  
*  This method of memory management has a very big restriction: It works like stack
*  For example:
*   char* Mem1 = StackAlloc(10);
*   char* Mem2 = StackAlloc(10);
*   char* Mem3 = StackAlloc(10);  [Mem1,...,9,Mem2,...,19,Mem3,...,29,freePosition,.....]
*  If we free Mem1, the other memory will be lost and overwritten. So we need to free
*  the allocated memory in this order:
*   StackFree(Mem3);
*   StackFree(Mem2);
*   StackFree(Mem1);
*
*******************************************************************************/
void StackFree(char* memory)
{
	/* A pointer to the end of the buffer */
    char* endOfBuffer = allocBuffer + ALLOC_SIZE;

	/* If the memory pointer points inside the buffer,
	 * we simple move freePosition pointer into this memory
	 */
	if (memory >= allocBuffer && memory <= endOfBuffer)
	{
       freePosition = memory;
	}
}

/***
*HANDLE CreateHeap(int size) - creates a new heap in the current process
*
*Purpose:
*       This function is a simple wrapper for windows.h HeapCreate
*
*Entry:
*       int    size       - the size of the new heap
*
*Exit:
*       NOT NULL                    success
*       NULL                        failure
*Exceptions:
*
*******************************************************************************/
HANDLE CreateHeap(int size) 
{
	/* Creating a new heap */
    return HeapCreate(0, size, 0);
}


/***
*int DeleteHeap(HANDLE heapHandler) - deletes a new heap from the current process
*
*Purpose:
*       This function is a simple wrapper for windows.h HeapDestroy
*
*Entry:
*       HANDLE heapHandler       - a handler to the heap
*
*Exit:
*       0                            SUCCESS       
*       1  ERROR_NULL_HANDLER        could not destroy NULL handler
*       2  SYSTEM_ERROR_DESTROY_HEAP windows HeapDestroy returned error
*
*Exceptions:
*
*******************************************************************************/
int DeleteHeap(HANDLE heapHandler)
{
	int heapDestroyResult;

	/* Method does not process NULL handlers */
	if (heapHandler == NULL)
	{
		return ERROR_NULL_HANDLER;
	}

	/* Try to destroy heap using Win API */
    heapDestroyResult = HeapDestroy(heapHandler);

	/* If error, quit function */
    if (heapDestroyResult > 0)
	{
		return SYSTEM_ERROR_DESTROY_HEAP;
	}

	/* Return success */
	return 0;
}



/***
*void* MemoryAlloc_Heap(HANDLE heapHandler, unsigned int size) - allocates and pins a new memory
*
*Purpose:
*       This function is a simple wrapper for windows.h HeapAlloc
*
*Entry:
*       HANDLE        heapHandler   -   a handler to a heap 
*       unsigned int  size          -   a size of memory
*
*Exit:
*       not NULL                            SUCCESS       
*       NULL                                FAILURE
*
*Remarks:
*       if heapHandler is NULL, function will return NULL.
*
*Exceptions:
*
*******************************************************************************/
void* MemoryAlloc_Heap(HANDLE heapHandler, unsigned int size)
{
	void*  newMemory;

	/* Method does not process NULL handlers */
	if (heapHandler == NULL)
	{
		return NULL;
	}

	/* Try to allocate new memory in the heap */
    return HeapAlloc(heapHandler, HEAP_ZERO_MEMORY, size);	
}

/***
*void* MemoryAlloc(unsigned int size) - allocates and pins a new memory
*
*Purpose:
*      This function is almost identical to the previous MemoryAlloc
*      The only difference is that this function uses the process heap
*
*******************************************************************************/
void* MemoryAlloc(unsigned int size)
{
	/* Get a handle to the process heap */
	HANDLE processHeap = GetProcessHeap();

	/* If we could not get the process heap, return null */
	if (processHeap == NULL)
	{
		return NULL;
	}

	/* Heap allocation */
	return MemoryAlloc_Heap(processHeap, size);
}

/***
*void* MemoryRealloc_Heap(HANDLE heapHandler, void* memory, unsigned int newSize) 
*  - reallocates and pins a new memory
*
*Purpose:
*       This function is a simple wrapper for windows.h HeapRealloc
*
*Entry:
*       HANDLE        heapHandler   -   a handler to a heap 
*       void*         memory        -   an old memory
*       unsigned int  newSize       -   a new size of memory for extending
*
*Exit:
*       not NULL                            SUCCESS       
*       NULL                                FAILURE
*
*Remarks:
*       if heapHandler is NULL, function will return NULL.
*
*Exceptions:
*
*******************************************************************************/
void* MemoryRealloc_Heap(HANDLE heapHandler, void* memory, unsigned int newSize)
{
    void*  newMemory;

    /* Method does not process NULL handlers */
	if (heapHandler == NULL)
	{
		return NULL;
	}

    /* Try to reallocate the memory in the heap */
    return HeapReAlloc(
		heapHandler, 
		HEAP_REALLOC_IN_PLACE_ONLY, 
		memory,
		newSize);
}

/***
*void* MemoryRealloc(void* memory, unsigned int newSize) - reallocates an existed memory
*
*Purpose:
*      This function is almost identical to the previous MemoryRealloc
*      The only difference is that this function uses the process heap
*
*******************************************************************************/
void* MemoryRealloc(void* memory, unsigned int newSize)
{
    /* Get a handle to the process heap */
	HANDLE processHeap = GetProcessHeap();

	/* If we could not get the process heap, return null */
	if (processHeap == NULL)
	{
		return NULL;
	}

	/* Heap allocation */
	return MemoryRealloc_Heap(processHeap, memory, newSize);
}

int MemoryFree(void* memory)
{
	/* Get a handle to the process heap */
	HANDLE processHeap = GetProcessHeap();

	/* If we could not get the process heap, return null */
	if (processHeap == NULL)
	{
		return ERROR_INVALID_HEAP;
	}

	/* Heap free */
	return MemoryFree_Heap(processHeap, memory);
}

/***
*int MemoryFree_Heap(HANDLE heapHandler, void* memory) - frees a memory into a heap
*
*Purpose:
*       This function is a simple wrapper for windows.h HeapFree
*
*Entry:
*       HANDLE        heapHandler   -   a handler to a heap 
*       void*         memory        -   an old memory
*
*Exit:
*       0                            SUCCESS       
*       non zero                     FAILURE
*
*Remarks:
*       if heapHandler is NULL, function will return NULL.
*
*Exceptions:
*
*******************************************************************************/
int MemoryFree_Heap(HANDLE heapHandler, void* memory)
{
	int heapValidateResult;

	/* Method does not process NULL handlers */
	if (heapHandler == NULL)
	{
		return ERROR_NULL_HANDLER;
	}

	/* Validate if the memory belong to the heap
	 * Otherwise we are not allowed to free this memory
	 */
    heapValidateResult = HeapValidate(
		heapHandler,
		HEAP_NO_SERIALIZE,
		memory);

	/* For invalid memory we return error */
	if (heapValidateResult == 0)
	{
        return ERROR_MEM_DOES_NOT_BELONG_TO_HEAP;
	}

	/* Free memory */
	return HeapFree(
		heapHandler,
		0,
		memory);

	/* Return success */
	return 0;
}

/***
*void MemorySet(char* memory, char symbol, int count) - 
* fills allocated memory with a specific sysmbol.
*
*******************************************************************************/
void MemorySet(char* memory, char symbol, int count)
{
	while (count--)
	{
		*memory++ = symbol;
	}
}