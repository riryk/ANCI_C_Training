#include "mthread.h"
#include "Dbghelp.h"


/****
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

/****
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
///* This test method illustrates how to pass over a handle to some 
// * kernel object from one process to other process. This function 
// * must be called in a source process.
// * dwTProcessId - is an identifier of a target process identifier
// * where we are going to copy a handle to.
// */
//void DuplicateHandleToProcess(DWORD dwTProcessId)
//{
//	/* An uninitialized handle relative to process T */
//	HANDLE hObjInProcessT;
//	/* Some kernel object inside the process S. */
//    HANDLE hObjInProcessS = CreateMutex(NULL, FALSE, NULL);
//	/* Then we need to obtain a handle to a target process 
//	 * Opens an existing local process object.
//	 * dwDesiredAccess [in] - The access to the process object. 
//	 *    This access right is checked against the security descriptor for the process. 
//	 *    This parameter can be one or more of the process access rights.
//	 * bInheritHandle [in] - If this value is TRUE, processes created by this process 
//	 *    will inherit the handle. Otherwise, the processes do not inherit this handle.
//	 * dwProcessId [in] - The identifier of the local process to be opened.
//     *    If the specified process is the System Process (0x00000000), 
//	 *    the function fails and the last error code is ERROR_INVALID_PARAMETER. 
//	 *    If the specified process is the Idle process or one of the CSRSS processes, 
//	 *    this function fails and the last error code is ERROR_ACCESS_DENIED 
//	 *    because their access restrictions prevent user-level code from opening them.
//	 */
//	HANDLE processT = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwTProcessId);
//    /* This function duplicates the handle into the target process handle table
//	 * and returns a reference to this handle. And we need to use some kind of 
//	 * inter process communication to notify the target process about the fact
//	 * that the handle is now available for him.
//	 */
//	DuplicateHandle(
//		GetCurrentProcess(),
//        hObjInProcessS,
//        dwTProcessId,
//		&hObjInProcessT,
//		0,
//		FALSE,
//		DUPLICATE_SAME_ACCESS);
//	/* Use some kind or IPC mechanism to get the handle value of hObjInProcessS into 
//	 * process T. We no longer need to communicate with Process T.
//	 */
//    CloseHandle(processT);
//	/* When process S no longer needs to use the mutex, it should close it. */
//	CloseHandle(hObjInProcessS);
//}
///* Gets the base address of the running application */ 
//void DumpModule()
//{
//    /* Get the base address of the running application.
//	 * Can be different from the running module if this code is in a DLL.
//	 */
//    HMODULE hModule = GetModuleHandle(NULL);
//	_tprintf(TEXT("with GetModuleHandle(NULL) = 0x%x\r\n"), hModule);
//	/* Pass the address of the current method DumpModule
//	 * as parameter to GetModuleHandleEx to get the address
//	 * of the current module hModule/hInstance.
//	 */
//    hModule = NULL;
//	GetModuleHandleEx(
//       GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
//	   (PCTSTR)DumpModule,
//	   &hModule);
//	_tprintf(TEXT("with GetModuleHandleEx = 0x%x\r\n"), hModule);
//}
//
///* Parses environment strings which have a pattern:
// * =::=::\
// * =...
// * var=value\0
// * ...
// * var=value\0\0
// */
//void DumpEnvStrings()
//{
//	PTSTR pEnvBlock = GetEnvironmentStrings();
//	TCHAR szName[MAX_PATH];
//	TCHAR szValue[MAX_PATH];
//	PTSTR pszCurrent = pEnvBlock;
//	HRESULT hr = S_OK;
//	PCTSTR pszPos = NULL;
//	int current;
//    size_t cbNameLength;
//
//    while (pszCurrent != NULL)
//	{
//        /* Skip the meanigless strings like:
//		 * "=::=::\"
//		 */ 
//		if (*pszCurrent != TEXT('='))
//		{
//			/* Look for '=' separator */
//            pszPos = _tcschr(pszCurrent, TEXT('='));
//			/* Point now to the first character of the value */
//            pszPos++;
//			/* Copy the variable name 
//			 * The length without '='
//			 */
//            cbNameLength = (size_t)pszPos - (size_t)pszCurrent - sizeof(TCHAR);
//            hr = StringCbCopyN(szName, MAX_PATH, pszCurrent, cbNameLength);
//			if (FAILED(hr))
//				break;
//            /* Copy the variable value with the last NULL character 
//			 * and allow truncation because this is for UI only.
//			 */
//			hr = StringCchCopyN(szValue, MAX_PATH, pszPos, _tcslen(pszPos) + 1);
//			if (SUCCEEDED(hr))
//			{
//				_tprintf(TEXT("[%u] %s=%s\r\n"), current, szName, szValue);
//			}
//			else if (hr = STRSAFE_E_INSUFFICIENT_BUFFER)
//			{
//                _tprintf(TEXT("[%u] %s=%s...\r\n"), current, szName, szValue);
//			}
//			else
//			{
//                _tprintf(TEXT("[%u] %s=???\r\n"), current, szName);
//				break;
//			}
//		}
//		else
//		{
//            _tprintf(TEXT("[%u] %s\r\n"), current, pszCurrent);
//		}
//		/* Next variable please */
//		current++;
//		/* Move to the end of the string */
//		while (*pszCurrent != TEXT('\0'))
//			pszCurrent++;
//		pszCurrent++;
//		/* Check if it was not the last string. */
//		if (*pszCurrent == TEXT('\0'))
//			break;
//	}
//	/* Do not forget to to free the memory. */
//	FreeEnvironmentStrings(pEnvBlock);
//}
//
//void PrintEnvironmentVariable(PCTSTR pszVariableName)
//{
//	PTSTR pszValue = NULL;
//	/* Get the size of the buffer that is required to store the value */
//	DWORD dwResult = GetEnvironmentVariable(pszVariableName, pszValue, 0);
//	if (dwResult != 0)
//	{
//		/* Allocate the buffer to store the environment variable value */
//        DWORD size = dwResult * sizeof(TCHAR);
//        pszValue = (PTSTR)malloc(size);
//		GetEnvironmentVariable(pszVariableName, pszValue, size);
//		_tprintf(TEXT("%s=%s\n"), pszVariableName, pszValue);
//		free(pszValue);
//	}
//	else
//	{
//        _tprintf(TEXT("'%s'=<unknown value>\n"), pszVariableName);
//	}
//}
//
//void SpawnChildProcesses()
//{
//	STARTUPINFO si = { sizeof(si) };
//
//	SECURITY_ATTRIBUTES saProcess;
//    SECURITY_ATTRIBUTES saThread;
//
//	PROCESS_INFORMATION piProcessB;
//    PROCESS_INFORMATION piProcessC;
//    
//    TCHAR szPath[MAX_PATH];
//	/* Prepare to spawn Process B from Process A.
//	 * The handle identifying the new process
//	 * object should be inheritable.
//	 */
//	saProcess.nLength = sizeof(saProcess);
//	saProcess.lpSecurityDescriptor = NULL;
//	saProcess.bInheritHandle = TRUE;
//    /* The handle identifying the new thread
//	 * object should not be inheritable.
//	 */
//	saThread.nLength = sizeof(saThread);
//	saThread.lpSecurityDescriptor = NULL;
//	saThread.bInheritHandle = FALSE;
//	/* Spawn Process B */
//	_tcscpy_s(szPath, _countof(szPath), TEXT("cmd.exe")); 
//	CreateProcess(
//		NULL,
//		szPath,
//		&saProcess,
//		&saThread,
//		FALSE,
//		0,
//		NULL,
//		NULL,
//		&si,
//        &piProcessB);
//    _tcscpy_s(szPath, _countof(szPath), TEXT("cmd.exe")); 
//	CreateProcess(
//		NULL,
//		szPath,
//		NULL,
//        NULL,
//		TRUE,
//		0,
//        NULL,
//		NULL,
//		&si,
//		&piProcessC);
//	return 0;
//}
//
//void SpawnChildProcessAndWaitUntilItFinishes()
//{
//	STARTUPINFO si = { sizeof(si) };
//	PROCESS_INFORMATION pi;
//	DWORD dwExitCode;
//    TCHAR szPath[MAX_PATH];
//    BOOL fSuccess;
//
//    _tcscpy_s(szPath, _countof(szPath), TEXT("cmd.exe")); 
//     fSuccess = 
//	  CreateProcess(
//		NULL,
//		szPath,
//		NULL,
//        NULL,
//		TRUE,
//		0,
//        NULL,
//		NULL,
//		&si,
//		&pi);
//
//	 if (fSuccess)
//	 {
//		 /* Close the thread handle as soon as it is no longer needed */
//		 CloseHandle(&pi.hThread);
//		 /* Suspend our execution until the child has terminated */
//		 WaitForSingleObject(pi.hProcess, INFINITE);
//		 /* The child process terminated; get its exit code */
//		 GetExitCodeProcess(pi.hProcess, &dwExitCode);
//         /* Close the process handle as soon as it is no longer needed */
//		 CloseHandle(pi.hProcess);
//	 }
//}
//
//void RunProcessAsAdmin()
//{
//	/* Initialize the structure */
//	SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
//	/* Ask for privileges elevation */
//	sei.lpVerb = TEXT("runas");
//	/* Create a Command Promt from which you will be able to start
//	 * other elevated applications.
//	 */
//    sei.lpFile = TEXT("cmd.exe");
//	/* Don't forget this parameter; otherwise, the window will be hidden */
//	sei.nShow = SW_SHOWNORMAL;
//	/* Elevate privileges for running this application */
//    if (!ShellExecuteEx(&sei))
//	{
//       DWORD dwStatus = GetLastError();  
//	   if (dwStatus == ERROR_CANCELLED)
//	   {
//		   /* The user refused to allow privileges elevation */
//	   }
//	   else if (dwStatus == ERROR_FILE_NOT_FOUND)
//	   {
//          /* The file defined by lpFile was not found and 
//		   * an error message popped up.
//		   */ 
//	   }
//	}
//}
//
//BOOL GetProcessElevation(TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin)
//{
//    HANDLE hToken = NULL;
//	DWORD dwSize;
//	BOOL bResult = FALSE;
//	/* Get current process token */
//	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
//	{
//       return (FALSE); 
//	}
//    /* Retrieve elevation type information */
//	if (GetTokenInformation(
//		 hToken,
//		 TokenElevationType,
//		 pElevationType, 
//		 sizeof(TOKEN_ELEVATION_TYPE),
//		 &dwSize))
//	{
//        /* Create the SID corresponding to the Administrators group */ 
//        BYTE adminSID[SECURITY_MAX_SID_SIZE];
//        dwSize = sizeof(adminSID);
//		CreateWellKnownSid(
//            WinBuiltinAdministratorsSid,
//			NULL,
//			&adminSID,
//			&dwSize);
//
//        if (*pElevationType == TokenElevationTypeLimited)
//		{
//			/* Get handle to linked token (will have one if we are lua) */
//			HANDLE hUnfilteredToken = NULL;
//
//            GetTokenInformation(
//				hToken,
//				TokenLinkedToken,
//				(VOID*)&hUnfilteredToken,
//				sizeof(HANDLE),
//				&dwSize);
//			/* Check if this original token contains SID */
//            if (CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin))
//			{
//                bResult = TRUE; 
//			}
//			/* Close the unfiltered token */
//			CloseHandle(hUnfilteredToken);
//		}
//		else
//		{
//			*pIsAdmin = IsUserAnAdmin();
//            bResult = TRUE; 
//		}
//	}
//	CloseHandle(hToken);
//	return (bResult);
//}
//
///* Return values are: 
// *  1. means that the process is inside a job.
// *  2. 
// */
//int StartRestrictedProcess()
//{
//   /* Check if we are not already associated with a job.
//    * If this is the case, there is no way to switch to 
//	* another job.
//    */
//    BOOL bInJob = FALSE;
//	HANDLE hjob;
//	/* Place some restrictions on processes in the job. 
//	 * First, set some basic restrictions.
//	 */
//	JOBOBJECT_BASIC_LIMIT_INFORMATION jobli = { 0 };
//	/* Second, set some UI restrictions. */
//	JOBOBJECT_BASIC_UI_RESTRICTIONS jobuir;
//	STARTUPINFO si = { sizeof(si) };
//	PROCESS_INFORMATION pi;
//    TCHAR szCmdLine[8];
//	BOOL bResult;
//
//	FILETIME CreationTime;
//    FILETIME ExitTime;
//    FILETIME KernelTime;
//    FILETIME UserTime;
//    TCHAR szInfo[MAX_PATH];
//
//    DWORD dw;
//    HANDLE h[2];
//
//	/* Determines whether the process is running in the specified job. 
//	 * ProcessHandle [in]
//	 *   A handle to the process to be tested. 
//	 *   The handle must have the PROCESS_QUERY_INFORMATION 
//	 *   or PROCESS_QUERY_LIMITED_INFORMATION access right. 
//	 *   For more information, see Process Security and Access Rights.
//	 * JobHandle [in, optional]
//     *   A handle to the job. If this parameter is NULL, 
//	 *   the function tests if the process is running under any job.
//     *   If this parameter is not NULL, the handle must have the JOB_OBJECT_QUERY access right. 
//	 *   For more information, see Job Object Security and Access Rights.
//     * Result [out]
//     *   A pointer to a value that receives TRUE 
//	 *   if the process is running in the job, and FALSE otherwise.
//	 */
//    IsProcessInJob(GetCurrentProcess(), NULL, &bInJob);
//	if (bInJob)
//	{
//	   return 1;   
//	}
//	/* Create a job kernel object. */
//    hjob = CreateJobObject(NULL, TEXT("Wintellect_RestrictedProcessJob"));
//	/* The process always runs in the idle priority class. */
//	jobli.PriorityClass = IDLE_PRIORITY_CLASS;
//	/* The job cannot use more than 1 second of CPU time. */
//	jobli.PerJobUserTimeLimit.QuadPart = 10 * 1000;
//	/* These are the only 2 restrictions I want placed on the job (process). */
//	jobli.LimitFlags = JOB_OBJECT_LIMIT_PRIORITY_CLASS
//		| JOB_OBJECT_LIMIT_JOB_TIME;
//	/* Sets limits for a job object. */
//	SetInformationJobObject(
//		hjob, 
//		JobObjectBasicLimitInformation,
//	    &jobli,
//		sizeof(jobli));
//	/* A fancy zero */
//	jobuir.UIRestrictionsClass = JOB_OBJECT_UILIMIT_NONE;
//	/* The process can't log off the system. */
//	jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_EXITWINDOWS;
//	/* The process can't access USER objects (such as other windows)
//	 * in the system.   
//	 */
//	jobuir.UIRestrictionsClass |= JOB_OBJECT_UILIMIT_HANDLES;
//    /* Sets limits for a job object.
//	 */
//	SetInformationJobObject(
//		hjob, 
//		JobObjectBasicUIRestrictions, 
//		&jobuir,
//		sizeof(jobuir));
//	/* Spawn the process that is to be in the job.
//	 * Note: 
//	 *   You must first spawn the process and then place the process
//	 *   in the job. This means that the process' thread must be initially
//	 *   suspended so that it can't so that it can't execute any code 
//	 *   outside of the job's restrictions.
//	 */
//	_tcscpy_s(szCmdLine, _countof(szCmdLine), TEXT("CMD"));
//	bResult = 
//		CreateProcess(
//		   NULL,
//		   szCmdLine,
//		   NULL,
//		   NULL,
//		   FALSE,
//		   CREATE_SUSPENDED | CREATE_NEW_CONSOLE, 
//		   NULL,
//		   NULL,
//		   &si,
//		   &pi);
//	/* Place the process in the job.
//	 * Note: If this process spawns any children, the children
//	 * are automatically part of the same job.
//	 */
//	AssignProcessToJobObject(hjob, pi.hProcess);
//	/* Now we can allow the child process' thread to execute code */
//	ResumeThread(pi.hThread);
//	/* Close handle */
//	CloseHandle(pi.hThread);
//	/* Wailt for the process to terminate or
//	 * for all the job's CPU time to be used.
//	 */
//	h[0] = pi.hProcess;
//    h[1] = hjob;
//
//    dw = WaitForMultipleObjects(2, h, FALSE, INFINITE);
//	switch (dw - WAIT_OBJECT_0)
//	{
//	case 0:
//		printf("The process has terminated...");
//		break;
//	case 1:
//        printf("All of the job's allotted CPU time was used...");
//		break;
//	}
//
//	/* Get timing information about the processes */
//	GetProcessTimes(
//		pi.hProcess,
//		&CreationTime,
//		&ExitTime,
//		&KernelTime,
//		&UserTime);
//	StringCchPrintf(
//		szInfo,
//		_countof(szInfo),
//		TEXT("Kernel = %u | User = %u\n"),
//		KernelTime.dwLowDateTime / 10000,
//		UserTime.dwLowDateTime / 10000);
//	CloseHandle(pi.hProcess);
//	CloseHandle(hjob);
//}
//
//void EnumProcessIdsInJob(HANDLE hjob)
//{
//   /* I assume that there will never be more 
//    * 10 processes in this job.
//    */
//   #define MAX_PROCESS_IDS 10
//   DWORD x;
//   /* Calculate the number of bytes needed for
//    * structure and process IDs.
//    */
//   DWORD cb = sizeof(JOBOBJECT_BASIC_PROCESS_ID_LIST) + 
//	   (MAX_PROCESS_IDS - 1) * sizeof(DWORD);
//   /* Allocate the block of memory. */
//   PJOBOBJECT_BASIC_PROCESS_ID_LIST pjobpil = 
//	   (PJOBOBJECT_BASIC_PROCESS_ID_LIST)_alloca(cb);
//   /* Tell the function the maximum number of processes
//    * that we allocated space for.
//    */
//   pjobpil->NumberOfAssignedProcesses = MAX_PROCESS_IDS;
//   /* Request the current set of process IDs. 
//    * Retrieves limit and job state information from the job object.
//	* hJob [in, optional]
//    *    A handle to the job whose information is being queried.
//	*    If this value is NULL and the calling process is associated 
//	*    with a job, the job associated with the calling process is used. 
//	*    If the job is nested, the immediate job of the calling process is used.   
//	* obObjectInfoClass [in]
//    *    The information class for the limits to be queried. 
//	*    This parameter can be one of the following values.
//	*    JobObjectBasicProcessIdList = 3
//	*      The lpJobObjectInfo parameter is a pointer 
//	*      to a JOBOBJECT_BASIC_PROCESS_ID_LIST structure.
//    * pJobObjectInfo [out]
//    *    The limit or job state information. 
//	*    The format of this data depends on the value 
//	*    of the JobObjectInfoClass parameter.     
//	* cbJobObjectInfoLength [in]
//    *    The count of the job information being queried, in bytes. 
//	*    This value depends on the value of the JobObjectInfoClass parameter.
//	* lpReturnLength [out, optional]
//    *    A pointer to a variable that receives the length of data 
//	*    written to the structure pointed to by the lpJobObjectInfo parameter. 
//	*    If you do not want to receive this information, specify NULL.
//    */
//   QueryInformationJobObject(
//	   hjob,
//	   JobObjectBasicProcessIdList,
//	   pjobpil,
//	   cb,
//	   &cb);
//   /* Enumerate the process IDs */
//   for (x = 0; x < pjobpil->NumberOfProcessIdsInList; x++)
//   {
//	   /* Use pjobpil->ProcessIdList[x]... */
//   }
//   /* Since _alloca was used to allocate the memory,
//    * we do not need to free it here.
//    */
//}
//
//DWORD WINAPI SecondThread(PVOID pvParam)
//{
//   /* Do something lengthing here. ... 
//    * Attempt to access the variable on the FirstThread's stack.
//	* NOTE: This may cause an access violation - it depends on timing!
//    */
//   int* xInt = (int*)pvParam;
//   *xInt = 5; // Access violation exception.
//}
//
//DWORD WINAPI FirstThread(PVOID pvParam)
//{
//   /* Initialize a stack-based variable */
//   int x = 0;
//   DWORD dwThreadID;
//   /* Create a new thread */
//   HANDLE hThread = 
//	   CreateThread(
//          NULL,
//		  0,
//		  SecondThread,
//          (PVOID)&x,
//          0,
//		  &dwThreadID);
//   /* We do not reference the new thread anymore,
//    * so close our handle to it.
//    */
//   CloseHandle(hThread);
//   /* Our thread is done.
//    * BUG: our stack will be destroyed, but Second thread
//	* might try to access it.
//    */
//   return (0);
//}
//
//struct _MTidData
//{
//   unsigned long _tid; /* thread ID */  
//   unsigned long _thandle; /* thread handle */
//   int _terrno; /* errno value */
//   unsigned long _tdoserrno; /* _doerrno value */
//   void* _initaddr;
//   void* _initarg;
//};
//
//typedef struct _MTidData * _PMidData;
//
//void _mEndThreadEx(unsigned retcode)
//{
//	/* Pointer to thread's data block */
//    _PMidData ptd;
//	/* Clean up floating-point support (code not shown).
//	 * Get the address of this thread's tiddata block.
//   	 */
//	//ptd = _getptd_noexit();
//	/* Free the tiddata block. */
//    if (ptd != NULL)
//		//_freeptd(ptd);
//	/* Terminate the thread. */
//	ExitThread(retcode);
//}
//
//void _mCallThreadStartex()
//{
//	unsigned threadRetCode;
//	void* initArg;
//    unsigned (*func)(void*);
//	/* pointer to thread's _MTidData structure */
//	_PMidData ptd;
//    /* get the pointer to thread data from TLS 
//	 * This function inside uses
//	 * LPVOID WINAPI TlsGetValue(_In_  DWORD dwTlsIndex);
//     * dwTlsIndex [in] 
//     *   The TLS index that was allocated by the TlsAlloc function.
//	 * If the function succeeds, the return value is 
//	 * the value stored in the calling thread's 
//	 * TLS slot associated with the specified index.
//	 */
//    ptd = _getptd();
//	/* Wrap desired thread function in SEH frame to
//	 * handle run-time errors and signal support
//	 */
//	__try
//	{
//       /* Call desired thread function, passing it the desired parameter. 
//	    * Pass thread's exit code value to _endthreadex.
//	    */
//		initArg = ptd->_initarg;
//		func = ptd->_initaddr;
//		threadRetCode = func(initArg);
//
//	   _mEndThreadEx(threadRetCode);
//	   /* 
//        *  GetExceptionCode()
//	    * Retrieves a code that identifies the type of exception that occurs. 
//	    * The function can be called only from within the filter expression 
//		* or exception-handler block of an exception handler.
//	    *  
//		*  GetExceptionInformation()
//        * Retrieves a computer-independent description of an exception, 
//		* and information about the computer state that exists for the thread when the exception occurs. 
//		* This function can be called only from within the filter expression of an exception handler.
//	    */
//	}
//	__except(_XcptFilter(GetExceptionCode(), GetExceptionInformation()))
//	{
//        /* The C run-time's exception handler deals with run-time errors
//		 * and signal support; we should never get it here.
//		 */ 
//		_exit(GetExceptionCode());
//	}
//}
//
//unsigned long WINAPI _mThreadStartex (void* ptd)
//{
//   /* Note: ptd is the address of this thread's tiddata block.
//    * Assosiate the tiddata block with this thread so
//	* _getptd() will be able to find it in _callthreadstartex.
//    */
//
//	/* Stores a value in the calling thread's thread 
//	 * local storage (TLS) slot for the specified TLS index. 
//	 * Each thread of a process has its own slot for each TLS index. 
//	 * 
//	 * dwTlsIndex [in] 
//     *   The TLS index that was allocated by the TlsAlloc function.
//     * 
//	 * lpTlsValue [in, optional] 
//     *   The value to be stored in the calling thread's TLS slot for the index.
//	 */
//
//	//TlsSetValue(__tlsindex, ptd);
//
//	/* Save this thread ID in the _tiddata block */
//	((_PMidData)ptd)->_tid = GetCurrentThreadId();
//   	/* Initialize floating-point support () 
//	 * call helper function.
//	 */
//	_mCallThreadStartex();
//	/* We never get here; the thread dies in _callthreadstartex() */
//}
//
//uintptr_t __cdecl _mBeginThread
//(
//  void* psa,
//  unsigned cbStackTrace,
//  unsigned (__stdcall *pfnStartAddr) (void*),
//  void* pvParam,
//  unsigned dwCreateFlags,
//  unsigned *pdwThreadID
//)
//{
//   _PMidData ptd; /* Pointer to thread's data block */
//   uintptr_t thdl; /* Thread's handle */
//   /* Allocate data block for the new thread */
//   ptd = (_PMidData)malloc(sizeof(_PMidData));
//   if (ptd == NULL)
//   {
//      goto error_return;
//   }
//   /* Initialize the data block. */
//
//   /* DWORD WINAPI TlsAlloc(void); 
//    *
//    *  Allocates a thread local storage (TLS) index. 
//	*  Any thread of the process can subsequently use this index 
//	*  to store and retrieve values that are local to the thread, 
//	*  because each thread receives its own slot for the index.
//	*
//    */
//
//   _initptd(ptd);
//   /* Save the desired thread function and the parameter
//    * we want it to get in the data block.
//    */
//   ptd->_initaddr = (void*)pfnStartAddr;
//   ptd->_initarg = pvParam;
//   ptd->_thandle = (uintptr_t)(-1);
//   /* Create the new thread. */
//   thdl = (uintptr_t)CreateThread(
//	        (LPSECURITY_ATTRIBUTES)psa,
//             cbStackTrace,
//			 _mThreadStartex,
//             (PVOID)ptd,
//			 dwCreateFlags,
//             pdwThreadID);
//     
//   if (thdl == 0)
//   {
//       /* Thread couldn't be created, cleanup and return failure. */
//	   goto error_return;
//   }
//   /* Thread created OK, return the handle as unsigned long. */ 
//   return (thdl);
//error_return:
//   /* 
//    * Error: data or thread couldn't be created.
//	* GetLastError() is mapped into errno corresponding values
//	* if something wrong happened in CreateThread.
//    */
//   free(ptd);
//   return ((uintptr_t)0L);
//}
//
//DWORD WINAPI ChildThread(PVOID pvParam)
//{
//   HANDLE hThreadParent = (HANDLE)pvParam;
//   FILETIME ftCreationTime, ftExitTime, ftKernelTime, ftUserTime;
//   GetThreadTimes(hThreadParent,
//      &ftCreationTime, &ftExitTime, &ftKernelTime, &ftUserTime);
//   CloseHandle(hThreadParent);
//   // Function continues...
//}
//
//DWORD WINAPI ParentThread(PVOID pvParam)
//{
//   HANDLE hThreadParent;
//   /*
//     hSourceProcessHandle [in]
//        A handle to the process with the handle to be duplicated.
//        The handle must have the PROCESS_DUP_HANDLE access right. 
//		For more information, see Process Security and Access Rights.
//
//     hSourceHandle [in]
//        The handle to be duplicated. This is an open object handle 
//		that is valid in the context of the source process. 
//		For a list of objects whose handles can be duplicated, 
//		see the following Remarks section.
//
//     hTargetProcessHandle [in]
//        A handle to the process that is to receive the duplicated handle. 
//		The handle must have the PROCESS_DUP_HANDLE access right.
//
//     lpTargetHandle [out]
//        A pointer to a variable that receives the duplicate handle. 
//		This handle value is valid in the context of the target process.
//        If hSourceHandle is a pseudo handle returned 
//		by GetCurrentProcess or GetCurrentThread, 
//		DuplicateHandle converts it to a real handle to a process or thread, respectively.
//        If lpTargetHandle is NULL, the function duplicates the handle, 
//		but does not return the duplicate handle value to the caller. 
//		This behavior exists only for backward compatibility 
//		with previous versions of this function. 
//		You should not use this feature, as you will lose 
//		system resources until the target process terminates.
//
//     dwDesiredAccess [in]
//        The access requested for the new handle. 
//		For the flags that can be specified for each object type, 
//		see the following Remarks section.
//        This parameter is ignored if the dwOptions parameter specifies 
//		the DUPLICATE_SAME_ACCESS flag. 
//		Otherwise, the flags that can be specified depend on the type of 
//		object whose handle is to be duplicated.
//
//     bInheritHandle [in]
//        A variable that indicates whether the handle is inheritable. 
//		If TRUE, the duplicate handle can be inherited by 
//		new processes created by the target process. 
//		If FALSE, the new handle cannot be inherited.
//
//     dwOptions [in]
//        Optional actions. This parameter can be zero, 
//		or any combination of the following values.
//
//        Value	     Meaning
//        DUPLICATE_CLOSE_SOURCE
//        0x00000001   Closes the source handle. 
//		             This occurs regardless of any error status returned.
//
//        DUPLICATE_SAME_ACCESS
//        0x00000002    Ignores the dwDesiredAccess parameter. 
//		              The duplicate handle has the same 
//					  access as the source handle.
//    */
//   DuplicateHandle(
//	   GetCurrentProcess(), // Handle of process that thread
//	                        // pseudohandle is relative to
//
//	   GetCurrentThread(),  // Parent thread's pseudohandle
//       GetCurrentProcess(), // Handle of process that the new, real,
//	                        // thread handle is relative to
//	   &hThreadParent,      // Will receive the new, real, handle 
//	                        // identifying the parent thread
//	   0,                   // Ignored due to DUPLICATE_SAME_ACCESS
//	   FALSE,               // New thread handle is not inheritable
//       DUPLICATE_SAME_ACCESS); // New thread handle has same 
//                               // access as pseudohandle 
//
//   CreateThread(NULL, 0, ChildThread, (PVOID)hThreadParent, 0, NULL);
//   // Function continues...
//}
//
//void SuspendProcess(DWORD dwProcessID, BOOL fSuspend)
//{
//    /* Get the list of threads in the system. */
//	/* Takes a snapshot of the specified processes, 
//	 * as well as the heaps, modules, and threads used by these processes. 
//	 *
//	 * dwFlags [in] 
//     *  The portions of the system to be included in the snapshot. 
//	 *  This parameter can be one or more of the following values.
//	 *
//     *  TH32CS_SNAPHEAPLIST 0x00000001 
//	 *  Includes all heaps of the process specified in th32ProcessID in the snapshot. 
//	 *  To enumerate the heaps, see Heap32ListFirst.
//	 *
//	 *  TH32CS_SNAPTHREAD 0x00000004
//	 *  Includes all threads in the system in the snapshot. 
//	 *  To enumerate the threads, see Thread32First.
//     *  To identify the threads that belong to a specific process, 
//	 *  compare its process identifier to the th32OwnerProcessID 
//	 *  member of the THREADENTRY32 structure when enumerating the threads
//	 * 
//	 * th32ProcessID [in] 
//     *  The process identifier of the process to be included in the snapshot. 
//	 *  This parameter can be zero to indicate the current process.
//     *
//	 */
//    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessID);
//	if (hSnapshot != INVALID_HANDLE_VALUE)
//	{
//		/* Walk the list of threads */
//		THREADENTRY32 te = { sizeof(te) };
//		/* Retrieves information about the first thread 
//		 * of any process encountered in a system snapshot. 
//		 * hSnapshot [in] 
//         *  A handle to the snapshot returned from a previous 
//		 *  call to the CreateToolhelp32Snapshot function.
//         * lpte [in, out] 
//         *  A pointer to a THREADENTRY32 structure.
//		 */
//		BOOL fOk = Thread32First(hSnapshot, &te);
//		for (; fOk; fOk = Thread32Next(hSnapshot, &te))
//		{
//            /* Is this thread in the desired process? */
//            if (te.th32OwnerProcessID == dwProcessID)
//			{
//				/* Attempt to convert the thread ID into a handle. */
//                /* Opens an existing thread object.
//				 *
//				 * dwDesiredAccess [in] 
//                 *  The access to the thread object. This access right is checked against 
//				 *  the security descriptor for the thread. 
//				 *  This parameter can be one or more of the thread access rights.
//				 *
//                 * bInheritHandle [in] 
//                 *  If this value is TRUE, processes created by this process will inherit the handle. 
//				 *  Otherwise, the processes do not inherit this handle.
//				 *
//                 * dwThreadId [in] 
//                 *   The identifier of the thread to be opened.
//				 */ 
//				HANDLE hThread = OpenThread(
//					THREAD_SUSPEND_RESUME,
//					FALSE,
//					te.th32ThreadID);
//
//				if (hThread != NULL)
//				{
//                    /* Suspend or resume the thread. */
//					if (fSuspend)
//						SuspendThread(hThread);
//					else
//                        ResumeThread(hThread);
//				}
//				CloseHandle(hThread);
//			}
//		}
//        CloseHandle(hSnapshot);
//	}
//}
//
//DWORD GetCPUFrequencyInMHz()
//{
//	LARGE_INTEGER m_liPerfFreq;
//    LARGE_INTEGER m_liPerfStart;
//    LARGE_INTEGER m_liPerfNow;
//    LARGE_INTEGER m_liPerfEnd;
//
//	__int64 qwElapsedTime;
//	__int64 elapsedTime;
//    __int64 elapsedTimeEnd;
//	int currentPriority;
//    unsigned __int64 cyclesOnStart;
//    unsigned __int64 numberOfCycles;
//	DWORD dwCPUFrequency;
//
//	/* In the beginning we do a snapshot of CPU time 
//	 * Retrieves the current value of the high-resolution performance counter. 
//	 * lpPerformanceCount [out] 
//     *   Type: LARGE_INTEGER*
//     *   A pointer to a variable that receives 
//	 *   the current performance-counter value, in counts. 
//     *
//	 */
//    QueryPerformanceCounter(&m_liPerfFreq);
//    QueryPerformanceCounter(&m_liPerfStart);
//
//	/* Execute some complicated code */
//
//    /* Get how much time has elapsed up to now */
//    QueryPerformanceCounter(&m_liPerfNow);
//	qwElapsedTime = 
//		((m_liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000) 
//		   / m_liPerfFreq.QuadPart;
//
//	/* Change the priority to ensure the thread will have more
//	 * chances to be scheduled when Sleep() ends.
//	 */
//	currentPriority = GetThreadPriority(GetCurrentThread());
//	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
//	/* keep track of the elapsed time with the other timer */
//	elapsedTime = 0;
//
//	/* Create a stopwatch timer (which defaults to the current time) */
//	QueryPerformanceCounter(&m_liPerfNow);
//    elapsedTime = 
//	    ((m_liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000000)
//		   / m_liPerfFreq.QuadPart;
//
//	/* Get the current number of cycles */
//	cyclesOnStart = ReadTimeStampCounter();
//	/* Wait for ~1 second */
//	Sleep(1000);
//	/* Get the number of cycles after  */
//	numberOfCycles = ReadTimeStampCounter() - cyclesOnStart;
//	/* Get how much time has elapsed with greater precision */
//    
//	/* Get time after sleep.  */
//	QueryPerformanceCounter(&m_liPerfEnd);
//    elapsedTimeEnd = 
//	    ((m_liPerfEnd.QuadPart - m_liPerfStart.QuadPart) * 1000000)
//		   / m_liPerfFreq.QuadPart;
//   
//    /* Restore the thread priority */
//    SetThreadPriority(GetCurrentThread(), currentPriority);
//	/* Compute the frequency in MHz */
//	dwCPUFrequency = (DWORD)(numberOfCycles / (elapsedTimeEnd - elapsedTime));
//	return (dwCPUFrequency);
//}
//
//void GetCPUContext()
//{
//	HANDLE hThread;
//	/* Create a CONTEXT structure. */
//	CONTEXT Context;
//	/* Tell the system that we are interested in only 
//	 * the control registers.
//	 */
//	Context.ContextFlags = CONTEXT_CONTROL | CONTEXT_INTEGER;
//    /* Tell the system to get the registers 
//	 * associated with a thread.
//	 */
//	hThread = GetCurrentThread();
//	/* Stop the thread from running. */
//	SuspendThread(hThread);
//	GetThreadContext(hThread, &Context);
//
//    /* The control register members in the CONTEXT structure 
//	 * reflect the thread's control registers. The other members
//	 * are undefined.
//	 */
//
//    /* Make the instruction pointer point to the address of your choice.
//	 * Here I have arbitrarily set the address instruction pointer to
//	 * 0x0000000.
//	 */
//	Context.Eip = 0x00010000;
//    /* Set the thread's registers to reflect
//	 * the changed values. It's not really necessary to reset the ContextFlags 
//	 * member because it was set earlier.
//	 */   
//	Context.ContextFlags = CONTEXT_CONTROL;
//	SetThreadContext(hThread, &Context);
//	/* Resuming the thread will cause it ti begin execution
//	 * at address 0x00010000.
//	 */
//	ResumeThread(hThread);
//}
//
///* Here we have some global variable */
//long g_x = 0;
//
///* We need some explanation why this code is unsave 
// * and why we may receive unpredictable results
// * Let's translate this function into assembler code:
// * 
// * MOV EAX, [g_x] - this command reads the value of the g_x
// *                  variable from memory into EAX processor register
// * INC EAX        - processor increments EAX value 
// * MOV [g_x], EAX - writes the value from EAX processor register
// *                  into [g_x] operative memory address
// * 
// * If this code runs simultaneously we can anticipate 2 possible
// * situations:
// * 1. When these operations executes consequently:
// *
// *  Thread1 CPU1           Thread2 CPU2
// *
// *  MOV EAX, [g_x]  0
// *  INC EAX         1
// *  MOV [g_x], EAX  1            
// *                         MOV EAX, [g_x]  1
// *                         INC EAX         2 
// *                         MOV [g_x], EAX  2           
// *  In this case the value will be 2
// *
// * 2. When operations are executed simultaneously 
// *
// *  Thread 1 CPU1         Thread2 CPU2
// *  MOV EAX, [g_x]  0     
// *  INC EAX         1     MOV EAX, [g_x]  0     
// *  MOV [g_x], EAX  1     INC EAX         1
// *                        MOV [g_x], EAX  1
// *  The result of this operation will be 1
// */
//DWORD WINAPI ThreadFunc1_UnSave(PVOID pvParam)
//{
//   g_x++;
//   return (0);
//}
//
//DWORD WINAPI ThreadFunc2_UnSave(PVOID pvParam)
//{
//   g_x++;
//   return (0);
//}
//
//DWORD WINAPI ThreadFunc1_Save(PVOID pvParam)
//{
//   InterlockedExchangeAdd(&g_x, 1);
//   return (0);
//}
//
//DWORD WINAPI ThreadFunc2_Save(PVOID pvParam)
//{
//   InterlockedExchangeAdd(&g_x, 1);
//   return (0);
//}
//
///* Another example with volatile keyword. Infinite loop 
// * Assembler code:
// * 
// * mov    ptr, #0x1234     
//   mov    a, @ptr 
//   loop     
//   bz    
//   loop
// * read a value from RAM into register and loop after that
// * With volatile this code will be:
// *  mov     ptr, #0x1234
//    loop    mov    a, @ptr        
//    bz      loop
// * the value will be read from RAM into register for every loop step.
// * 
// */
//void BadLoop()
//{
//	/* We have a pointer to some ram memory */
//	int* ptr = (int*)0x1234;
//    /* Wait for register to become non-zero. 
//	 * This code is executed in one thread.
//	 * The other simultaneous thread can set this variable to 0 */
//    while (*ptr == 0);
//}
//
///* Global variable indicating whether
// * a shared resource is in use or not
// */
//BOOL g_fResourceInUse = FALSE;
//
//void SpinLock()
//{
//   /* Sets a 32-bit variable to the specified value
//    * as an atomic operation. 
//	*
//	* Target [in, out] 
//    *    A pointer to the value to be exchanged. 
//	*	 The function sets this variable to Value, 
//	*    and returns its prior value.
//	*
//    * Value [in] 
//    *    The value to be exchanged with 
//	*    the value pointed to by Target.
//	*
//    * The function returns the initial 
//	* value of the Target parameter.
//	* 
//	*/
//
//   /* Wait to access the resource. 
//    * We need to explain how spin lock works:
//	* First thread:
//	*  g_fResourceInUse is set to false. Then the thread
//	*  atomically sets the variable to true and checks that
//	*  the previous value was false and break the loop. Then 
//	*  he does some work.
//	* Meantime the second thread sets the variable to true and 
//	* checks that the previous value was true and continue doing it
//	* until the value will be set into false by another thread.
//	* After accomplishing its work the first thread sets the variable
//	* to false and indicates to other threads that the resource is 
//	* free.
//    */  
//   while (InterlockedExchange(&g_fResourceInUse, TRUE) == TRUE)
//      Sleep(0);
//
//   /* Access the resource 
//    * ...
//    */
//
//   /* We no longer need to access the resource. */
//   InterlockedExchange(&g_fResourceInUse, FALSE);
//}
//
//LONG MInterlockedCompareExchange(
//   PLONG plDestination,
//   LONG lExchange,
//   LONG lComparand)
//{
//	/* Original value */
//	LONG lRet = *plDestination;
//    if (*plDestination == lComparand)   
//        *plDestination = lExchange;
//	return (0);
//}
//
//LONGLONG MInterlockedAnd64(
//   LONGLONG *Destination,
//   LONGLONG Value)
//{
//	LONGLONG Old;
//
//	do
//	{
//       Old = *Destination;
//	}
//	while (InterlockedCompareExchange64(Destination, Old & Value, Old) != Old);
//
//	return Old;
//}
//
//struct CUSTINFO
//{
//	DWORD dwCustomerID;  // Mostly read-only
//	int nBalanceDue;     // Read-write
//	wchar_t szName[100]; // Mostly read-only
//	FILETIME ftLastOrderDate; // Read-write
//};
//
//#define CACHE_ALIGN 64
//
///* Force each structure to be in a different cache line. */
//__declspec(align(CACHE_ALIGN)) struct CUSTINFO_ALIGNED
//{
//	DWORD dwCustomerInfo;  // Mostly read-only
//	wchar_t szName[100];   // Mostly read-only
//
//	// Force the following members to be in a different cache line.
//    __declspec(align(CACHE_ALIGN))
//	int nBalanceDue;       // Read-write
//	FILETIME ftLastOrderDate; // Read-write
//};
//
///* Why do we need a volatile keyword here? 
// * The compiler can optimize code and translate
// * a simple cycle into 
// *
// *    while (!g_fFinishedCalculation)
// * 		     ;
// * 
// *    MOV Reg0, [g_fFinishedCalculation]    ; Copy the value into a register
// *    Label: TEST Reg0, 0                   ; Is the value 0?
// *    JMP Reg0 == 0, Label                  ; The register is 0, try again
// *                                          ; The register is not 0 (end of loop)
// *
// * The compiler can generate a cycle which reads the variable from the CPU cache 
// * and it may cause an infinite loop. Volatile makes compile to generate 
// * a code which always reads the value from RAM
// */
//volatile BOOL g_fFinishedCalculation = FALSE;
//
//DWORD WINAPI RecalcFunc(PVOID pvParam)
//{
//	// Perform the recalculation.
//	g_fFinishedCalculation = TRUE;
//	return (0);
//}
//
//int WINAPI MainFunction()
//{
//	// CreateThread( ... , RecalcFunc, ... );
//	// ...
//	// Wait for the recalculation to complete.
//	while (!g_fFinishedCalculation)
//		;
//}
//
//const int COUNT = 1000;
//int g_nSum = 0;
//
//DWORD WINAPI FirstThread1(PVOID pvParam)
//{
//	int n;
//
//	EnterCriticalSection(&g_nSum);
//
//	g_nSum = 0;
//
//	for (n = 1; n <= COUNT; n++)
//	{
//		g_nSum += n;
//	}
//
//	LeaveCriticalSection(&g_nSum);
//
//	return (g_nSum);
//}
//
//DWORD WINAPI SecondThread1(PVOID pvParam)
//{
//	int n;
//
//	EnterCriticalSection(&g_nSum);
//
//	g_nSum = 0;
//
//	for (n = 1; n <= COUNT; n++)
//	{
//		g_nSum += n;
//	}
//
//	LeaveCriticalSection(&g_nSum);
//
//	return (g_nSum);
//}
//
//
////  ####################          QUEUE                 ##################################
//
//struct QueueItem
//{
//   int m_nThreadNum;
//   int m_nRequestNum;
//   // Other element data should go here
//};
//
//struct InnerQueueItem
//{
//   int m_nStamp; // o means empty
//   struct QueueItem m_element;
//};
//
///* Array of elements to be processed */
//struct InnerQueueItem* m_pElements;
///* Maximum # of elements in the array */
//int m_nMaxElements;
///* Keep track of the # of added elements */
//int m_nAddedElements;
//
//int GetFreeSlot()
//{
//	int current;
//	/* Look for the first element with a 0 stamp 
//	 * We are looking for the first element with 0 which means 
//	 * that this slot is free or empty or has been already read
//	 * by a processing thread.
//	 * This function returns 0 when it is not possible to find 
//	 * a free slot in the array.
//	 */
//    for (current = 0; current < m_nMaxElements; current++)
//	{
//		if (m_pElements[current].m_nStamp == 0)
//		{
//			return (current);
//		}
//	}
//	/* No free slot found */
//	return (-1);
//}
//
//int GetNextSlot(int nThreadNum)
//{
//	int current;
//    /* By default, there is no slot for this thread */
//    int firstSlot = -1;
//	/* The element can't have a stamp higher than the last added */
//	int firstStamp = m_nAddedElements + 1;
//	/* Look for the even (thread 0) / odd (thread 1) 
//	 * element that is not free */
//	for (current = 0; current < m_nMaxElements; current++)
//	{
//        /* Keep track of the first added (lowest stamp) in the queue
//	     * so that "first in first out" behaviour is ensured
//	     */ 
//		if (m_pElements[current].m_nStamp != 0 && // free element
//			m_pElements[current].m_element.m_nRequestNum % 2 == nThreadNum &&
//			m_pElements[current].m_nStamp < firstStamp)
//		{
//			firstStamp = m_pElements[current].m_nStamp;
//			firstSlot = current;
//		}
//	}
//}
//
//void AddElement(struct QueueItem e)
//{
//	/* Do nothing if the queue is full */
//	int nFreeSlot = GetFreeSlot();
//	if (nFreeSlot == -1)
//		return;
//
//    /* Copy the content of the element */
//	m_pElements[nFreeSlot].m_element = e;
//
//	/* Mark the element with the new stamp */
//	m_pElements[nFreeSlot].m_nStamp = ++m_nAddedElements;
//}
//
///* This method is used by a server thread and it
// * retrieves the next element in queue to process
// */
//BOOL GetNewElement(int nThreadNum, struct QueueItem* e)
//{
//   int nNewSlot = GetNextSlot(nThreadNum);
//   if (nNewSlot == -1)
//	   return (FALSE);
//
//   /* Copy the content of the element */
//   e = &m_pElements[nNewSlot].m_element;
//   /* Mark the elements as read */
//   m_pElements[nNewSlot].m_nStamp = 0;
//
//   return (TRUE);
//}
//
///* Here we need to illustrate how this queue works 
// *
// * 1. Adding a certain number of new elements:
// *
// *    Queue from the start looks like:  00000...000  (m_nMaxElements) times
// *    call AddElement firstly:
// *    GetFreeSlot() returns 0 and then we fill in this slot by a new element
// *    and increment m_nAddedElements variable 
// *
// *    m_nAddedElements :  1
// *    m_pElements      :  [1][0][0]....[0]  m_nMaxElements is the length
// *    
// *    Add a new element
// *        
// *    m_nAddedElements :  2
// *    m_pElements      :  [1][2][0]....[0]  m_nMaxElements is the length
// *
// *    Add a new element
// *    
// *    m_nAddedElements :  3
// *    m_pElements      :  [1][2][3]....[0]  m_nMaxElements is the length
// *
// *    ...   
// * 
// *    Lets suppose the we have added k elements into the queue:
// *    
// *    m_nAddedElements :  k
// *    m_pElements      :  [1][2][3]...[k-1][k]...[0]  m_nMaxElements is the length
// * 
// * 2. Processing items from the queue or dequeue operations:
// *    
// *    Dequeue. We need to find the next queue item 
// *             which has the smallest stamp. The smallest means that this item
// *             has been firstly added into the queue and must be processed first.
// *             and set stamp to 0
// *         
// *    m_nAddedElements :  k
// *    m_pElements      :  [0][2][3]...[k-1][k]...[0]  m_nMaxElements is the length
// *    
// *    Dequeue again
// *     
// *    m_nAddedElements :  k
// *    m_pElements      :  [0][0][3]...[k-1][k]...[0]  m_nMaxElements is the length
// *    
// *    Let's suppose that we have dequeued m elements and m < k. Our queue looks like
// *
// *    m_nAddedElements :  k
// *    m_pElements      :  [0][0][0]...[m-1][m]...[k-1][k]...[0][0][0]
// *    
// * 3. Adding items again:  
// *     
// *    m_nAddedElements :  k + 1
// *    m_pElements      :  [k + 1][0][0]...[m-1][m]...[k-1][k]...[0][0][0]
// *    
// *    m_nAddedElements :  k + 2
// *    m_pElements      :  [k + 1][k + 2][0]...[m-1][m]...[k-1][k]...[0][0][0]
// */   
//
///* Reader/writer lock to protect the queue */
//SRWLOCK g_srwLock; 
///* Signalled by writes 
// * If a reader thread tries to read from the empty
// * queue, it will wait for at least one item being 
// * enqueued into the queue.
// */
//CONDITION_VARIABLE g_cvReadyToConsume;
///* Signalled by readers 
// * Reader signals this variable after reading 
// * which indicates that a new empty slot has just been freed
// * and the queue is ready to enqueing new items
// */
//CONDITION_VARIABLE g_cvReadyToProduce;
///* A variable that indicates the end of the application */
//BOOL g_fShutdown = FALSE;
//
///* Client thread is a writer thread  */
//DWORD WINAPI WriterThread(PVOID pvParam)
//{
//   int nThreadNum = PtrToUlong(pvParam);
//   int nRequestNum;
//
//   for (nRequestNum = 1; !g_fShutdown; nRequestNum++)
//   {
//	   struct QueueItem elem = { nThreadNum, nRequestNum };
//	   /* Require access for writing 
//	    * Acquires a slim reader/writer (SRW) lock in exclusive mode.
//		* SRWLock [in, out]
//        *   A pointer to the SRW lock.
//	    */
//       AcquireSRWLockExclusive(&g_srwLock);
//	   /* If the queue is full, fall asleep as long as 
//	    * the condition variable is not signaled
//		* Note: During the wait for acquiring the lock,
//		*       a stop might have been received
//	    */
//       if (GetFreeSlot() == -1 && !g_fShutdown)
//	   {
//           /* Need to wait for a reader to empty a slot 
//		    * before acquiring the lock again
//			*
//			* Sleeps on the specified condition variable 
//			* and releases the specified lock as an atomic operation.
//			*
//			* ConditionVariable [in, out]
//            *  A pointer to the condition variable. 
//			*  This variable must be initialized using the 
//			*  InitializeConditionVariable function.
//			*
//            * SRWLock [in, out]
//            *  A pointer to the lock. This lock must be held in 
//			*  the manner specified by the Flags parameter.
//		    */ 
//           SleepConditionVariableSRW(
//			   &g_cvReadyToProduce,
//			   &g_srwLock,
//			   INFINITE,
//			   0);
//		   /* We have just received a message that the queue has 
//		    * free slots
//		    */
//	   }
//
//	   /* It there are no request for quitting  */
//	   if (!g_fShutdown)
//	   {
//           /* Add the element into the queue */
//           AddElement(elem);
//		   /* Now we need to release the SRW lock 
//		    * Releases a slim reader/writer (SRW) lock 
//			* that was acquired in exclusive mode.
//		    */
//		   ReleaseSRWLockExclusive(&g_srwLock);
//		   /* Signal all reader threads that there is an
//		    * element to consume.
//		    */
//           //WakeAllConditionVariable(&g_cvReadyToConsume);
//		   /* Wait before adding a new element */
//		   Sleep(1500);
//
//		   continue;
//	   }
//
//	   /* Here g_fShutdown variable is set to true
//	    * so we need to quit properly
//	    */
//       ReleaseSRWLockExclusive(&g_srwLock);
//	   /* Signal other blocked writer threads that 
//	    * it is time to exit
//		*/
//       //WakeAllConditionVariable(&g_cvReadyToProduce);
//
//	   return (0);
//   }
//
//   return (0);
//}
//
//BOOL ConsumeElement(int nThreadNum, int nRequestNum, HWND hWndLB)
//{
//	/* Get the first new element */
//    struct QueueItem e;
//    /* Get access to the queue to consume a new element 
//	 * Acquires a slim reader/writer 
//	 * (SRW) lock in shared mode.
//	 * SHARED MODE
//	 *    grants shared read-only access to multiple reader threads,
//	 *    which enables them to read data from 
//	 *    the shared resource concurrently.
//	 */
//    AcquireSRWLockShared(&g_srwLock);
//	/* Fall asleep until there is something to read.
//	 * Check if, while it was asleep,
//	 * it was not decided that the thread should stop
//	 */
//	while (GetNextSlot(nThreadNum) == -1 && !g_fShutdown)
//	{
//        /* There was not a readable element 
//		 * The queue is empty
//		 * Wait until a writer adds a new element to read
//		 * and come back with the lock acquired in shared mode
//		 * 
//		 * Flags [in]
//         *  If this parameter is CONDITION_VARIABLE_LOCKMODE_SHARED, 
//         *  the SRW lock is in shared mode. Otherwise, 
//		 *  the lock is in exclusive mode.
//		 */
//		SleepConditionVariableSRW(
//			&g_cvReadyToConsume, 
//			&g_srwLock,
//			INFINITE,
//			CONDITION_VARIABLE_LOCKMODE_SHARED);
//	}
//	/* When thread is existing, the lock should be released for writers
//	 * and readers should be signaled through the condition variable
//	 */
//	if (g_fShutdown)
//	{
//		/* Show that the current thread is existing
//		 * Another writer thread might still be blocked 
//		 * on the lock --> release it before exiting
//		 */
//        ReleaseSRWLockShared(&g_srwLock);
//		/* Notify other readers that it is time to exit
//		 * --> release readers
//		 */
//		WakeConditionVariable(&g_cvReadyToConsume);
//
//		return (FALSE);
//	}
//    /* Note: No need to test the return value since
//	 * IsEmpty returned FALSE
//	 */
//    GetNewElement(nThreadNum, &e);
//    /* No need to keep the lock any longer */
//	ReleaseSRWLockShared(&g_srwLock);
//	/* A free slot is now available for writer threads to produce
//	 * --> wake up a writer thread
//	 */
//	WakeConditionVariable(&g_cvReadyToProduce);
//
//	return (TRUE);
//}
//
///* Now we need to explain what happens when share and exclusive locks are 
// * acquired
// *
// * 1. Shared locks are acquired simultaneously:
// *   1.1 Firstly if the shared resource is free and 
// *       we try to acquire a shared lock we will succeed 
// *       bacause the shared resource is free.
// *
// *   1.2 Then while the first thread is reading the resource,
// *       the second thread is trying to acquire a shared lock.
// *       he will succeed because the system allows multiply
// *       concurrent readers.
// *  
// * 2. Exclusive locks are acquired simultaneously:
// *   2.1 Firstly if the shared resource is free and 
// *       we try to acquire an exclusive lock we will succeed 
// *       bacause the shared resource is free.
// *     
// *   2.2 Then while the first thread is modifying the resource,
// *       the second thread is trying to acquire an exclusive lock
// *       it will be blocked until the first thread finishes 
// *       modifying the resource.
// *   
// * 3. Shared and exclusive locks are acquired simultaneously 
// *    (exclusive after shared)
// *   3.1 Firstly the shared resource is free and 
// *       we try to acquire a shared lock, we will succeed 
// *       bacause the shared resource is free.    
// *   
// *   3.2 Then while the first thread is reading the shared resource
// *       value, the second thread is trying to acquire an exclusive 
// *       lock. It will be blocked until the first thread finishes 
// *       reading. Why it is true?
// *       To avoid data corruption - the reader thread may read data
// *       only partially and then read the new version od data which is
// *       unacceptable.
// *   
// * 4. Shared and exclusive locks are acquired simultaneously   
// *    (shared after exclusive)
// *   4.1 Firstly the shared resource is free and 
// *       we try to acquire an exclusive lock, we will succeed 
// *       bacause the shared resource is free.     
// *   
// *   4.2 Then while the first thread is modifying the shared resource
// *       value, the second thread is trying to acquire a shared lock.
// *       It will be blocked until the first finishes writing.
// */
//
//void StopProcessing()
//{
//	if (!g_fShutdown)
//	{
//		/* Ask all threads to end */
//		InterlockedExchangePointer((PLONG*)&g_fShutdown, (LONG)TRUE);
//		/* Free all threads waiting on condition variables */
//		//WakeAllConditionVariable(&g_cvReadyToConsume);
//        //WakeAllConditionVariable(&g_cvReadyToProduce);
//		/* Wait for all the threads to terminate & then clean up */
//		//WaitForMultipleObjects(
//	}
//}
//
///* Deadlock example 
// * 1. The first thread has got ownership for the resource1
// * 2. The second thread has got ownership for the resource2
// * 3. The first thread tries to get ownership on the resource 2
// *    and it is blocked because the thread2 has put a critical section
// *    on it.
// * 4. The second thread tries to get ownership on the resource 1
// *    and it gets blocked because the resource 1 is hold by the thread 1
// *    So a deadlock has occured.
// */
//
//int g_Resource1;
//int g_Resource2;
//
//void ThreadFunc1()
//{
//   EnterCriticalSection(&g_Resource1);
//   EnterCriticalSection(&g_Resource2);
//
//   LeaveCriticalSection(&g_Resource1);
//   LeaveCriticalSection(&g_Resource2);
//}
//
//void ThreadFunc2()
//{
//   EnterCriticalSection(&g_Resource2);
//   EnterCriticalSection(&g_Resource1);
//
//   LeaveCriticalSection(&g_Resource2);
//   LeaveCriticalSection(&g_Resource1);
//}
//
//void WaitForSingleObjectTest()
//{
//	HANDLE hProcess;
//	DWORD dw = WaitForSingleObject(hProcess, 5000);
//	switch (dw)
//	{
//	case WAIT_OBJECT_0:
//		//The process terminated.
//		break;
//	case WAIT_TIMEOUT:
//		//The process did not terminate within 5000 milliseconds
//		break;
//	case WAIT_FAILED:
//		//Bas call to function (invalid handle)
//		break;
//	}
//}
//
//void WaitForMultipleObjectsTest()
//{
//	HANDLE hProcess1;
//    HANDLE hProcess2;
//    HANDLE hProcess3;
//    DWORD dw;
//
//	HANDLE h[3];
//    h[0] = hProcess1;
//    h[1] = hProcess2;
//    h[2] = hProcess3;
//
//	dw = WaitForMultipleObjects(3, h, FALSE, 5000);
//
//    switch (dw)
//	{
//	case WAIT_FAILED:
//        //Bad call to function (invalid handle)
//		break;
//	case WAIT_TIMEOUT:
//		//None of the objects became signalled within 5000 milliseconds.
//		break;
//	case WAIT_OBJECT_0 + 0:
//		//The process identified by h[0] (hProcess1) terminated.
//		break;
//	case WAIT_OBJECT_0 + 1:
//		//The process identified by h[1] (hProcess2) terminated.
//		break;
//    case WAIT_OBJECT_0 + 2:
//		//The process identified by h[2] (hProcess3) terminated.
//		break;
//	}
//}
//
///* Create a global handle to a manual-reset, nonsignalled event. */
//HANDLE g_hEvent;
//
//void WordCount()
//{
//    /* Wait until the file's data is in memory. */
//    WaitForSingleObject(g_hEvent, INFINITE);
//    /* Access the memory block. 
//	 * ...
//	 */ 
//	return (0);
//}
//
//void SpellCheck()
//{
//    /* Wait until the file's data is in memory. */
//    WaitForSingleObject(g_hEvent, INFINITE);
//    /* Access the memory block. 
//	 * ...
//	 */ 
//	return (0);
//}
//
//void GrammarCheck()
//{
//    /* Wait until the file's data is in memory. */
//    WaitForSingleObject(g_hEvent, INFINITE);
//    /* Access the memory block. 
//	 * ...
//	 */ 
//	return (0);
//}
//
//void ManualResentEventTest()
//{
//	HANDLE hThread[3];
//	DWORD dwThreadID;
//    /* Create the manual-reset, nonsignalled event. 
//	 * From the start the event state is nonsignalled
//	 * So if some thread call WallForSingleObject - 
//	 * It will be not scheduled util the state of the manual-
//	 * reset event changes to signalled.
//	 */
//    g_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
//	/* Spawn 3 new threads. */
//    hThread[0] = _beginthreadex(NULL, 0, WordCount, NULL, 0, &dwThreadID);
//	hThread[1] = _beginthreadex(NULL, 0, SpellCheck, NULL, 0, &dwThreadID);
//    hThread[2] = _beginthreadex(NULL, 0, GrammarCheck, NULL, 0, &dwThreadID);
//
//	/* Open file and read contents into memory (...) 
//	 * This operation is time-consuming and all other threads 
//	 * are wailting this operation to complete.
//	 */
//    
//	/* Allow all 3 threads to access the memory 
//	 * Set the event to signalled state. Now all threads 
//	 * are resumed and continue working.
//	 * We need to describe a difference between AutoResetEvent
//	 * and ManualResetEvent. 
//	 * ManualResetEvent does not have any side effects. Afrer the 
//	 * object become signalled, all threads which wait for it become 
//	 * schedulable. 
//	 * AutoResetEvent has a side effect that after it gets signalled
//	 * only one thread become schedulable and then it becomes unsignalled
//	 */
//	SetEvent(g_hEvent);
//}
//
///* Create a global handle to a auto-reset, nonsignalled event. 
// * This is a handle to a auto-reset event.
// */
//HANDLE g_hAutoResetEvent;
//
//void WordCountAuto()
//{
//    /* Wait until the file's data is in memory. */
//    WaitForSingleObject(g_hAutoResetEvent, INFINITE);
//    /* Access the memory block. 
//	 * ...
//	 */ 
//	/* After set AutoReset event windows runs successful side-effect:
//	 * Windows runs only one thread which has been waiting for before
//	 * and sets the object to nonsignalled mode. Windows chooses the first
//	 * arbitrarily. The two other threads are left in non-scheduling mode
//	 * and they are left waiting for the auto reset event.
//	 */
//    SetEvent(g_hAutoResetEvent);
//	return (0);
//}
//
//void SpellCheckAuto()
//{
//    /* Wait until the file's data is in memory. */
//    WaitForSingleObject(g_hAutoResetEvent, INFINITE);
//    /* Access the memory block. 
//	 * ...
//	 */ 
//	SetEvent(g_hAutoResetEvent);
//	return (0);
//}
//
//void GrammarCheckAuto()
//{
//    /* Wait until the file's data is in memory. */
//    WaitForSingleObject(g_hEvent, INFINITE);
//    /* Access the memory block. 
//	 * ...
//	 */ 
//	SetEvent(g_hAutoResetEvent);
//	return (0);
//}
//
//void AutoResentEventTest()
//{
//	HANDLE hThread[3];
//	DWORD dwThreadID;
//
//    g_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
//	/* Spawn 3 new threads. */
//    hThread[0] = _beginthreadex(NULL, 0, WordCountAuto, NULL, 0, &dwThreadID);
//	hThread[1] = _beginthreadex(NULL, 0, SpellCheckAuto, NULL, 0, &dwThreadID);
//    hThread[2] = _beginthreadex(NULL, 0, GrammarCheckAuto, NULL, 0, &dwThreadID);
//
//	/* Using an auto-reset event puts all thread into queue.
//	 * All thread are executed consequentially
//	 */
//	SetEvent(g_hEvent);
//}
//
//HANDLE hEventWorkerThreadDone;
//HANDLE hEventMoreWorkToBeDone;
//
//void TestWithoutSignalAndWait_Thread1()
//{
//	/* Perform some work. ...  
//	 * there are many threads which are waiting
//	 * for this work to be done. After that we notify 
//	 * these threads about completed work. */
//	SetEvent(hEventWorkerThreadDone);
//	/* Wait for event - moreWorkToBeDone */
//    WaitForSingleObject(hEventMoreWorkToBeDone, INFINITE);
//	/* Perform more work. ... */
//}
//
//void TestWithoutSignalAndWait_Thread2()
//{
//	/* Wait for the thread 1 to perform some work  */
//    WaitForSingleObject(hEventWorkerThreadDone, INFINITE);
//	/* After that do some work and pulse event MoreWorkToDo */
//    PulseEvent(hEventMoreWorkToBeDone);
//}
//
///* Why this approach is not reliable?
// * The first thread set event: WorkerThreadDone
// * And after that the second thread might resume
// * almost immediately and pulse event MoreWorkToBeDone
// * before the first thread starts waiting for it.
// * So that we miss the event order.
// * We need to substitute SetEvent and WaitForSingleObject
// * by SignalObject and wait to make this two operations atomic
// */
//
//
///* ############################# Asyncronous IO device operations  ########################################## */
//
//void AsyncIOUsingSignallingDeviceKernelObject()
//{
//	BYTE bBuffer[100];
//	DWORD dwError;
//	OVERLAPPED o = { 0 };
//    BOOL bReadDone;
//    HANDLE hFile;
//
//	o.Offset = 345;
//
//    hFile = CreateFile("path to file",          // name of the write
//                             GENERIC_WRITE,           // open for writing
//                             0,                       // do not share
//                             NULL,                    // default security
//                             CREATE_NEW,              // create new file only
//                             FILE_ATTRIBUTE_NORMAL,   // normal file
//                             NULL);                   // no attr. template
//
//	/* If we add the latest overlapped parameter, we do an asyncronous request */
//	bReadDone = ReadFile(hFile, bBuffer, 100, NULL, &o);
//    /* Function returns immediately */
//	dwError = GetLastError();
//
//	if (!bReadDone && (dwError == ERROR_IO_PENDING))
//	{
//		/* We have successfully posted an asyncronous request
//		 * and now the I/O is being performed asyncronously; We can do 
//		 * some work and after that wait for the operation to complete.
//		 * ReadFile sets the state of file handler to nonsignalled and after 
//		 * accomplishing operation it becomes signalled. */
//        WaitForSingleObject(hFile, INFINITE);
//		bReadDone = TRUE;
//	}
//	
//	if (bReadDone)
//	{
//		// o.Internal contains the I/O error
//		// o.InternalHigh contains the number of bytes transferred
//		// bBuffer contains the read data
//	}
//	else
//	{
//		// An error occured; see dwError
//	}
//}
//
//void SimultaneousReadAndWrite()
//{
//    BYTE bReadBuffer[10];
//	OVERLAPPED oRead = { 0 };
//    HANDLE hFile;
//       
//	BYTE bWriteBuffer[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    OVERLAPPED oWrite = { 0 };
//	oWrite.Offset = 10;
//
//    hFile = CreateFile("path to file",          // name of the write
//                             GENERIC_WRITE,           // open for writing
//                             0,                       // do not share
//                             NULL,                    // default security
//                             CREATE_NEW,              // create new file only
//                             FILE_ATTRIBUTE_NORMAL,   // normal file
//                             NULL);                   // no attr. template
//
//	ReadFile(hFile, bReadBuffer, 10, NULL, &oRead);
//	WriteFile(hFile, bWriteBuffer, _countof(bWriteBuffer), NULL, &oWrite);
//	//... do some additional work
//	WaitForSingleObject(hFile, INFINITE);
//	/* If one of the read or write operations has completed, the state of the object becomes signalled
//	 * but we do not know what operation has completed: read or write. */
//}
//
//void SimultaneousReadAndWriteWithEvents()
//{
//	HANDLE h[2];
//    DWORD dw;
//	HANDLE hFile;
//
//    BYTE bReadBuffer[10];
//	OVERLAPPED oRead = { 0 };
//
//    BYTE bWriteBuffer[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//    OVERLAPPED oWrite = { 0 };
//
//	oRead.Offset = 0;
//	oRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
//
//	oWrite.Offset = 10;
//	oWrite.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
//
//
//    hFile = CreateFile("path to file",          // name of the write
//                             GENERIC_WRITE,           // open for writing
//                             0,                       // do not share
//                             NULL,                    // default security
//                             CREATE_NEW,              // create new file only
//                             FILE_ATTRIBUTE_NORMAL,   // normal file
//                             NULL);              
//
//	ReadFile(hFile, bReadBuffer, 10, NULL, &oRead);
//	WriteFile(hFile, bWriteBuffer, _countof(bWriteBuffer), NULL, &oWrite);
//
//	h[0] = oRead.hEvent;
//	h[1] = oWrite.hEvent;
// 
//	/* We wait until the both objects become signalled
//	 * When asyncronous read operation finishes, it will set oRead event to signalled
//	 * When asyncronous write operation completes, it will set oWrite event to signalled
//	 * And only after this we finish waiting. */
//    dw = WaitForMultipleObjects(2, h, FALSE, INFINITE);
//	switch (dw - WAIT_OBJECT_0)
//	{
//	case 0: //Read completed
//       break;
//	case 1: // Write completed
//	   break;
//	}
//}
//
//BOOL GetOverlappedResult1(
//    HANDLE hFile,
//	OVERLAPPED* po,
//	PDWORD pdwNumBytes,
//    BOOL bWait
//)
//{
//	DWORD dwWaitRet;
//
//	if (po->Internal == STATUS_PENDING)
//	{
//		DWORD dwWaitResult = WAIT_TIMEOUT;
//		if (bWait)
//		{
//		   /* Wait for the I/O to complete */
//		   dwWaitRet = WaitForSingleObject(
//			(po->hEvent != NULL) ? po->hEvent : hFile, INFINITE);
//		}
//         
//        if (dwWaitResult == WAIT_TIMEOUT)
//		{
//           /* I/O not complete and we are not supposed to wait */  
//           SetLastError(ERROR_IO_INCOMPLETE);
//		   return (FALSE);
//		}
//
//		if (dwWaitResult == WAIT_OBJECT_0)
//		{
//		   /* Error calling WaitForSingleObject */
//			return (FALSE);
//		}
//		/* I/O is complete; return number of bytes transferred */
//		*pdwNumBytes = po->InternalHigh;
//		if (SUCCEEDED(po->Internal)) {
//			return (TRUE); // No I/O error
//		}
//		/* Set last error to I/O error */
//		SetLastError(po->Internal);
//		return (FALSE);
//	}
//}
//
///* #################################  Alertable I/O ################################## */
//
//// The APC callback function has nothing to do
//VOID WINAPI APCFunc(ULONG_PTR dwParam) 
//{
//   // Nothing to do in here
//}
//
//UINT WINAPI APCThreadFunc(PVOID pvParam) 
//{
//   HANDLE hEvent = (HANDLE) pvParam; // Handle is passed to this thread
//   // Wait in an alertable state so that we can be forced to exit cleanly
//   DWORD dw = WaitForSingleObjectEx(hEvent, INFINITE, TRUE); 
//   if (dw == WAIT_OBJECT_0) 
//   {
//      // Object became signaled
//   }
//   if (dw == WAIT_IO_COMPLETION) 
//   {
//      // QueueUserAPC forced us out of a wait state
//      return(0); // Thread dies cleanly
//   }
//   ///...
//   return(0);
//}
//
//void APCMain() 
//{
//   HANDLE hEvent =  CreateEvent(NULL, FALSE, FALSE, NULL);
//   HANDLE hThread = 
//	   (HANDLE)_beginthreadex(
//	       NULL, 
//		   0,
//           APCThreadFunc, 
//		   (PVOID)hEvent, 
//		   0, 
//		   NULL);
//
//   // Force the secondary thread to exit cleanly
//   QueueUserAPC(APCFunc, hThread, NULL);
//   WaitForSingleObject(hThread, INFINITE);
//   CloseHandle(hThread);
//   CloseHandle(hEvent);
//}
//
///* #################################  Com port thread pool  ################################## */
//
//LONG g_nThreadsMin;    // Minimum number of threads in pool
//LONG g_nThreadsMax;    // Maximum number of threads in pool
//LONG g_nThreadsCrnt;   // Current number of threads in pool
//LONG g_nThreadsBusy;   // Number of busy threads in pool
//
//DWORD GetCPUUsage()
//{
//   return 0; 
//}
//
//DWORD WINAPI ThreadPoolFunc(PVOID pv) 
//{
//   BOOL bStayInPool;
//   // Thread is entering pool
//   InterlockedIncrement(&g_nThreadsCrnt);
//   InterlockedIncrement(&g_nThreadsBusy);
//
//   for (bStayInPool = TRUE; bStayInPool;) 
//   {
//       BOOL bOk;
//	   DWORD dwIOError;
//       int nThreadsBusy;
//	   // Thread stops executing and waits for something to do
//       InterlockedDecrement(&g_nThreadsBusy);
//	   bOk = GetQueuedCompletionStatus(NULL, NULL, NULL, NULL, 0);
//	   dwIOError = GetLastError();
//	   // Thread has something to do, so it's busy
//       nThreadsBusy = InterlockedIncrement(&g_nThreadsBusy);
//	   // Should we add another thread to the pool?
//       if (nThreadsBusy == g_nThreadsCrnt) 
//	   { 
//		   // All threads are busy
//           if (nThreadsBusy < g_nThreadsMax) 
//		   { 
//			   // The pool isn't full
//               if (GetCPUUsage() < 75) 
//			   { 
//				   // CPU usage is below 75%
//                   // Add thread to pool
//                   //CloseHandle(chBEGINTHREADEX(...));
//               }
//           }
//       }
//	   if (!bOk && (dwIOError == WAIT_TIMEOUT)) 
//	   { 
//		   // Thread timed out
//           // There isn't much for the server to do, and this thread
//           // can die even if it still has outstanding I/O requests
//           bStayInPool = FALSE;
//       }
//       if (bOk) 
//	   {
//          // Thread woke to process something; process it
//          //...
//          if (GetCPUUsage() > 90) 
//		  { 
//			  // CPU usage is above 90%
//              if (g_nThreadsCrnt > g_nThreadsMin) 
//			  { 
//				  // Pool above min
//                  bStayInPool = FALSE; 
//				  // Remove thread from pool
//              }
//          }
//       }
//   }
//
//   // Thread is leaving pool
//   InterlockedDecrement(&g_nThreadsBusy);
//   InterlockedDecrement(&g_nThreadsCrnt);
//   return(0);
//}
//
///* ################################################## Fibers ########################################### */
//

VOID __stdcall ReadFiberFunc(LPVOID lpParameter);
VOID __stdcall WriteFiberFunc(LPVOID lpParameter);
void DisplayFiberInfo(void);


typedef struct
{
   DWORD dwParameter;          // DWORD parameter to fiber (unused)
   DWORD dwFiberResultCode;    // GetLastError() result code
   HANDLE hFile;               // handle to operate on
   DWORD dwBytesProcessed;     // number of bytes processed
} FIBERDATASTRUCT, *PFIBERDATASTRUCT, *LPFIBERDATASTRUCT;

#define RTN_OK 0
#define RTN_USAGE 1
#define RTN_ERROR 13

#define BUFFER_SIZE 32768   // read/write buffer size
#define FIBER_COUNT 3       // max fibers (including primary)

#define PRIMARY_FIBER 0 // array index to primary fiber
#define READ_FIBER 1    // array index to read fiber
#define WRITE_FIBER 2   // array index to write fiber

LPVOID g_lpFiber[FIBER_COUNT];
LPBYTE g_lpBuffer;
DWORD g_dwBytesRead;

void DisplayFiberInfo(void)
{
   LPFIBERDATASTRUCT fds = (LPFIBERDATASTRUCT)GetFiberData();
   LPVOID lpCurrentFiber = GetCurrentFiber();

   //
   // Determine which fiber is executing, based on the fiber address
   //
   if (lpCurrentFiber == g_lpFiber[READ_FIBER])
      printf("Read fiber entered");
   else
   {
      if (lpCurrentFiber == g_lpFiber[WRITE_FIBER])
         printf("Write fiber entered");
      else
      {
         if (lpCurrentFiber == g_lpFiber[PRIMARY_FIBER])
            printf("Primary fiber entered");
         else
            printf("Unknown fiber entered");
      }
   }

   //
   // Display dwParameter from the current fiber data structure
   //
   printf(" (dwParameter is 0x%lx)\n", fds->dwParameter);
}

VOID __stdcall ReadFiberFunc(LPVOID lpParameter)
{
   LPFIBERDATASTRUCT fds = (LPFIBERDATASTRUCT)lpParameter;

   //
   // If this fiber was passed NULL for fiber data, just return,
   // causing the current thread to exit
   //
   if (fds == NULL)
   {
      printf("Passed NULL fiber data; exiting current thread.\n");
      return;
   }

   //
   // Display some information pertaining to the current fiber
   //
   DisplayFiberInfo();

   fds->dwBytesProcessed = 0;

   while (1)
   {
      //
      // Read data from file specified in the READ_FIBER structure
      //
      if (!ReadFile(fds->hFile, g_lpBuffer, BUFFER_SIZE, 
         &g_dwBytesRead, NULL))
      {
         break;
      }

      //
      // if we reached EOF, break
      //
      if (g_dwBytesRead == 0) break;
      
      //
      // Update number of bytes processed in the fiber data structure
      //
      fds->dwBytesProcessed += g_dwBytesRead;

      //
      // Switch to the write fiber
      //
      SwitchToFiber(g_lpFiber[WRITE_FIBER]);
   }

   //
   // Update the fiber result code
   //
   fds->dwFiberResultCode = GetLastError();

   //
   // Switch back to the primary fiber
   //
   SwitchToFiber(g_lpFiber[PRIMARY_FIBER]);
}

VOID __stdcall WriteFiberFunc(LPVOID lpParameter)
{
   LPFIBERDATASTRUCT fds = (LPFIBERDATASTRUCT)lpParameter;
   DWORD dwBytesWritten;

   //
   // If this fiber was passed NULL for fiber data, just return,
   // causing the current thread to exit
   //
   if (fds == NULL)
   {
      printf("Passed NULL fiber data; exiting current thread.\n");
      return;
   }

   //
   // Display some information pertaining to the current fiber
   //
   DisplayFiberInfo();

   //
   // Assume all writes succeeded.  If a write fails, the fiber
   // result code will be updated to reflect the reason for failure
   //
   fds->dwBytesProcessed = 0;
   fds->dwFiberResultCode = ERROR_SUCCESS;

   while (1)
   {
      //
      // Write data to the file specified in the WRITE_FIBER structure
      //
      if (!WriteFile(fds->hFile, g_lpBuffer, g_dwBytesRead, 
         &dwBytesWritten, NULL))
      {
         //
         // If an error occurred writing, break
         //
         break;
      }

      //
      // Update number of bytes processed in the fiber data structure
      //
      fds->dwBytesProcessed += dwBytesWritten;

      //
      // Switch back to the read fiber
      //
      SwitchToFiber(g_lpFiber[READ_FIBER]);
   }

   //
   // If an error occurred, update the fiber result code...
   //
   fds->dwFiberResultCode = GetLastError();

   //
   // ...and switch to the primary fiber
   //
   SwitchToFiber(g_lpFiber[PRIMARY_FIBER]);
}

int TestFibers()
{
   int argc;
   TCHAR *argv[3];

   LPFIBERDATASTRUCT fs;

   if (argc != 3)
   {
      printf("Usage: %s <SourceFile> <DestinationFile>\n", argv[0]);
      return RTN_USAGE;
   }

   //
   // Allocate storage for our fiber data structures
   //
   fs = (LPFIBERDATASTRUCT) HeapAlloc(
                              GetProcessHeap(), 0,
                              sizeof(FIBERDATASTRUCT) * FIBER_COUNT);

   //
   // Allocate storage for the read/write buffer
   //
   g_lpBuffer = (LPBYTE)HeapAlloc(GetProcessHeap(), 0, BUFFER_SIZE);
   if (g_lpBuffer == NULL)
   {
      printf("HeapAlloc error (%d)\n", GetLastError());
      return RTN_ERROR;
   } 
   
   //
   // Open the source file
   //
   fs[READ_FIBER].hFile = CreateFile(
                                    argv[1],
                                    GENERIC_READ,
                                    FILE_SHARE_READ,
                                    NULL,
                                    OPEN_EXISTING,
                                    FILE_FLAG_SEQUENTIAL_SCAN,
                                    NULL
                                    );

   if (fs[READ_FIBER].hFile == INVALID_HANDLE_VALUE)
   {
      printf("CreateFile error (%d)\n", GetLastError());
      return RTN_ERROR;
   }

   //
   // Open the destination file
   //
   fs[WRITE_FIBER].hFile = CreateFile(
                                     argv[2],
                                     GENERIC_WRITE,
                                     0,
                                     NULL,
                                     CREATE_NEW,
                                     FILE_FLAG_SEQUENTIAL_SCAN,
                                     NULL
                                     );

   if (fs[WRITE_FIBER].hFile == INVALID_HANDLE_VALUE)
   {
      printf("CreateFile error (%d)\n", GetLastError());
      return RTN_ERROR;
   }

   //
   // Convert thread to a fiber, to allow scheduling other fibers
   //
   g_lpFiber[PRIMARY_FIBER]=ConvertThreadToFiber(&fs[PRIMARY_FIBER]);

   if (g_lpFiber[PRIMARY_FIBER] == NULL)
   {
      printf("ConvertThreadToFiber error (%d)\n", GetLastError());
      return RTN_ERROR;
   }

   //
   // Initialize the primary fiber data structure.  We don't use
   // the primary fiber data structure for anything in this sample.
   //
   fs[PRIMARY_FIBER].dwParameter = 0;
   fs[PRIMARY_FIBER].dwFiberResultCode = 0;
   fs[PRIMARY_FIBER].hFile = INVALID_HANDLE_VALUE;

   //
   // Create the Read fiber
   //
   g_lpFiber[READ_FIBER]=CreateFiber(0,ReadFiberFunc,&fs[READ_FIBER]);

   if (g_lpFiber[READ_FIBER] == NULL)
   {
      printf("CreateFiber error (%d)\n", GetLastError());
      return RTN_ERROR;
   }

   fs[READ_FIBER].dwParameter = 0x12345678;

   //
   // Create the Write fiber
   //
   g_lpFiber[WRITE_FIBER]=CreateFiber(0,WriteFiberFunc,&fs[WRITE_FIBER]);

   if (g_lpFiber[WRITE_FIBER] == NULL)
   {
      printf("CreateFiber error (%d)\n", GetLastError());
      return RTN_ERROR;
   }

   fs[WRITE_FIBER].dwParameter = 0x54545454;

   //
   // Switch to the read fiber
   //
   SwitchToFiber(g_lpFiber[READ_FIBER]);

   //
   // We have been scheduled again. Display results from the 
   // read/write fibers
   //
   printf("ReadFiber: result code is %lu, %lu bytes processed\n",
   fs[READ_FIBER].dwFiberResultCode, fs[READ_FIBER].dwBytesProcessed);

   printf("WriteFiber: result code is %lu, %lu bytes processed\n",
   fs[WRITE_FIBER].dwFiberResultCode, fs[WRITE_FIBER].dwBytesProcessed);

   //
   // Delete the fibers
   //
   DeleteFiber(g_lpFiber[READ_FIBER]);
   DeleteFiber(g_lpFiber[WRITE_FIBER]);

   //
   // Close handles
   //
   CloseHandle(fs[READ_FIBER].hFile);
   CloseHandle(fs[WRITE_FIBER].hFile);

   //
   // Free allocated memory
   //
   HeapFree(GetProcessHeap(), 0, g_lpBuffer);
   HeapFree(GetProcessHeap(), 0, fs);

   return RTN_OK;
}  

void CreateMemoryMappedFiles()
{
	HANDLE hFile = CreateFile(TEXT("C:\\test.dat"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		PAGE_READWRITE, 0, 100, NULL);

	HANDLE hFileMap = CreateFileMapping(hFile, NULL, 
		PAGE_READWRITE, 0, 100, NULL);

	CloseHandle(hFileMap);
    CloseHandle(hFile);

	return 0;
}

void TestMapViewOfFile()
{
	// Open the file that we want to map.
    HANDLE hFile = CreateFile(TEXT("C:\\test.dat"),
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    // Create a file-mapping object for the file.
    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_WRITECOPY, 0, 0, NULL);

	/* Maps a view of a file mapping into the address space of a calling process.
	 *
	 * hFileMappingObject [in]
     *   A handle to a file mapping object. 
	 *   The CreateFileMapping and OpenFileMapping functions return this handle.
	 * 
	 * dwDesiredAccess [in]
     *   The type of access to a file mapping object, 
	 *   which determines the protection of the pages. 
	 *   This parameter can be one of the following values.
	 *
     * FILE_MAP_COPY
     *   A copy-on-write view of the file is mapped. 
	 *   The file mapping object must have been created with 
	 *   PAGE_READONLY, PAGE_READ_EXECUTE, PAGE_WRITECOPY, 
	 *   PAGE_EXECUTE_WRITECOPY, PAGE_READWRITE, 
	 *   or PAGE_EXECUTE_READWRITE protection.
     *   When a process writes to a copy-on-write page, 
	 *   the system copies the original page to a new page that is private to the process. 
	 *   The new page is backed by the paging file. 
	 *   The protection of the new page changes from copy-on-write to read/write.
     *   When copy-on-write access is specified, 
	 *   the system and process commit charge 
	 *   taken is for the entire view 
	 *   because the calling process can potentially 
	 *   write to every page in the view, making all pages private. 
	 *   The contents of the new page are never written back to the original file 
	 *   and are lost when the view is unmapped.
	 * 
	 * dwFileOffsetHigh [in]
     *   A high-order DWORD of the file offset where the view begins. 
	 * 
	 * dwFileOffsetLow [in]
     *   A low-order DWORD of the file offset where the view is to begin. 
	 *   The combination of the high and low offsets 
	 *   must specify an offset within the file mapping. 
	 *   They must also match the memory allocation granularity of the system. 
	 *   That is, the offset must be a multiple of the allocation granularity. 
	 *   To obtain the memory allocation granularity of the system, 
	 *   use the GetSystemInfo function, which fills in the members of a SYSTEM_INFO structure.
	 * 
	 * dwNumberOfBytesToMap [in]
     *   The number of bytes of a file mapping to map to the view. 
	 *   All bytes must be within the maximum size specified by CreateFileMapping. 
	 *   If this parameter is 0 (zero), 
	 *   the mapping extends from the specified offset to the end of the file mapping. 
	 */
    PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_COPY, 0, 0, 0);
    // Read a byte from the mapped view.
	BYTE bSomeByte = pbFile[0];
	// Write a byte to the mapped view
	/* When writing for the first time, the system grabs a committed page 
	 * from the paging file, copies the original contents of the page
	 * (the copy) into the process' address space. The new page has 
	 * an attribute of PAGE_READWRITE */
    pbFile[0] = 0;
	//Write another byte to the mapped view.
    pbFile[1] = 0;
	/* Because this byte is now in a PAGE_READWRITE page, the system
	 * simply writes the byte to the page (backed by the paging file).
	 */
	UnmapViewOfFile(pbFile);
	CloseHandle(hFileMapping);
    CloseHandle(hFile);
}

void TestMapViewOfFile1()
{
    HANDLE hFile = CreateFile(TEXT("C:\\test.dat"),
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, 
		PAGE_READWRITE, 0, 0, NULL);

	// Map a view of the whole file into our address space.
    PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);

	// Map a view of the file (starting 64 KB) into our address space
    pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 65536, 0);
}

DWORD g_dwTlsIndex; // Assume that this is initialized
                    // with the result of a call to TlsAlloc.

struct SOMESTRUCT
{
    int  A;
};

typedef struct SOMESTRUCT* PSOMESTRUCT;

void MyFunction(PSOMESTRUCT pSomeStruct)
{
    if (pSomeStruct != NULL)
	{
	   // The caller is priming this function.
	   // See if we already allocated space to save the data.
	   if (TlsGetValue(g_dwTlsIndex) == NULL)
	   {
          // Space was never allocated. This is the first
	      // time this function has ever been called by this thread.
	      TlsSetValue(g_dwTlsIndex, 
			  HeapAlloc(GetProcessHeap(), 0, sizeof(*pSomeStruct)));
	   }

	   // Memory already exists for the data;
	   // save the newly passed values.
       memcpy(TlsGetValue(g_dwTlsIndex), pSomeStruct, sizeof(*pSomeStruct));
	}
	else
	{
		// The caller already primed the function. Now it
		// wants to do something with the saved data.
		// Get the address of the saved data.
        pSomeStruct = (PSOMESTRUCT)TlsGetValue(g_dwTlsIndex);
		// The saved data is pointed to by pSomeStruct; use it.
	}
}

//void ReplaceIATEntryInOneMod(PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew, HMODULE hmodCaller)
//{
//    // Get the address of the module's import section
//	ULONG ulSize;
//    // An exception was triggered by Explorer (when browsing the content of
//	// a folder) into imagehlp.dll. It looks like one module was unloaded...
//	// Maybe some threading problem: the list of modules from Toolhelp might
//	// not be accurate if FreeLibrary is called during the enumeration.
//    PIMATE_IMPORT_DESCRIPTOR pImportDesc = NULL;
//    __try
//	{
//		/* This function has been superseded by the ImageDirectoryEntryToDataEx function. 
//		 * Use ImageDirectoryEntryToDataEx to retrieve the section header. 
//		 *
//		 * Base [in]
//         *  The base address of the image.
//		 *
//		 * MappedAsImage [in]
//         *  If this parameter is TRUE, the file is mapped by the system as an image. 
//		 *  If the flag is FALSE, the file is mapped as 
//		 *  a data file by the MapViewOfFile function.
//		 *  
//		 * DirectoryEntry [in]
//         *  The index number of the desired directory entry. 
//		 *  This parameter can be one of the following values.
//		 *  IMAGE_DIRECTORY_ENTRY_IMPORT   Import directory
//		 * 
//		 * Size [out]
//         *  A pointer to a variable that receives the size of the data 
//		 *  for the directory entry, in bytes.
//		 * 
//		 * If the function succeeds, the return value is a pointer 
//		 * to the directory entry's data
//		 */
//        pImportDesc = (PIMATE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(
//            hmodCaller, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ulSize);
//	}
//	__except (InvalidReadExceptionFilter(GetExceptionInformation()))
//	{
//        // Nothing to do in here, thread continues to run normally
//		// with NULL for pImportDesc
//	}
//    if (pImportDesc == NULL)
//		return; // This module has no import section or is no longer loaded.
//
//	// Find the import descriptor containing references to callee's functions
//    for (; pImportDesc->Name; pImportDesc++)
//	{
//		PSTR pszModName = (PSTR)((PBYTE)hmodCaller + pImportDesc->Name);
//		if (lstrcmpiA(pszModName, pszCalleeModName))
//		{
//           // Get caller's import address table (IAN) for the calee's functions.
//		   PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)
//			   ((PBYTE)hmodCaller + pImportDesc->FirstThunk);
//
//           // Replace current function address with new function address
//		   for (; pThunk->u1.Function; pThunk++)
//		   {
//               // Get the address of the function address.
//			   PROC* ppfn = (PROC*)&pThunk->u1.Function;
//			   // Is this the function we're looking for?
//               BOOL bFound = (*ppfn == pfnCurrent);
//			   if (bFound)
//			   {
//				   /* Writes data to an area of memory in a specified process. 
//				    * The entire area to be written to must be accessible or the operation fails. 
//					*
//					* hProcess [in]
//                    *  A handle to the process memory to be modified. 
//					*  The handle must have PROCESS_VM_WRITE and PROCESS_VM_OPERATION access to the process.
//					* 
//					* lpBaseAddress [in]
//                    *  A pointer to the base address in the specified process to which data is written. 
//					*  Before data transfer occurs, the system verifies 
//					*  that all data in the base address and memory 
//					*  of the specified size is accessible for write access, 
//					*  and if it is not accessible, the function fails.
//					* 
//					* lpBuffer [in]
//                    *  A pointer to the buffer that contains data to be written 
//					*  in the address space of the specified process.
//					* 
//					* nSize [in]
//                    *  The number of bytes to be written to the specified process. 
//					*/
//                   if (!WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL)
//					   && (ERROR_NOACCESS == GetLastError()))
//				   {
//                      DWORD dwOldProtect;
//					  /* 
//					   * Changes the protection on a region of committed pages 
//					   * in the virtual address space of the calling process.
//                       * To change the access protection of any process, 
//					   * use the VirtualProtectEx function. 
//					   *
//					   * lpAddress [in]
//                       *   A pointer an address that describes the starting 
//					   *   page of the region of pages whose access protection attributes are to be changed.
//					   * 
//					   * dwSize [in]
//                       *   The size of the region whose access protection attributes are to be changed, in bytes. 
//					   * 
//					   * flNewProtect [in]
//                       *   The memory protection option. 
//					   *   This parameter can be one of the memory protection constants.
//					   *   For mapped views, this value must be compatible with the access protection 
//					   *   specified when the view was mapped
//					   *   
//					   * lpflOldProtect [out]
//                       *   A pointer to a variable that receives the previous access protection value of 
//					   *   the first page in the specified region of pages.
//                       *
//					   */  
//                      if (VirtualProtect(ppfn, sizeof(pfnNew), PAGE_WRITECOPY, &dwOldProtect))
//					  {
//                          WriteProcessMemory(GetCurrentProcess(), ppfn, &pfnNew, sizeof(pfnNew), NULL);
//                          VirtualProtect(ppfn, sizeof(pfnNew), PAGE_WRITECOPY, &dwOldProtect);
//					  }
//				   }
//				   return;
//			   }
//		   }
//		}
//	}
//}