
#include <windows.h>
#include "memory.h"


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
