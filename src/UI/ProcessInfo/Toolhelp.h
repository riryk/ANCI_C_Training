/******************************************************************************
Module:  Toolhelp.h
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#pragma once


///////////////////////////////////////////////////////////////////////////////


#include "CmnHdr.h"     /* See Appendix A. */
#include <tlhelp32.h>
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////


class CToolhelp {
private:
   HANDLE m_hSnapshot;

public:
   CToolhelp(DWORD dwFlags = 0, DWORD dwProcessID = 0);
   ~CToolhelp();

   BOOL CreateSnapshot(DWORD dwFlags, DWORD dwProcessID = 0);
   
   BOOL ProcessFirst(PPROCESSENTRY32 ppe) const;
   BOOL ProcessNext(PPROCESSENTRY32 ppe) const;
   BOOL ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe) const;

   BOOL ModuleFirst(PMODULEENTRY32 pme) const;
   BOOL ModuleNext(PMODULEENTRY32 pme) const;
   BOOL ModuleFind(PVOID pvBaseAddr, PMODULEENTRY32 pme) const;
   BOOL ModuleFind(PTSTR pszModName, PMODULEENTRY32 pme) const;
   
   BOOL ThreadFirst(PTHREADENTRY32 pte) const;
   BOOL ThreadNext(PTHREADENTRY32 pte) const;
   
   BOOL HeapListFirst(PHEAPLIST32 phl) const;
   BOOL HeapListNext(PHEAPLIST32 phl) const;
   int  HowManyHeaps() const;

   // Note: The heap block functions do not reference a snapshot and
   // just walk the process's heap from the beginning each time. Infinite 
   // loops can occur if the target process changes its heap while the
   // functions below are enumerating the blocks in the heap.
   BOOL HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, 
      UINT_PTR dwHeapID) const;
   BOOL HeapNext(PHEAPENTRY32 phe) const;
   int  HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapId) const;
   BOOL IsAHeap(HANDLE hProcess, PVOID pvBlock, PDWORD pdwFlags) const;

public:
   static BOOL EnablePrivilege(PCTSTR szPrivilege, BOOL fEnable = TRUE);
   static BOOL ReadProcessMemory(DWORD dwProcessID, LPCVOID pvBaseAddress, 
      PVOID pvBuffer, SIZE_T cbRead, SIZE_T* pNumberOfBytesRead = NULL);
};


///////////////////////////////////////////////////////////////////////////////


inline CToolhelp::CToolhelp(DWORD dwFlags, DWORD dwProcessID) {

   m_hSnapshot = INVALID_HANDLE_VALUE;
   CreateSnapshot(dwFlags, dwProcessID);
}


///////////////////////////////////////////////////////////////////////////////


inline CToolhelp::~CToolhelp() {

   if (m_hSnapshot != INVALID_HANDLE_VALUE)
      CloseHandle(m_hSnapshot);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CToolhelp::CreateSnapshot(DWORD dwFlags, DWORD dwProcessID) {

   if (m_hSnapshot != INVALID_HANDLE_VALUE)
      CloseHandle(m_hSnapshot);

   if (dwFlags == 0) {
      m_hSnapshot = INVALID_HANDLE_VALUE;
   } else {
      m_hSnapshot = CreateToolhelp32Snapshot(dwFlags, dwProcessID);
   }
   return(m_hSnapshot != INVALID_HANDLE_VALUE);
}


inline BOOL CToolhelp::EnablePrivilege(PCTSTR szPrivilege, BOOL fEnable) 
{
   /* Enabling the debug privilege allows the application to see
    * information about service applications.
    * Assume function fails
	*/
   BOOL fOk = FALSE;   
   HANDLE hToken;

   /* Try to open this process's access token 
    * The OpenProcessToken function opens the access token associated with a process
	* Parameters:
	* 1. ProcessHandle [in] 
    *    A handle to the process whose access token is opened. 
	*    The process must have the PROCESS_QUERY_INFORMATION access permission.
    * 2. DesiredAccess [in] 
    *    Specifies an access mask that specifies 
	*    the requested types of access to the access token. 
	*    These requested access types are compared with the discretionary access 
	*    control list (DACL) of the token to determine which accesses are granted or denied.
    *    For a list of access rights for access tokens, see Access Rights for Access-Token Objects
	* 3. TokenHandle [out] 
    *    A pointer to a handle that identifies 
	*    the newly opened access token when the function returns
    *
    */
   if (OpenProcessToken(
	      GetCurrentProcess(), 
		  TOKEN_ADJUST_PRIVILEGES, 
          &hToken)) 
   {
      /* Attempt to modify the given privilege */
      TOKEN_PRIVILEGES tp;
      tp.PrivilegeCount = 1;
	  /* The LookupPrivilegeValue function retrieves the locally 
	   * unique identifier (LUID) used on a specified system 
	   * to locally represent the specified privilege name 
	   * Parameters:
	   * 1. lpSystemName [in, optional] 
       *    A pointer to a null-terminated string that specifies
	   *    the name of the system on which the privilege name is retrieved. 
	   *    If a null string is specified, the function attempts 
	   *    to find the privilege name on the local system.
	   * 2. lpName [in] 
       *    A pointer to a null-terminated string that specifies 
	   *    the name of the privilege, as defined in the Winnt.h header file. 
	   *    For example, this parameter could specify the constant, 
	   *    SE_SECURITY_NAME, or its corresponding string, "SeSecurityPrivilege".
       * 3. lpLuid [out] 
       *    A pointer to a variable that receives the LUID by which 
	   *    the privilege is known on the system specified by the lpSystemName parameter.
	   */
      LookupPrivilegeValue(NULL, szPrivilege, &tp.Privileges[0].Luid);
      tp.Privileges[0].Attributes = fEnable ? SE_PRIVILEGE_ENABLED : 0;
	  /* The AdjustTokenPrivileges function enables 
	   * or disables privileges in the specified access token.
	   * Enabling or disabling privileges in an access 
	   * token requires TOKEN_ADJUST_PRIVILEGES access 
	   * Parameters:
	   * 1. TokenHandle [in] 
       *    A handle to the access token that contains the privileges to be modified. 
	   *    The handle must have TOKEN_ADJUST_PRIVILEGES access to the token. 
	   *    If the PreviousState parameter is not NULL, the handle must also have TOKEN_QUERY access.
       * 2. DisableAllPrivileges [in] 
       *    Specifies whether the function disables all of the token's privileges. 
	   *    If this value is TRUE, the function disables all privileges and ignores the NewState parameter.
	   *    If it is FALSE, the function modifies privileges based on 
	   *    the information pointed to by the NewState parameter.
	   * 3. NewState [in, optional] 
       *    A pointer to a TOKEN_PRIVILEGES structure that specifies an array 
	   *    of privileges and their attributes. If the DisableAllPrivileges parameter is FALSE, 
	   *    the AdjustTokenPrivileges function enables, disables, or removes these privileges for the token. 
	   *    The following table describes the action taken by the AdjustTokenPrivileges function, 
	   *    based on the privilege attribute.
	   */
      AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
      fOk = (GetLastError() == ERROR_SUCCESS);

      /* Don't forget to close the token handle */
      CloseHandle(hToken);
   }
   return(fOk);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CToolhelp::ReadProcessMemory(DWORD dwProcessID, 
   LPCVOID pvBaseAddress, PVOID pvBuffer, SIZE_T cbRead, 
   SIZE_T* pNumberOfBytesRead) {

   return(Toolhelp32ReadProcessMemory(dwProcessID, pvBaseAddress, pvBuffer, 
      cbRead, pNumberOfBytesRead));
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CToolhelp::ProcessFirst(PPROCESSENTRY32 ppe) const {

   BOOL fOk = Process32First(m_hSnapshot, ppe);
   if (fOk && (ppe->th32ProcessID == 0))
      fOk = ProcessNext(ppe); // Remove the "[System Process]" (PID = 0)
   return(fOk);
}


inline BOOL CToolhelp::ProcessNext(PPROCESSENTRY32 ppe) const {

   BOOL fOk = Process32Next(m_hSnapshot, ppe);
   if (fOk && (ppe->th32ProcessID == 0))
      fOk = ProcessNext(ppe); // Remove the "[System Process]" (PID = 0)
   return(fOk);
}


inline BOOL CToolhelp::ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe) 
   const {

   BOOL fFound = FALSE;
   for (BOOL fOk = ProcessFirst(ppe); fOk; fOk = ProcessNext(ppe)) {
      fFound = (ppe->th32ProcessID == dwProcessId);
      if (fFound) break;
   }
   return(fFound);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CToolhelp::ModuleFirst(PMODULEENTRY32 pme) const {

   return(Module32First(m_hSnapshot, pme));
}


inline BOOL CToolhelp::ModuleNext(PMODULEENTRY32 pme) const {
   return(Module32Next(m_hSnapshot, pme));
}

inline BOOL CToolhelp::ModuleFind(PVOID pvBaseAddr, PMODULEENTRY32 pme) const {

   BOOL fFound = FALSE;
   for (BOOL fOk = ModuleFirst(pme); fOk; fOk = ModuleNext(pme)) {
      fFound = (pme->modBaseAddr == pvBaseAddr);
      if (fFound) break;
   }
   return(fFound);
}

inline BOOL CToolhelp::ModuleFind(PTSTR pszModName, PMODULEENTRY32 pme) const {
   BOOL fFound = FALSE;
   for (BOOL fOk = ModuleFirst(pme); fOk; fOk = ModuleNext(pme)) {
      fFound = (lstrcmpi(pme->szModule,  pszModName) == 0) || 
               (lstrcmpi(pme->szExePath, pszModName) == 0);
      if (fFound) break;
   }
   return(fFound);
}


///////////////////////////////////////////////////////////////////////////////


inline BOOL CToolhelp::ThreadFirst(PTHREADENTRY32 pte) const {

   return(Thread32First(m_hSnapshot, pte));
}


inline BOOL CToolhelp::ThreadNext(PTHREADENTRY32 pte) const {

   return(Thread32Next(m_hSnapshot, pte));
}


///////////////////////////////////////////////////////////////////////////////


inline int CToolhelp::HowManyHeaps() const {

   int nHowManyHeaps = 0;
   HEAPLIST32 hl = { sizeof(hl) };
   for (BOOL fOk = HeapListFirst(&hl); fOk; fOk = HeapListNext(&hl))
      nHowManyHeaps++;
   return(nHowManyHeaps);
}


inline int CToolhelp::HowManyBlocksInHeap(DWORD dwProcessID, 
   DWORD dwHeapID) const {

   int nHowManyBlocksInHeap = 0;
   HEAPENTRY32 he = { sizeof(he) };
   BOOL fOk = HeapFirst(&he, dwProcessID, dwHeapID);
   for (; fOk; fOk = HeapNext(&he))
      nHowManyBlocksInHeap++;
   return(nHowManyBlocksInHeap);
}


inline BOOL CToolhelp::HeapListFirst(PHEAPLIST32 phl) const {

   return(Heap32ListFirst(m_hSnapshot, phl));
}


inline BOOL CToolhelp::HeapListNext(PHEAPLIST32 phl) const {

   return(Heap32ListNext(m_hSnapshot, phl));
}


inline BOOL CToolhelp::HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, 
   UINT_PTR dwHeapID) const {

   return(Heap32First(phe, dwProcessID, dwHeapID));
}


inline BOOL CToolhelp::HeapNext(PHEAPENTRY32 phe) const {

   return(Heap32Next(phe));
}


inline BOOL CToolhelp::IsAHeap(HANDLE hProcess, PVOID pvBlock, 
   PDWORD pdwFlags) const {

   HEAPLIST32 hl = { sizeof(hl) };
   for (BOOL fOkHL = HeapListFirst(&hl); fOkHL; fOkHL = HeapListNext(&hl)) {
      HEAPENTRY32 he = { sizeof(he) };
      BOOL fOkHE = HeapFirst(&he, hl.th32ProcessID, hl.th32HeapID);
      for (; fOkHE; fOkHE = HeapNext(&he)) {
         MEMORY_BASIC_INFORMATION mbi;
         VirtualQueryEx(hProcess, (PVOID) he.dwAddress, &mbi, sizeof(mbi));
         if (chINRANGE(mbi.AllocationBase, pvBlock, 
            (PBYTE) mbi.AllocationBase + mbi.RegionSize)) {

            *pdwFlags = hl.dwFlags;
            return(TRUE);
         }
      }
   }
   return(FALSE);
}


//////////////////////////////// End of File //////////////////////////////////
