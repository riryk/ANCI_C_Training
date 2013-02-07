/***
* Purpose:
*       This file include the function declarations 
*       of base operations for memory management
*
*******************************************************************************/

#include <windows.h>

#define ERROR_NULL_HANDLER         1
#define SYSTEM_ERROR_DESTROY_HEAP  2

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
*int DeleteMyHEap(HANDLE heapHandler) - deletes a new heap from the current process
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
int DeleteMyHeap(HANDLE heapHandler)
{
	bool heapDestroyResult;

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
*void* MemoryAlloc(unsigned int size) - allocates and pins a new memory
*
*Purpose:
*       This function is a simple wrapper for windows.h HeapAlloc
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
void* MemoryAlloc(unsigned int size)
{
	if (myHeap != NULL)
	{
		return (void*)HeapAlloc(myHeap, 0, size);
	}

	return NULL;
}

