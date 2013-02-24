
#include "mthread.h"

/***
*HANDLE CreateMThread(PTHREAD_START_ROUTINE threadFunc)  - creates a new thread in the current process
*
*Purpose:
*       This function is a simple wrapper for windows.h CreateThread
*
*Entry:
*       PTHREAD_START_ROUTINE threadFunc       -  a piece of code to execute in the new thread
*       LPVOID threadParam                     -  a thread parameter
*
*Exit:
*       NOT NULL                    success
*       NULL                        failure
*Exceptions:
*
*******************************************************************************/
HANDLE CreateMThread(PTHREAD_START_ROUTINE threadFunc, LPVOID threadParam) 
{
	/* Try to create a new thread */
	return CreateThread(
		     NULL,
		     0,
             threadFunc,
		     threadParam,
		     0,
		     NULL);
}

/***
*DWORD WaitFor(CONST HANDLE *threadHandlers, int threadCount)  - wait for all threads to finish
*
*Purpose:
*       This function is a simple wrapper for windows.h WaitForMultipleObjects
*
*Entry:
*       CONST HANDLE *threadHandlers       -  an array of threads to wait for
*       int threadCount                    -  a number of items in the array
*
*******************************************************************************/
DWORD WaitFor(CONST HANDLE *threadHandlers, int threadCount)
{
	/* Try to wait for threads to finish */
	return WaitForMultipleObjects(
		threadCount,
        threadHandlers,
		TRUE,
		INFINITE);
}



