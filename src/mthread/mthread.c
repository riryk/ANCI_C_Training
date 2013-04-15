
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
		 GetExitCodeProcess(pi.hProcess, &dwExitCode);
         /* Close the process handle as soon as it is no longer needed */
		 CloseHandle(pi.hProcess);
	 }
}

void RunProcessAsAdmin()
{
	/* Initialize the structure */
	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
	/* Ask for privileges elevation */
	sei.lpVerb = TEXT("runas");
	/* Create a Command Promt from which you will be able to start
	 * other elevated applications.
	 */
    sei.lpFile = TEXT("cmd.exe");
	/* Don't forget this parameter; otherwise, the window will be hidden */
	sei.nShow = SW_SHOWNORMAL;
	/* Elevate privileges for running this application */
    if (!ShellExecuteEx(&sei))
	{
       DWORD dwStatus = GetLastError();  
	   if (dwStatus == ERROR_CANCELLED)
	   {
		   /* The user refused to allow privileges elevation */
	   }
	   else if (dwStatus == ERROR_FILE_NOT_FOUND)
	   {
          /* The file defined by lpFile was not found and 
		   * an error message popped up.
		   */ 
	   }
	}
}

BOOL GetProcessElevation(TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin)
{
    HANDLE hToken = NULL;
	DWORD dwSize;
	BOOL bResult = FALSE;
	/* Get current process token */
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
	{
       return (FALSE); 
	}
    /* Retrieve elevation type information */
	if (GetTokenInformation(
		 hToken,
		 TokenElevationType,
		 pElevationType, 
		 sizeof(TOKEN_ELEVATION_TYPE),
		 &dwSize))
	{
        /* Create the SID corresponding to the Administrators group */ 
        BYTE adminSID[SECURITY_MAX_SID_SIZE];
        dwSize = sizeof(adminSID);
		CreateWellKnownSid(
            WinBuiltinAdministratorsSid,
			NULL,
			&adminSID,
			&dwSize);

        if (*pElevationType == TokenElevationTypeLimited)
		{
			/* Get handle to linked token (will have one if we are lua) */
			HANDLE hUnfilteredToken = NULL;

            GetTokenInformation(
				hToken,
				TokenLinkedToken,
				(VOID*)&hUnfilteredToken,
				sizeof(HANDLE),
				&dwSize);
			/* Check if this original token contains SID */
            if (CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin))
			{
                bResult = TRUE; 
			}
			/* Close the unfiltered token */
			CloseHandle(hUnfilteredToken);
		}
		else
		{
			*pIsAdmin = IsUserAnAdmin();
            bResult = TRUE; 
		}
	}
	CloseHandle(hToken);
	return (bResult);
}

/* Return values are: 
 *  1. means that the process is inside a job.
 *  2. 
 */
int StartRestrictedProcess()
{
   /* Check if we are not already associated with a job.
    * If this is the case, there is no way to switch to 
	* another job.
    */
    BOOL bInJob = FALSE;
	HANDLE hjob;
	/* Place some restrictions on processes in the job. 
	 * First, set some basic restrictions.
	 */
	JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = { 0 };
	/* Second, set some UI restrictions. */
	JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
    TCHAR szCmdLine[8];
	BOOL bResult;

	FILETIME CreationTime;
    FILETIME ExitTime;
    FILETIME KernelTime;
    FILETIME UserTime;
    TCHAR szInfo[MAX_PATH];

    DWORD dw;
    HANDLE h[2];

	/* Determines whether the process is running in the specified job. 
	 * ProcessHandle [in]
	 *   A handle to the process to be tested. 
	 *   The handle must have the PROCESS_QUERY_INFORMATION 
	 *   or PROCESS_QUERY_LIMITED_INFORMATION access right. 
	 *   For more information, see Process Security and Access Rights.
	 * JobHandle [in, optional]
     *   A handle to the job. If this parameter is NULL, 
	 *   the function tests if the process is running under any job.
     *   If this parameter is not NULL, the handle must have the JOB_OBJECT_QUERY access right. 
	 *   For more information, see Job Object Security and Access Rights.
     * Result [out]
     *   A pointer to a value that receives TRUE 
	 *   if the process is running in the job, and FALSE otherwise.
	 */
    IsProcessInJob(GetCurrentProcess(), NULL, &bInJob);
	if (bInJob)
	{
	   return 1;   
	}
	/* Create a job kernel object. */
    hjob = CreateJobObject(NULL, TEXT("Wintellect_RestrictedProcessJob"));
	/* The process always runs in the idle priority class. */
	jobli.PriorityClass = IDLE_PRIORITY_CLASS;
	/* The job cannot use more than 1 second of CPU time. */
	jobli.PerJobUserTimeLimit.QuadPart = 10 * 1000;
	/* These are the only 2 restrictions I want placed on the job (process). */
	jobli.LimitFlags = JOB_OBJECT_LIMIT_PRIORITY_CLASS
		| JOB_OBJECT_LIMIT_JOB_TIME;
	/* Sets limits for a job object. */
	SetInformationJobObject(
		hjob, 
		JobObjectBasicLimitInformation,
	    &jobli,
		sizeof(jobli));
	/* A fancy zero */
	jobuir.UIRestrictionsClass = JOB_OBJECT_UILIMIT_NONE;
	/* The process can't log off the system. */
	jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;
	/* The process can't access USER objects (such as other windows)
	 * in the system.   
	 */
	jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;
    /* Sets limits for a job object.
	 */
	SetInformationJobObject(
		hjob, 
		JobObjectBasicUIRestrictions, 
		&jobuir,
		sizeof(jobuir));
	/* Spawn the process that is to be in the job.
	 * Note: 
	 *   You must first spawn the process and then place the process
	 *   in the job. This means that the process' thread must be initially
	 *   suspended so that it can't so that it can't execute any code 
	 *   outside of the job's restrictions.
	 */
	_tcscpy_s(szCmdLine, _countof(szCmdLine), TEXT("CMD"));
	bResult = 
		CreateProcess(
		   NULL,
		   szCmdLine,
		   NULL,
		   NULL,
		   FALSE,
		   CREATE_SUSPENDED | CREATE_NEW_CONSOLE, 
		   NULL,
		   NULL,
		   &si,
		   &pi);
	/* Place the process in the job.
	 * Note: If this process spawns any children, the children
	 * are automatically part of the same job.
	 */
	AssignProcessToJobObject(hjob, pi.hProcess);
	/* Now we can allow the child process' thread to execute code */
	ResumeThread(pi.hThread);
	/* Close handle */
	CloseHandle(pi.hThread);
	/* Wailt for the process to terminate or
	 * for all the job's CPU time to be used.
	 */
	h[0] = pi.hProcess;
    h[1] = hjob;

    dw = WaitForMultipleObjects(2, h, FALSE, INFINITE);
	switch (dw - WAIT_OBJECT_0)
	{
	case 0:
		printf("The process has terminated...");
		break;
	case 1:
        printf("All of the job's allotted CPU time was used...");
		break;
	}

	/* Get timing information about the processes */
	GetProcessTimes(
		pi.hProcess,
		&CreationTime,
		&ExitTime,
		&KernelTime,
		&UserTime);
	StringCchPrintf(
		szInfo,
		_countof(szInfo),
		TEXT("Kernel = %u | User = %u\n"),
		KernelTime.dwLowDateTime / 10000,
		UserTime.dwLowDateTime / 10000);
	CloseHandle(pi.hProcess);
	CloseHandle(hjob);
}

void EnumProcessIdsInJob(HANDLE hjob)
{
   /* I assume that there will never be more 
    * 10 processes in this job.
    */
   #define MAX_PROCESS_IDS 10
   DWORD x;
   /* Calculate the number of bytes needed for
    * structure and process IDs.
    */
   DWORD cb = sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + 
	   (MAX_PROCESS_IDS - 1) * sizeof(DWORD);
   /* Allocate the block of memory. */
   PJOBOBJECT_BASIC_PROCESS_ID_LIST pjobpil = 
	   (PJOBOBJECT_BASIC_PROCESS_ID_LIST)_alloca(cb);
   /* Tell the function the maximum number of processes
    * that we allocated space for.
    */
   pjobpil->NumberOfAssignedProcesses = MAX_PROCESS_IDS;
   /* Request the current set of process IDs. 
    * Retrieves limit and job state information from the job object.
	* hJob [in, optional]
    *    A handle to the job whose information is being queried.
	*    If this value is NULL and the calling process is associated 
	*    with a job, the job associated with the calling process is used. 
	*    If the job is nested, the immediate job of the calling process is used.   
	* obObjectInfoClass [in]
    *    The information class for the limits to be queried. 
	*    This parameter can be one of the following values.
	*    JobObjectBasicProcessIdList = 3
	*      The lpJobObjectInfo parameter is a pointer 
	*      to a JOBOBJECT_BASIC_PROCESS_ID_LIST structure.
    * pJobObjectInfo [out]
    *    The limit or job state information. 
	*    The format of this data depends on the value 
	*    of the JobObjectInfoClass parameter.     
	* cbJobObjectInfoLength [in]
    *    The count of the job information being queried, in bytes. 
	*    This value depends on the value of the JobObjectInfoClass parameter.
	* lpReturnLength [out, optional]
    *    A pointer to a variable that receives the length of data 
	*    written to the structure pointed to by the lpJobObjectInfo parameter. 
	*    If you do not want to receive this information, specify NULL.
    */
   QueryInformationJobObject(
	   hjob,
	   JobObjectBasicProcessIdList,
	   pjobpil,
	   cb,
	   &cb);
   /* Enumerate the process IDs */
   for (x = 0; x < pjobpil->NumberOfProcessIdsInList; x++)
   {
	   /* Use pjobpil->ProcessIdList[x]... */
   }
   /* Since _alloca was used to allocate the memory,
    * we do not need to free it here.
    */
}

DWORD WINAPI SecondThread(PVOID pvParam)
{
   /* Do something lengthing here. ... 
    * Attempt to access the variable on the FirstThread's stack.
	* NOTE: This may cause an access violation - it depends on timing!
    */
   int* xInt = (int*)pvParam;
   *xInt = 5; // Access violation exception.
}

DWORD WINAPI FirstThread(PVOID pvParam)
{
   /* Initialize a stack-based variable */
   int x = 0;
   DWORD dwThreadID;
   /* Create a new thread */
   HANDLE hThread = 
	   CreateThread(
          NULL,
		  0,
		  SecondThread,
          (PVOID)&x,
          0,
		  &dwThreadID);
   /* We do not reference the new thread anymore,
    * so close our handle to it.
    */
   CloseHandle(hThread);
   /* Our thread is done.
    * BUG: our stack will be destroyed, but Second thread
	* might try to access it.
    */
   return (0);
}

struct _MTidData
{
   unsigned long _tid; /* thread ID */  
   unsigned long _thandle; /* thread handle */
   int _terrno; /* errno value */
   unsigned long _tdoserrno; /* _doerrno value */
   void* _initaddr;
   void* _initarg;
};

typedef struct _MTidData * _PMidData;

void _mEndThreadEx(unsigned retcode)
{
	/* Pointer to thread's data block */
    _PMidData ptd;
	/* Clean up floating-point support (code not shown).
	 * Get the address of this thread's tiddata block.
   	 */
	//ptd = _getptd_noexit();
	/* Free the tiddata block. */
    if (ptd != NULL)
		//_freeptd(ptd);
	/* Terminate the thread. */
	ExitThread(retcode);
}

void _mCallThreadStartex()
{
	unsigned threadRetCode;
	void* initArg;
    unsigned (*func)(void*);
	/* pointer to thread's _MTidData structure */
	_PMidData ptd;
    /* get the pointer to thread data from TLS 
	 * This function inside uses
	 * LPVOID WINAPI TlsGetValue(_In_  DWORD dwTlsIndex);
     * dwTlsIndex [in] 
     *   The TLS index that was allocated by the TlsAlloc function.
	 * If the function succeeds, the return value is 
	 * the value stored in the calling thread's 
	 * TLS slot associated with the specified index.
	 */
    ptd = _getptd();
	/* Wrap desired thread function in SEH frame to
	 * handle run-time errors and signal support
	 */
	__try
	{
       /* Call desired thread function, passing it the desired parameter. 
	    * Pass thread's exit code value to _endthreadex.
	    */
		initArg = ptd->_initarg;
		func = ptd->_initaddr;
		threadRetCode = func(initArg);

	   _mEndThreadEx(threadRetCode);
	   /* 
        *  GetExceptionCode()
	    * Retrieves a code that identifies the type of exception that occurs. 
	    * The function can be called only from within the filter expression 
		* or exception-handler block of an exception handler.
	    *  
		*  GetExceptionInformation()
        * Retrieves a computer-independent description of an exception, 
		* and information about the computer state that exists for the thread when the exception occurs. 
		* This function can be called only from within the filter expression of an exception handler.
	    */
	}
	__except(_XcptFilter(GetExceptionCode(), GetExceptionInformation()))
	{
        /* The C run-time's exception handler deals with run-time errors
		 * and signal support; we should never get it here.
		 */ 
		_exit(GetExceptionCode());
	}
}

unsigned long WINAPI _mThreadStartex (void* ptd)
{
   /* Note: ptd is the address of this thread's tiddata block.
    * Assosiate the tiddata block with this thread so
	* _getptd() will be able to find it in _callthreadstartex.
    */

	/* Stores a value in the calling thread's thread 
	 * local storage (TLS) slot for the specified TLS index. 
	 * Each thread of a process has its own slot for each TLS index. 
	 * 
	 * dwTlsIndex [in] 
     *   The TLS index that was allocated by the TlsAlloc function.
     * 
	 * lpTlsValue [in, optional] 
     *   The value to be stored in the calling thread's TLS slot for the index.
	 */

	//TlsSetValue(__tlsindex, ptd);

	/* Save this thread ID in the _tiddata block */
	((_PMidData)ptd)->_tid = GetCurrentThreadId();
   	/* Initialize floating-point support () 
	 * call helper function.
	 */
	_mCallThreadStartex();
	/* We never get here; the thread dies in _callthreadstartex() */
}

uintptr_t __cdecl _mBeginThread
(
  void* psa,
  unsigned cbStackTrace,
  unsigned (__stdcall *pfnStartAddr) (void*),
  void* pvParam,
  unsigned dwCreateFlags,
  unsigned *pdwThreadID
)
{
   _PMidData ptd; /* Pointer to thread's data block */
   uintptr_t thdl; /* Thread's handle */
   /* Allocate data block for the new thread */
   ptd = (_PMidData)malloc(sizeof(_PMidData));
   if (ptd == NULL)
   {
      goto error_return;
   }
   /* Initialize the data block. */

   /* DWORD WINAPI TlsAlloc(void); 
    *
    *  Allocates a thread local storage (TLS) index. 
	*  Any thread of the process can subsequently use this index 
	*  to store and retrieve values that are local to the thread, 
	*  because each thread receives its own slot for the index.
	*
    */

   _initptd(ptd);
   /* Save the desired thread function and the parameter
    * we want it to get in the data block.
    */
   ptd->_initaddr = (void*)pfnStartAddr;
   ptd->_initarg = pvParam;
   ptd->_thandle = (uintptr_t)(-1);
   /* Create the new thread. */
   thdl = (uintptr_t)CreateThread(
	        (LPSECURITY_ATTRIBUTES)psa,
             cbStackTrace,
			 _mThreadStartex,
             (PVOID)ptd,
			 dwCreateFlags,
             pdwThreadID);
     
   if (thdl == 0)
   {
       /* Thread couldn't be created, cleanup and return failure. */
	   goto error_return;
   }
   /* Thread created OK, return the handle as unsigned long. */ 
   return (thdl);
error_return:
   /* 
    * Error: data or thread couldn't be created.
	* GetLastError() is mapped into errno corresponding values
	* if something wrong happened in CreateThread.
    */
   free(ptd);
   return ((uintptr_t)0L);
}

DWORD WINAPI ChildThread(PVOID pvParam)
{
   HANDLE hThreadParent = (HANDLE)pvParam;
   FILETIME ftCreationTime, ftExitTime, ftKernelTime, ftUserTime;
   GetThreadTimes(hThreadParent,
      &ftCreationTime, &ftExitTime, &ftKernelTime, &ftUserTime);
   CloseHandle(hThreadParent);
   // Function continues...
}

DWORD WINAPI ParentThread(PVOID pvParam)
{
   HANDLE hThreadParent;
   /*
     hSourceProcessHandle [in]
        A handle to the process with the handle to be duplicated.
        The handle must have the PROCESS_DUP_HANDLE access right. 
		For more information, see Process Security and Access Rights.

     hSourceHandle [in]
        The handle to be duplicated. This is an open object handle 
		that is valid in the context of the source process. 
		For a list of objects whose handles can be duplicated, 
		see the following Remarks section.

     hTargetProcessHandle [in]
        A handle to the process that is to receive the duplicated handle. 
		The handle must have the PROCESS_DUP_HANDLE access right.

     lpTargetHandle [out]
        A pointer to a variable that receives the duplicate handle. 
		This handle value is valid in the context of the target process.
        If hSourceHandle is a pseudo handle returned 
		by GetCurrentProcess or GetCurrentThread, 
		DuplicateHandle converts it to a real handle to a process or thread, respectively.
        If lpTargetHandle is NULL, the function duplicates the handle, 
		but does not return the duplicate handle value to the caller. 
		This behavior exists only for backward compatibility 
		with previous versions of this function. 
		You should not use this feature, as you will lose 
		system resources until the target process terminates.

     dwDesiredAccess [in]
        The access requested for the new handle. 
		For the flags that can be specified for each object type, 
		see the following Remarks section.
        This parameter is ignored if the dwOptions parameter specifies 
		the DUPLICATE_SAME_ACCESS flag. 
		Otherwise, the flags that can be specified depend on the type of 
		object whose handle is to be duplicated.

     bInheritHandle [in]
        A variable that indicates whether the handle is inheritable. 
		If TRUE, the duplicate handle can be inherited by 
		new processes created by the target process. 
		If FALSE, the new handle cannot be inherited.

     dwOptions [in]
        Optional actions. This parameter can be zero, 
		or any combination of the following values.

        Value	     Meaning
        DUPLICATE_CLOSE_SOURCE
        0x00000001   Closes the source handle. 
		             This occurs regardless of any error status returned.

        DUPLICATE_SAME_ACCESS
        0x00000002    Ignores the dwDesiredAccess parameter. 
		              The duplicate handle has the same 
					  access as the source handle.
    */
   DuplicateHandle(
	   GetCurrentProcess(), // Handle of process that thread
	                        // pseudohandle is relative to

	   GetCurrentThread(),  // Parent thread's pseudohandle
       GetCurrentProcess(), // Handle of process that the new, real,
	                        // thread handle is relative to
	   &hThreadParent,      // Will receive the new, real, handle 
	                        // identifying the parent thread
	   0,                   // Ignored due to DUPLICATE_SAME_ACCESS
	   FALSE,               // New thread handle is not inheritable
       DUPLICATE_SAME_ACCESS); // New thread handle has same 
                               // access as pseudohandle 

   CreateThread(NULL, 0, ChildThread, (PVOID)hThreadParent, 0, NULL);
   // Function continues...
}

void SuspendProcess(DWORD dwProcessID, BOOL fSuspend)
{
    /* Get the list of threads in the system. */
	/* Takes a snapshot of the specified processes, 
	 * as well as the heaps, modules, and threads used by these processes. 
	 *
	 * dwFlags [in] 
     *  The portions of the system to be included in the snapshot. 
	 *  This parameter can be one or more of the following values.
	 *
     *  TH32CS_SNAPHEAPLIST 0x00000001 
	 *  Includes all heaps of the process specified in th32ProcessID in the snapshot. 
	 *  To enumerate the heaps, see Heap32ListFirst.
	 *
	 *  TH32CS_SNAPTHREAD 0x00000004
	 *  Includes all threads in the system in the snapshot. 
	 *  To enumerate the threads, see Thread32First.
     *  To identify the threads that belong to a specific process, 
	 *  compare its process identifier to the th32OwnerProcessID 
	 *  member of the THREADENTRY32 structure when enumerating the threads
	 * 
	 * th32ProcessID [in] 
     *  The process identifier of the process to be included in the snapshot. 
	 *  This parameter can be zero to indicate the current process.
     *
	 */
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);
	if (hSnapshot != INVALID_HANDLE_VALUE)
	{
		/* Walk the list of threads */
		THREADENTRY32 te = { sizeof(te) };
		/* Retrieves information about the first thread 
		 * of any process encountered in a system snapshot. 
		 * hSnapshot [in] 
         *  A handle to the snapshot returned from a previous 
		 *  call to the CreateToolhelp32Snapshot function.
         * lpte [in, out] 
         *  A pointer to a THREADENTRY32 structure.
		 */
		BOOL fOk = Thread32First(hSnapshot, &te);
		for (; fOk; fOk = Thread32Next(hSnapshot, &te))
		{
            /* Is this thread in the desired process? */
            if (te.th32OwnerProcessID == dwProcessID)
			{
				/* Attempt to convert the thread ID into a handle. */
                /* Opens an existing thread object.
				 *
				 * dwDesiredAccess [in] 
                 *  The access to the thread object. This access right is checked against 
				 *  the security descriptor for the thread. 
				 *  This parameter can be one or more of the thread access rights.
				 *
                 * bInheritHandle [in] 
                 *  If this value is TRUE, processes created by this process will inherit the handle. 
				 *  Otherwise, the processes do not inherit this handle.
				 *
                 * dwThreadId [in] 
                 *   The identifier of the thread to be opened.
				 */ 
				HANDLE hThread = OpenThread(
					THREAD_SUSPEND_RESUME,
					FALSE,
					te.th32ThreadID);

				if (hThread != NULL)
				{
                    /* Suspend or resume the thread. */
					if (fSuspend)
						SuspendThread(hThread);
					else
                        ResumeThread(hThread);
				}
				CloseHandle(hThread);
			}
		}
        CloseHandle(hSnapshot);
	}
}

DWORD GetCPUFrequencyInMHz()
{
	LARGE_INTEGER m_liPerfFreq;
    LARGE_INTEGER m_liPerfStart;
    LARGE_INTEGER m_liPerfNow;
    LARGE_INTEGER m_liPerfEnd;

	__int64 qwElapsedTime;
	__int64 elapsedTime;
    __int64 elapsedTimeEnd;
	int currentPriority;
    unsigned __int64 cyclesOnStart;
    unsigned __int64 numberOfCycles;
	DWORD dwCPUFrequency;

	/* In the beginning we do a snapshot of CPU time 
	 * Retrieves the current value of the high-resolution performance counter. 
	 * lpPerformanceCount [out] 
     *   Type: LARGE_INTEGER*
     *   A pointer to a variable that receives 
	 *   the current performance-counter value, in counts. 
     *
	 */
    QueryPerformanceCounter(&m_liPerfFreq);
    QueryPerformanceCounter(&m_liPerfStart);

	/* Execute some complicated code */

    /* Get how much time has elapsed up to now */
    QueryPerformanceCounter(&m_liPerfNow);
	qwElapsedTime = 
		((m_liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) 
		   / m_liPerfFreq.QuadPart;

	/* Change the priority to ensure the thread will have more
	 * chances to be scheduled when Sleep() ends.
	 */
	currentPriority = GetThreadPriority(GetCurrentThread());
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
	/* keep track of the elapsed time with the other timer */
	elapsedTime = 0;

	/* Create a stopwatch timer (which defaults to the current time) */
	QueryPerformanceCounter(&m_liPerfNow);
    elapsedTime = 
	    ((m_liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000000)
		   / m_liPerfFreq.QuadPart;

	/* Get the current number of cycles */
	cyclesOnStart = ReadTimeStampCounter();
	/* Wait for ~1 second */
	Sleep(1000);
	/* Get the number of cycles after  */
	numberOfCycles = ReadTimeStampCounter() - cyclesOnStart;
	/* Get how much time has elapsed with greater precision */
    
	/* Get time after sleep.  */
	QueryPerformanceCounter(&m_liPerfEnd);
    elapsedTimeEnd = 
	    ((m_liPerfEnd.QuadPart - m_liPerfStart.QuadPart) * 1000000)
		   / m_liPerfFreq.QuadPart;
   
    /* Restore the thread priority */
    SetThreadPriority(GetCurrentThread(), currentPriority);
	/* Compute the frequency in MHz */
	dwCPUFrequency = (DWORD)(numberOfCycles / (elapsedTimeEnd - elapsedTime));
	return (dwCPUFrequency);
}

void GetCPUContext()
{
	HANDLE hThread;
	/* Create a CONTEXT structure. */
	CONTEXT Context;
	/* Tell the system that we are interested in only 
	 * the control registers.
	 */
	Context.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
    /* Tell the system to get the registers 
	 * associated with a thread.
	 */
	hThread = GetCurrentThread();
	/* Stop the thread from running. */
	SuspendThread(hThread);
	GetThreadContext(hThread, &Context);

    /* The control register members in the CONTEXT structure 
	 * reflect the thread's control registers. The other members
	 * are undefined.
	 */

    /* Make the instruction pointer point to the address of your choice.
	 * Here I have arbitrarily set the address instruction pointer to
	 * 0x0000000.
	 */
	Context.Eip = 0x00010000;
    /* Set the thread's registers to reflect
	 * the changed values. It's not really necessary to reset the ContextFlags 
	 * member because it was set earlier.
	 */   
	Context.ContextFlags = CONTEXT_CONTROL;
	SetThreadContext(hThread, &Context);
	/* Resuming the thread will cause it ti begin execution
	 * at address 0x00010000.
	 */
	ResumeThread(hThread);
}

/* Here we have some global variable */
long g_x = 0;

/* We need some explanation why this code is unsave 
 * and why we may receive unpredictable results
 * Let's translate this function into assembler code:
 * 
 * MOV EAX, [g_x] - this command reads the value of the g_x
 *                  variable from memory into EAX processor register
 * INC EAX        - processor increments EAX value 
 * MOV [g_x], EAX - writes the value from EAX processor register
 *                  into [g_x] operative memory address
 * 
 * If this code runs simultaneously we can anticipate 2 possible
 * situations:
 * 1. When these operations executes consequently:
 *
 *  Thread1 CPU1           Thread2 CPU2
 *
 *  MOV EAX, [g_x]  0
 *  INC EAX         1
 *  MOV [g_x], EAX  1            
 *                         MOV EAX, [g_x]  1
 *                         INC EAX         2 
 *                         MOV [g_x], EAX  2           
 *  In this case the value will be 2
 *
 * 2. When operations are executed simultaneously 
 *
 *  Thread 1 CPU1         Thread2 CPU2
 *  MOV EAX, [g_x]  0     
 *  INC EAX         1     MOV EAX, [g_x]  0     
 *  MOV [g_x], EAX  1     INC EAX         1
 *                        MOV [g_x], EAX  1
 *  The result of this operation will be 1
 */
DWORD WINAPI ThreadFunc1_UnSave(PVOID pvParam)
{
   g_x++;
   return (0);
}

DWORD WINAPI ThreadFunc2_UnSave(PVOID pvParam)
{
   g_x++;
   return (0);
}

DWORD WINAPI ThreadFunc1_Save(PVOID pvParam)
{
   InterlockedExchangeAdd(&g_x, 1);
   return (0);
}

DWORD WINAPI ThreadFunc2_Save(PVOID pvParam)
{
   InterlockedExchangeAdd(&g_x, 1);
   return (0);
}

/* Global variable indicating whether
 * a shared resource is in use or not
 */
BOOL g_fResourceInUse = FALSE;

void SpinLock()
{
   /* Sets a 32-bit variable to the specified value
    * as an atomic operation. 
	*
	* Target [in, out] 
    *    A pointer to the value to be exchanged. 
	*	 The function sets this variable to Value, 
	*    and returns its prior value.
	*
    * Value [in] 
    *    The value to be exchanged with 
	*    the value pointed to by Target.
	*
    * The function returns the initial 
	* value of the Target parameter.
	* 
	*/

   /* Wait to access the resource. 
    * We need to explain how spin lock works:
	* First thread:
	*  g_fResourceInUse is set to false. Then the thread
	*  atomically sets the variable to true and checks that
	*  the previous value was false and break the loop. Then 
	*  he does some work.
	* Meantime the second thread sets the variable to true and 
	* checks that the previous value was true and continue doing it
	* until the value will be set into false by another thread.
	* After accomplishing its work the first thread sets the variable
	* to false and indicates to other threads that the resource is 
	* free.
    */  
   while (InterlockedExchange(&g_fResourceInUse, TRUE) == TRUE)
      Sleep(0);

   /* Access the resource 
    * ...
    */

   /* We no longer need to access the resource. */
   InterlockedExchange(&g_fResourceInUse, FALSE);
}

LONG MInterlockedCompareExchange(
   PLONG plDestination,
   LONG lExchange,
   LONG lComparand)
{
	/* Original value */
	LONG lRet = *plDestination;
    if (*plDestination == lCompared)   
        *plDestination = lExchange;
	return (0);
}

LONGLONG MInterlockedAnd64(
   LONGLONG *Destination,
   LONGLONG Value)
{
	LONGLONG Old;

	do
	{
       Old = *Destination;
	}
	while (InterlockedCompareExchange64(Destination, Old & Value, Old) != Old);

	return Old;
}