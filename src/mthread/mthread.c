
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

/* Parses environment strings which have a pattern:
 * =::=::\
 * =...
 * var=value\0
 * ...
 * var=value\0\0
 */
void DumpEnvStrings()
{
	PTSTR pEnvBlock = GetEnvironmentStrings();
	TCHAR szName[MAX_PATH];
	TCHAR szValue[MAX_PATH];
	PTSTR pszCurrent = pEnvBlock;
	HRESULT hr = S_OK;
	PCTSTR pszPos = NULL;
	int current;
    size_t cbNameLength;

    while (pszCurrent != NULL)
	{
        /* Skip the meanigless strings like:
		 * "=::=::\"
		 */ 
		if (*pszCurrent != TEXT('='))
		{
			/* Look for '=' separator */
            pszPos = _tcschr(pszCurrent, TEXT('='));
			/* Point now to the first character of the value */
            pszPos++;
			/* Copy the variable name 
			 * The length without '='
			 */
            cbNameLength = (size_t)pszPos - (size_t)pszCurrent - sizeof(TCHAR);
            hr = StringCbCopyN(szName, MAX_PATH, pszCurrent, cbNameLength);
			if (FAILED(hr))
				break;
            /* Copy the variable value with the last NULL character 
			 * and allow truncation because this is for UI only.
			 */
			hr = StringCchCopyN(szValue, MAX_PATH, pszPos, _tcslen(pszPos) + 1);
			if (SUCCEEDED(hr))
			{
				_tprintf(TEXT("[%u] %s=%s\r\n"), current, szName, szValue);
			}
			else if (hr = STRSAFE_E_INSUFFICIENT_BUFFER)
			{
                _tprintf(TEXT("[%u] %s=%s...\r\n"), current, szName, szValue);
			}
			else
			{
                _tprintf(TEXT("[%u] %s=???\r\n"), current, szName);
				break;
			}
		}
		else
		{
            _tprintf(TEXT("[%u] %s\r\n"), current, pszCurrent);
		}
		/* Next variable please */
		current++;
		/* Move to the end of the string */
		while (*pszCurrent != TEXT('\0'))
			pszCurrent++;
		pszCurrent++;
		/* Check if it was not the last string. */
		if (*pszCurrent == TEXT('\0'))
			break;
	}
	/* Do not forget to to free the memory. */
	FreeEnvironmentStrings(pEnvBlock);
}

void PrintEnvironmentVariable(PCTSTR pszVariableName)
{
	PTSTR pszValue = NULL;
	/* Get the size of the buffer that is required to store the value */
	DWORD dwResult = GetEnvironmentVariable(pszVariableName, pszValue, 0);
	if (dwResult != 0)
	{
		/* Allocate the buffer to store the environment variable value */
        DWORD size = dwResult * sizeof(TCHAR);
        pszValue = (PTSTR)malloc(size);
		GetEnvironmentVariable(pszVariableName, pszValue, size);
		_tprintf(TEXT("%s=%s\n"), pszVariableName, pszValue);
		free(pszValue);
	}
	else
	{
        _tprintf(TEXT("'%s'=<unknown value>\n"), pszVariableName);
	}
}

void SpawnChildProcesses()
{
	STARTUPINFO si = { sizeof(si) };

	SECURITY_ATTRIBUTES saProcess;
    SECURITY_ATTRIBUTES saThread;

	PROCESS_INFORMATION piProcessB;
    PROCESS_INFORMATION piProcessC;
    
    TCHAR szPath[MAX_PATH];
	/* Prepare to spawn Process B from Process A.
	 * The handle identifying the new process
	 * object should be inheritable.
	 */
	saProcess.nLength = sizeof(saProcess);
	saProcess.lpSecurityDescriptor = NULL;
	saProcess.bInheritHandle = TRUE;
    /* The handle identifying the new thread
	 * object should not be inheritable.
	 */
	saThread.nLength = sizeof(saThread);
	saThread.lpSecurityDescriptor = NULL;
	saThread.bInheritHandle = FALSE;
	/* Spawn Process B */
	_tcscpy_s(szPath, _countof(szPath), TEXT("cmd.exe")); 
	CreateProcess(
		NULL,
		szPath,
		&saProcess,
		&saThread,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
        &piProcessB);
    _tcscpy_s(szPath, _countof(szPath), TEXT("cmd.exe")); 
	CreateProcess(
		NULL,
		szPath,
		NULL,
        NULL,
		TRUE,
		0,
        NULL,
		NULL,
		&si,
		&piProcessC);
	return 0;
}

void SpawnChildProcessAndWaitUntilItFinishes()
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	DWORD dwExitCode;
    TCHAR szPath[MAX_PATH];
    BOOL fSuccess;

    _tcscpy_s(szPath, _countof(szPath), TEXT("cmd.exe")); 
     fSuccess = 
	  CreateProcess(
		NULL,
		szPath,
		NULL,
        NULL,
		TRUE,
		0,
        NULL,
		NULL,
		&si,
		&pi);

	 if (fSuccess)
	 {
		 /* Close the thread handle as soon as it is no longer needed */
		 CloseHandle(&pi.hThread);
		 /* Suspend our execution until the child has terminated */
		 WaitForSingleObject(pi.hProcess, INFINITE);
		 /* The child process terminated; get its exit code */
		 GetExitCodeProcess(pi.hProcess);
         /* Close the process handle as soon as it is no longer needed */
		 CloseHandle(pi.hProcess);
	 }
}

