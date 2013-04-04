

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <Shlobj.h>
#include <malloc.h>


/***
* Purpose:
*       This file include the function declarations 
*       of base operations for threads
*
*******************************************************************************/

HANDLE CreateMThread(PTHREAD_START_ROUTINE threadFunc, LPVOID threadParam);
void DumpModule();
DWORD WaitFor(CONST HANDLE *threadHandlers, int threadCount);
void DumpEnvStrings();