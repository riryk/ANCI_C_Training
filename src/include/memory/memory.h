/***
* Purpose:
*       This file include the function declarations 
*       of base operations for memory management
*
*******************************************************************************/

#include <windows.h>

#define ERROR_NULL_HANDLER         1
#define SYSTEM_ERROR_DESTROY_HEAP  2


HANDLE CreateHeap(int size);
int DeleteMyHeap(HANDLE heapHandler);
void* MemoryAlloc(unsigned int size);