
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
/* This test method illustrates how to pass over a handle to some 
 * kernel object from one process to other process. This function 
 * must be called in a source process.
 * dwTProcessId - is an identifier of a target process identifier
 * where we are going to copy a handle to.
 */
void DuplicateHandleToProcess(DWORD dwTProcessId)
{
	/* An uninitialized handle relative to process T */
	HANDLE hObjInProcessT;
	/* Some kernel object inside the process S. */
    HANDLE hObjInProcessS = CreateMutex(NULL, FALSE, NULL);
	/* Then we need to obtain a handle to a target process 
	 * Opens an existing local process object.
	 * dwDesiredAccess [in] - The access to the process object. 
	 *    This access right is checked against the security descriptor for the process. 
	 *    This parameter can be one or more of the process access rights.
	 * bInheritHandle [in] - If this value is TRUE, processes created by this process 
	 *    will inherit the handle. Otherwise, the processes do not inherit this handle.
	 * dwProcessId [in] - The identifier of the local process to be opened.
     *    If the specified process is the System Process (0x00000000), 
	 *    the function fails and the last error code is ERROR_INVALID_PARAMETER. 
	 *    If the specified process is the Idle process or one of the CSRSS processes, 
	 *    this function fails and the last error code is ERROR_ACCESS_DENIED 
	 *    because their access restrictions prevent user-level code from opening them.
	 */
	HANDLE processT = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTProcessId);
    /* This function duplicates the handle into the target process handle table
	 * and returns a reference to this handle. And we need to use some kind of 
	 * inter process communication to notify the target process about the fact
	 * that the handle is now available for him.
	 */
	DuplicateHandle(
		GetCurrentProcess(),
        hObjInProcessS,
        dwTProcessId,
		&hObjInProcessT,
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS);
	/* Use some kind or IPC mechanism to get the handle value of hObjInProcessS into 
	 * process T. We no longer need to communicate with Process T.
	 */
    CloseHandle(processT);
	/* When process S no longer needs to use the mutex, it should close it. */
	CloseHandle(hObjInProcessS);
}
/* Gets the base address of the running application */ 
void DumpModule()
{
    /* Get the base address of the running application.
	 * Can be different from the running module if this code is in a DLL.
	 */
    HMODULE hModule = GetModuleHandle(NULL);
	_tprintf(TEXT("with GetModuleHandle(NULL) = 0x%x\r\n"), hModule);
	/* Pass the address of the current method DumpModule
	 * as parameter to GetModuleHandleEx to get the address
	 * of the current module hModule/hInstance.
	 */
    hModule = NULL;
	GetModuleHandleEx(
       GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
	   (PCTSTR)DumpModule,
	   &hModule);
	_tprintf(TEXT("with GetModuleHandleEx = 0x%x\r\n"), hModule);
}