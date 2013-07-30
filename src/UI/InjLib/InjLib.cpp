/******************************************************************************
Module:  InjLib.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <stdio.h>
#include <tchar.h>
#include <malloc.h>        // For alloca
#include <TlHelp32.h>
#include "Resource.h"
#include <StrSafe.h>



///////////////////////////////////////////////////////////////////////////////


#ifdef UNICODE
   #define InjectLib InjectLibW
   #define EjectLib  EjectLibW
#else
   #define InjectLib InjectLibA
   #define EjectLib  EjectLibA
#endif   // !UNICODE


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI InjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) {

   BOOL bOk = FALSE; // Assume that the function fails
   HANDLE hProcess = NULL, hThread = NULL;
   PWSTR pszLibFileRemote = NULL;

   __try {
      // Get a handle for the target process.
      hProcess = OpenProcess(
         PROCESS_QUERY_INFORMATION |   // Required by Alpha
         PROCESS_CREATE_THREAD     |   // For CreateRemoteThread
         PROCESS_VM_OPERATION      |   // For VirtualAllocEx/VirtualFreeEx
         PROCESS_VM_WRITE,             // For WriteProcessMemory
         FALSE, dwProcessId);
      if (hProcess == NULL) __leave;

      // Calculate the number of bytes needed for the DLL's pathname
      int cch = 1 + lstrlenW(pszLibFile);
      int cb  = cch * sizeof(wchar_t);

      // Allocate space in the remote process for the pathname
	  /* Reserves or commits a region of memory 
	   * within the virtual address space of a specified process. 
	   * The function initializes the memory it allocates to zero, 
	   * unless MEM_RESET is used. 
	   * 
	   * hProcess [in]
       *  The handle to a process. 
	   *  The function allocates memory within 
	   *  the virtual address space of this process.
       *  The handle must have the PROCESS_VM_OPERATION access right. 
	   *  For more information, see Process Security and Access Rights. 
	   */
      pszLibFileRemote = (PWSTR) 
         VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
      if (pszLibFileRemote == NULL) __leave;

      // Copy the DLL's pathname to the remote process' address space
	  /* Writes data to an area of memory in a specified process. 
	   * The entire area to be written to must be accessible or the operation fails. 
	   */
      if (!WriteProcessMemory(hProcess, pszLibFileRemote, 
         (PVOID) pszLibFile, cb, NULL)) __leave;

      // Get the real address of LoadLibraryW in Kernel32.dll
      PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
         GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryW");
      if (pfnThreadRtn == NULL) __leave;

      // Create a remote thread that calls LoadLibraryW(DLLPathname)
	  /* Creates a thread that runs in the virtual address space of another process.
       * Use the CreateRemoteThreadEx function to create a thread 
	   * that runs in the virtual address space of another processor 
	   * and optionally specify extended attributes.
	   * 
	   * hProcess [in]
       *   A handle to the process in which the thread is to be created. 
	   *   The handle must have the PROCESS_CREATE_THREAD, PROCESS_QUERY_INFORMATION, 
	   *   PROCESS_VM_OPERATION, PROCESS_VM_WRITE, and PROCESS_VM_READ access rights, 
	   *   and may fail without these rights on certain platforms.
	   * 
	   * lpThreadAttributes [in]
       *   A pointer to a SECURITY_ATTRIBUTES structure 
	   *   that specifies a security descriptor 
	   *   for the new thread and determines 
	   *   whether child processes can inherit the returned handle. 
	   *   If lpThreadAttributes is NULL, the thread gets 
	   *   a default security descriptor and the handle cannot be inherited. 
	   *   The access control lists (ACL) in the default security descriptor 
	   *   for a thread come from the primary token of the creator.
       *   Windows XP:  
	   *   The ACLs in the default security descriptor for a thread 
	   *   come from the primary or impersonation token of the creator. 
	   *   This behavior changed with Windows XP with SP2 and Windows Server 2003.
	   *
       * dwStackSize [in]
       *   The initial size of the stack, in bytes. 
	   *   The system rounds this value to the nearest page. 
	   *   If this parameter is 0 (zero), the new thread uses 
	   *   the default size for the executable. 
	   *   For more information, see Thread Stack Size.
	   * 
	   * lpStartAddress [in]
       *   A pointer to the application-defined function 
	   *   of type LPTHREAD_START_ROUTINE to be executed 
	   *   by the thread and represents the starting address 
	   *   of the thread in the remote process. 
	   *   The function must exist in the remote process. 
	   *   For more information, see ThreadProc.
	   */
      hThread = CreateRemoteThread(hProcess, NULL, 0, 
         pfnThreadRtn, pszLibFileRemote, 0, NULL);
      if (hThread == NULL) __leave;

      // Wait for the remote thread to terminate
      WaitForSingleObject(hThread, INFINITE);

      bOk = TRUE; // Everything executed successfully
   }
   __finally 
   { 
	  // Now, we can clean everything up
      // Free the remote memory that contained the DLL's pathname
      if (pszLibFileRemote != NULL) 
         VirtualFreeEx(hProcess, pszLibFileRemote, 0, MEM_RELEASE);

      if (hThread  != NULL) 
         CloseHandle(hThread);

      if (hProcess != NULL) 
         CloseHandle(hProcess);
   }

   return(bOk);
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI InjectLibA(DWORD dwProcessId, PCSTR pszLibFile) 
{
   // Allocate a (stack) buffer for the Unicode version of the pathname
   SIZE_T cchSize = lstrlenA(pszLibFile) + 1;
   PWSTR pszLibFileW = (PWSTR) 
      _alloca(cchSize * sizeof(wchar_t));

   // Convert the ANSI pathname to its Unicode equivalent
   StringCchPrintfW(pszLibFileW, cchSize, L"%S", pszLibFile);

   // Call the Unicode version of the function to actually do the work.
   return(InjectLibW(dwProcessId, pszLibFileW));
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI EjectLibW(DWORD dwProcessId, PCWSTR pszLibFile) 
{
   BOOL bOk = FALSE; // Assume that the function fails
   HANDLE hthSnapshot = NULL;
   HANDLE hProcess = NULL, hThread = NULL;

   __try 
   {
      // Grab a new snapshot of the process
      /* Takes a snapshot of the specified processes, 
	   * as well as the heaps, modules, and threads 
	   * used by these processes.
	   *  
	   * dwFlags [in]
       *   The portions of the system to be included in the snapshot. 
	   *   This parameter can be one or more of the following values.
	   *   TH32CS_SNAPMODULE
	   *   Includes all modules of the process 
	   *   specified in th32ProcessID in the snapshot. 
	   * 
	   */
      hthSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
      if (hthSnapshot == INVALID_HANDLE_VALUE) __leave;

      // Get the HMODULE of the desired library
      MODULEENTRY32W me = { sizeof(me) };
      BOOL bFound = FALSE;
	  /* Retrieves information about the first module associated with a process.
	   * hSnapshot [in]
       *  A handle to the snapshot returned from a previous call 
	   *  to the CreateToolhelp32Snapshot function.
       * lpme [in, out]
       *  A pointer to a MODULEENTRY32 structure.
	   */
      BOOL bMoreMods = Module32FirstW(hthSnapshot, &me);
	  /* Retrieves information about the next module 
	   * associated with a process or thread. */
      for (; bMoreMods; bMoreMods = Module32NextW(hthSnapshot, &me)) 
	  {
         bFound = (_wcsicmp(me.szModule,  pszLibFile) == 0) || 
                  (_wcsicmp(me.szExePath, pszLibFile) == 0);
         if (bFound) break;
      }
      if (!bFound) __leave;

      // Get a handle for the target process.
      hProcess = OpenProcess(
         PROCESS_QUERY_INFORMATION |   
         PROCESS_CREATE_THREAD     | 
         PROCESS_VM_OPERATION,  // For CreateRemoteThread
         FALSE, dwProcessId);
      if (hProcess == NULL) __leave;

      // Get the real address of FreeLibrary in Kernel32.dll
      PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
         GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "FreeLibrary");
      if (pfnThreadRtn == NULL) __leave;

      // Create a remote thread that calls FreeLibrary()
      hThread = CreateRemoteThread(hProcess, NULL, 0, 
         pfnThreadRtn, me.modBaseAddr, 0, NULL);
      if (hThread == NULL) __leave;

      // Wait for the remote thread to terminate
      WaitForSingleObject(hThread, INFINITE);

      bOk = TRUE; // Everything executed successfully
   }
   __finally { // Now we can clean everything up

      if (hthSnapshot != NULL) 
         CloseHandle(hthSnapshot);

      if (hThread     != NULL) 
         CloseHandle(hThread);

      if (hProcess    != NULL) 
         CloseHandle(hProcess);
   }

   return(bOk);
}


///////////////////////////////////////////////////////////////////////////////


BOOL WINAPI EjectLibA(DWORD dwProcessId, PCSTR pszLibFile) {

   // Allocate a (stack) buffer for the Unicode version of the pathname
   SIZE_T cchSize = lstrlenA(pszLibFile) + 1;
   PWSTR pszLibFileW = (PWSTR) 
      _alloca(cchSize * sizeof(wchar_t));

   // Convert the ANSI pathname to its Unicode equivalent
      StringCchPrintfW(pszLibFileW, cchSize, L"%S", pszLibFile);

   // Call the Unicode version of the function to actually do the work.
   return(EjectLibW(dwProcessId, pszLibFileW));
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   chSETDLGICONS(hWnd, IDI_INJLIB);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {
   
   switch (id) {
      case IDCANCEL:
         EndDialog(hWnd, id);
         break;

      case IDC_INJECT:
         DWORD dwProcessId = GetDlgItemInt(hWnd, IDC_PROCESSID, NULL, FALSE);
         if (dwProcessId == 0) {
            // A process ID of 0 causes everything to take place in the 
            // local process; this makes things easier for debugging.
            dwProcessId = GetCurrentProcessId();
         }

         TCHAR szLibFile[MAX_PATH];
		 /* Retrieves the fully qualified path for the file 
		  * that contains the specified module. 
		  * The module must have been loaded by the current process.
          * To locate the file for a module that was loaded by another process, 
		  * use the GetModuleFileNameEx function.
		  * 
		  * hModule [in, optional]
          *   A handle to the loaded module 
		  *   whose path is being requested. 
		  *   If this parameter is NULL, GetModuleFileName retrieves 
		  *   the path of the executable file of the current process.
		  */
         GetModuleFileName(NULL, szLibFile, _countof(szLibFile));
         PTSTR pFilename = _tcsrchr(szLibFile, TEXT('\\')) + 1;
         _tcscpy_s(pFilename, _countof(szLibFile) - (pFilename - szLibFile),
             TEXT("22-ImgWalk.DLL"));
         if (InjectLib(dwProcessId, szLibFile)) {
            chVERIFY(EjectLib(dwProcessId, szLibFile));
            chMB("DLL Injection/Ejection successful.");
         } else {
            chMB("DLL Injection/Ejection failed.");
         }
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hWnd, WM_COMMAND,    Dlg_OnCommand);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DialogBox(hInstExe, MAKEINTRESOURCE(IDD_INJLIB), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
