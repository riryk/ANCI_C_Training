/***
* Purpose:
*       This file include the function declarations 
*       of base operations for threads
*
*******************************************************************************/

HANDLE CreateMThread(PTHREAD_START_ROUTINE threadFunc, LPVOID threadParam);

DWORD WaitFor(CONST HANDLE *threadHandlers, int threadCount);