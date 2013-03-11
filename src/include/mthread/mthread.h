#include <windows.h>
#include <tchar.h>

/***
* Purpose:
*       This file include the function declarations 
*       of base operations for threads
*
*******************************************************************************/

HANDLE CreateMThread(PTHREAD_START_ROUTINE threadFunc, LPVOID threadParam);
void DumpModule();
DWORD WaitFor(CONST HANDLE *threadHandlers, int threadCount);