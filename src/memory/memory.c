
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
	/* Method does not process NULL handlers */
	if (heapHandler == NULL)
	{
		return NULL;
	}

	/* Try to allocate new memory in the heap */
    return HeapAlloc(heapHandler, NULL, size);	
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

/* This variables represents a free list. base is the header of the list
 * freeList always points to it and it is always empty. base is not a pointer,
 * memory is allocated for it.
 */
static Header base;
static Header *freeList = NULL;

/***
*Description: Allocates a memory of size nbytes using a custom algorythm
*  
*******************************************************************************/
void* LocMalloc(unsigned nbytes)
{
	Header* prevChunk;
	Header* currentChunk;
	unsigned nunits;
	/* First of all we need to compute a number of chunks needed for new memory 
	 * the size of chunk is : sizeof(Header)
	 * One possible formula is:
	 *   nbytes / ChunkSize + 1  when nbytes = k * ChunkSize + p, p > 0
	 *   nbytes / ChunkSize      when nbytes = k * ChunkSize
	 * We need to unite those two conditions in to one:
	 *   (nbytes - 1) / ChunkSize + 1 
	 */
	nunits = (nbytes + sizeof(Header) - 1) / sizeof(Header) + 1; 
    /* If we allocate memory for the first time, the free list is empty.
	 * We create one chunk item in the freelist, which is of size 0
	 * and references to itself.
	 */
	if ((prevChunk = freeList) == NULL)
	{
       base.info.next = freeList = prevChunk = &base;
	   base.info.size = 0;
	}

	/* Here we use two pointers of one-directional list lookup.
	 * The benefit of this approach is possibility to delete item we are loking for
	 * For example:
	 *
	 * 1. Start
	 *    From start we have one item in the list which references to itself.
	 * Lets cal this item LNil. So the list looks like:    ..->LNil->..self
	 * Then we keep two pointers: Prev and Current. From start Prev = Current = LNil
	 * freeList is always a pointer to the head of the list and it is always LNil.
	 *
	 * 2. Go through
	 *    When freeList is filled, this process lokks like here:
	 *    ..freeList->item1->item2->...->itemk-1->itemk->itemk+1->....->itemn->freeList...
	 *                                    /         /                    /       /
	 *                                  Prev       Current  ->->->->-> Prev     Current
     *
	 * 3. End. 
	 *    We reach the end of the list when the current starts pointing to the head of 
	 * the list(freeList). But in the following for cycle we do not have an out of cycle 
	 * condition. When we reach the end, that means that we have gone through all free
	 * list and have not found any fit free chunk of memory. We call NewMemoryChunkFromOperSyst
	 * function for taking a new memory out of the operational system. This function allocates 
	 * a new chunk and inserts it into the head of our free list. After that freeList looks like:
	 *
	 * ..freeList->newChunk->item1->item2->...->itemk-1->itemk->itemk+1->....->itemn->freeList...
	 *     /                                                                    /       /
	 *  Current                                                              Prev    Current
	 *  Then we move current to newChunk and run cycle further
	 *
	 * ..freeList->newChunk->item1->item2->...->itemk-1->itemk->itemk+1->....->itemn->freeList...
	 *     /        /                                                              
	 *  Prev      Current                                                        
	 * In this case newChunk is fit and we remove it from free list and return to a user
	 *
	 * 4. Delete
	 *    For deleting we simply move pointers
     *    Before deleting                      /----\ arrow skips itemk
	 * ..freeList->item1->item2->...->itemk-1->itemk->itemk+1->....->itemn->freeList...
	 *                                    /      /                   
	 *                                  Prev   Current  
	 *
	 * ..freeList->item1->item2->...->itemk-1--------->itemk+1->....->itemn->freeList...
	 *                                   /       itemk
	 *                               freeList  - after we have found and deleted item
	 *  freeList pointer points to the previous item
	 * 5. Invariant.
	 * We need to prove a cycle being not spinning to infinity. If the cycle go up
	 * to freeList pointer, we insert a new chunk in the head, freeList. And cycle 
	 * always finishes up after that because the newly created chunk's size is nunits
	 */
	currentChunk = prevChunk->info.next;
	for (;;prevChunk = currentChunk, currentChunk = currentChunk->info.next)
	{
	   /* big enough */
       if (currentChunk->info.size >= nunits)
	   {
		  /* If we have found a chunk with exact size, we remove it 
		   * from free list. We assign previous's next to the current's
		   * next and omit current chunk
		   */
          if (currentChunk->info.size == nunits)
		  {
			  /* Deleting of item into one-directional list */
              prevChunk->info.next = currentChunk->info.next;
		  }
		  /* if nunits is less than current chunk size, we divide it 
		   * into two parts: one part is returned to user and the other is left
		   * for recycling. We cut of memory from the tail
           * | current | current + 1 |...|current + n - k|...|current + n|
           * We want to cut off k chunks from the end. For this we set 
		   * current size to n - k and return pointer to current + n - k to user
		   */
		  else
		  {
			  /* current size - nunits is left in free list */
              currentChunk->info.size -= nunits;
			  /* Move current chunk pointer to the allocated tail */
              currentChunk += currentChunk->info.size;
			  /* Set appropriate size to chunk */
              currentChunk->info.size = nunits;
		  }
          freeList = prevChunk;
		  /* Return cut tail to the user. But we do not show the first to user */
		  return (void*)(currentChunk + 1);
	   }
       /* We have just gone through all free list and come back to the start
	    * point. In this case we ask an operational system for more memory
	    */
	   if (currentChunk == freeList)
	   {
	      /* If for some reasons a memory cannot be allocated, we return NULL */
          if ((currentChunk = NewMemoryChunkFromOperSyst(nunits)) == NULL)
		  {
			  return NULL;
		  }
	   }
	}
}

void LocFree(void* memory)
{
    Header* cur;
	Header* curHd;
    /* The memory, which has been returned to user, points to a body
	 * The real header one point back. */
    curHd = (Header*)memory - 1;
	/* cur is a reference to the head of freeList */
	cur = freeList;
    /* 1. We do not have adjacent blocks
	 * This cycle is important if the have been divided chunks. So we need to 
	 * find an adjacent chunk and coalesce it with our memory into one large chunk
	 * The algorythm when there are no divided chunks is a simple insertion into
	 * one-directional list:
	 *..freeList->item1->item2->...->itemk-1->itemk->itemk+1->....->itemn->freeList...
	 *     /   or  /                                                  /       /
	 *  Current  Current                                            Prev    Current
	 * after the following cycle cur pointer can be freeList or item1. Lets suppose 
	 * it is item1. And illustrate a process of insertion.
	 *               delete 
	 *..freeList->item1->item2->...->itemk-1->itemk->itemk+1->....->itemn->freeList...
	 *            /  \     /                                  
	 *     Current     new                      
	 * after insertion we have 
	 * ..freeList->item1->new->item2->...->itemk-1->itemk->itemk+1->....->itemn->freeList...
	 *            /  
	 *     Current   
     * 2. How to search an appropriate place where to insert a freed block. The purpose of
	 * this approach is to achieve maximum possibility of coalescing of chunks into one large
	 * chunk. If our minimal chunk size is small, that means that we will frequently ask operational
	 * system for giving us a new memory and op. syst. can provide us with contiguous blocks. 
	 * In operational system contiguous blocks are:
	 *  |   30 bytes length    |  40 bytes length | ...... | 30 bytes length  |
	 *0x444              0x444 + 30 in hex   0x44 + 70 hex   ....
	 * Block A1 is called contiguous to block A2 if:
	 * addr(A2) = Addr(A1) + size(A1);
	 * We keep freeList ordered by chunk adresses. For example the algorythm:
	 * 1. freeList is empty      ...->LNil->..
	 *                                 /
	 *                              freeList
	 * 2. Insert the first item into list: cur = freeList = cur.next. Our cycle breaks. 
	 * And here we can prove an invariant of the cycle. freeList looks like here:
	 *                ...->LNil->item1->LNil->... addr(item1) > addr(LNil) or 
	 *                      /                     addr(item1) < addr(LNil) all variants are
	 *                  freeList                  possible
	 * 3. When we inserts the second item
	 * Supposing that addr(item2) > addr(item1). We move through the free list and inserts into 
	 * an appropriate place
	 *  ...->LNil->item1->item2->item3->....>itemk-1->itemk->itemk+1->....->itemn->LNil->...
     *                                                  / 
	 *                                              freeList
	 * And this list is sorted. But there is a one critical point in the list, which is called 
	 * a point of entrance. For example itemp and itemp+1 when addr(itemp+1) < addr(itemp)
	 *  ...->itemk->itemk+1->... .In this case we insert new address between itemk and itemk+1
	 */
	for (; !(cur < curHd && curHd < cur->info.next); cur = cur->info.next)
	{
        if (cur >= cur->info.next && (cur < curHd || curHd < cur->info.next))
		{
			break;
		}
	}
    /* In this case we have a situation, when a freed chunk is adjacent to the right 
	 * chunk cur.next. For example a picture:
	 *  | cur size |......| curHd size  |cur.next size      | .....
	 *    /                 /             /
	 *  current         new chunk       next
	 * In this case we have:
	 *  new chunk + sizeof (new chunk) = next.
	 * We need to unite curHd chunk + cur.next. For this we simply 
	 * increase curHd size for next size and change curHd next 
	 * to the cur next chunk's next pointer.
	 */
	if (curHd + curHd->info.size == cur->info.next)
	{
        curHd->info.size += cur->info.next->info.size;
        curHd->info.next = cur->info.next->info.next;
	}
	else
	{
		/* This a part of simple insertion into one-directional list.
		 * new item's next pointer is set to the current's next
		 */
        curHd->info.next = cur->info.next;
	}
    /* In this case we have a situation, when a freed block is adjacent to the left
	 * chunk. For example a picture:
	 *  | cur size | curHd size  |...... | cur.next size      | .....
	 *    /           /                     /
	 *  current    new chunk              next 
	 * In this case we have:
	 *  current + current size = curHd
	 * We need to unite current chunk + curHd chunk. For this we simply
	 * increase current size for curHd chunk size and change current next to the curHd's 
	 * next pointer. This pointer is being evaluated just before this if.
	 *  There also can be a situation when we need to unate 3 blocks
	 * | cur size | curHd size  | cur.next size      | .....
	 *    /           /                     /
	 *  current    new chunk              next 
	 */
	if (cur + cur->info.size == curHd)
	{
        cur->info.size += curHd->info.size;
        cur->info.next = curHd->info.next;
	}
	else
	{
		/* This a part of simple insertion into one-directional list.
		 * new current's next pointer is set to a new item.
		 */
        cur->info.next = curHd;
	}
    /* We set freeList pointer to the current item and it will be used in 
	 * a cycle in LocMalloc method.
	 */
	freeList = cur;
}

/***
*Description: Take a memory from operational system of size
* sizeof(long)*newSize with possibility to reuse it.
*  
*******************************************************************************/
Header* NewMemoryChunkFromOperSyst(unsigned newSize)
{
	void* opSystNewMemory;
    Header* newMemory;
    unsigned factSize;

	/* If a chunk size is too small, we enhance its size to 
	 * the minimal allowed size
	 */
    if (newSize < MIN_ALLOC)
	{
        newSize = MIN_ALLOC;
	}
    /* We need newSize chunks for a new memory. But the first chuck always
	 * contains an information about the size = newSize and a pointer to 
	 * the next memory chunk. We need one more chuns for this.
	 * For example the new memory structure:
	 * | pointer to next free block  |  size   |  free block of size newSize ......  | 
	 * To user we return a pointer to free block
	 */
	factSize = newSize;
    /* Take a memory from an operational system */
    opSystNewMemory = MemoryAlloc(factSize * sizeof(Header));
    /* We get NULL pointer when it is not possible to allocate 
	 * memory from operational system. In this case, we return null
	 */
    if (opSystNewMemory == NULL)
	{
		return NULL;
	}
	/* Memory returned from opertion system is of type void*.
	 * We will keep it a pointer to Header. Now newMemory is a 
	 * pointer to the first block of size sizeof(Header) = 8 in 
	 * our allocated memory. And the first chunk contains an information
	 * about the size of this chunk and a pointer to the next chunk
	 */
	newMemory = (Header*)opSystNewMemory;
	/* Set size of new memory */
	newMemory->info.size = newSize;
	/* After that we need to insert this item into freeList for future use 
	 * in LocMalloc method
	 */
    LocFree((void*)(newMemory + 1));
	/* Return new memory */
	return freeList;
}

/* Returns 0 if the same instance has been already launched 
 * on this machine.
 * Return 1 if the are no other instances.
 */
int CheckIfAnotherInstanceIsRunning()
{
	/* Try to create named mutext.
	 * If a mutex with this name is already exists
	 * (The other instance has alreadu created this),
	 * the operational system creates a new handle which
	 * points to the same mutex and increments mutext usage
	 * GetLastError() returns error already exist message
	 */
	HANDLE handle = CreateMutex(
		NULL, 
		FALSE, 
		TEXT("{FA531CC1-0497-11d3-A180-00105A276C3E}"));
	/* Check if this mutex exists */
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		/* Here we have found the same instance and we need to
		 * close the already created handle and leave only the firstly created one
		 */
		CloseHandle(handle);
		return 0;
	}
	/* This is the first instance of this application running */
	/* ... */
	/* Before exiting, close the object */
	CloseHandle(handle);
	return 1;
}