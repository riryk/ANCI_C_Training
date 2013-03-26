

#include "CmnHdr.h"     
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include "Resource.h"
#include "Job.h"


#include <StrSafe.h>

#pragma comment (lib, "psapi.lib")  



/* Job object */
CJob   g_job;   
/* Handle to dialog box (accessible by all threads) */
HWND   g_hwnd;        

/* Completion port that receives Job notifications */
HANDLE g_hIOCP;        

/* Completion port thread */
HANDLE g_hThreadIOCP;   

/* Completion keys for the completion port */
#define COMPKEY_TERMINATE  ((UINT_PTR) 0)
#define COMPKEY_STATUS     ((UINT_PTR) 1)
#define COMPKEY_JOBOBJECT  ((UINT_PTR) 2)


void GetProcessName(DWORD PID, PTSTR szProcessName, size_t cchSize)
{
   /* Opens an existing local process object. */
   HANDLE hProcess = 
	   OpenProcess(
	       PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
           FALSE, 
		   PID);

   if (hProcess == NULL) 
   {
      _tcscpy_s(szProcessName, cchSize, TEXT("???"));
      return;
   }

   DWORD dwSize = (DWORD)cchSize;
   /* Retrieves the full name of the executable image 
    * for the specified process. 
    */
   QueryFullProcessImageName(
	   hProcess, 
	   0, 
	   szProcessName, 
	   &dwSize);

   /* Don't forget to close the process handle */
   CloseHandle(hProcess);
}

/* This function is launched in a separate thread and waits for 
 * a message from a completion port associated with a job.
 */
DWORD WINAPI JobNotify(PVOID) 
{
   TCHAR sz[2000];
   BOOL fDone = FALSE;

   while (!fDone) 
   {
      DWORD dwBytesXferred;
      ULONG_PTR CompKey;
      LPOVERLAPPED po;
      
	  /* Attempts to dequeue an I/O completion packet 
	   * from the specified I/O completion port. 
	   * If there is no completion packet queued, the function 
	   * waits for a pending I/O operation associated with the completion port to complete.
	   *
       * To dequeue multiple I/O completion packets at once, 
	   * use the GetQueuedCompletionStatusEx function
	   *  
	   * CompletionPort [in] 
       *   A handle to the completion port. To create a completion port, 
	   *   use the CreateIoCompletionPort function.
	   *
	   * lpNumberOfBytes [out] 
       *   A pointer to a variable that receives the number of bytes 
	   *   transferred during an I/O operation that has completed
       *   
	   * lpCompletionKey [out] 
       *   A pointer to a variable that receives the completion key value 
	   *   associated with the file handle whose I/O operation has completed. 
	   *   A completion key is a per-file key that is specified in a call to CreateIoCompletionPort.
       *
	   * lpOverlapped [out]  
       *   A pointer to a variable that receives the address of the 
	   *   OVERLAPPED structure that was specified when the completed I/O operation was started. 
	   * 
	   * dwMilliseconds [in] 
       *   The number of milliseconds that the caller is willing to wait 
	   *   for a completion packet to appear at the completion port. 
	   *   If a completion packet does not appear within the specified time, 
	   *   the function times out, returns FALSE, and sets *lpOverlapped to NULL.
	   *
       *   If dwMilliseconds is INFINITE, the function will never time out. 
	   *   If dwMilliseconds is zero and there is no I/O operation to dequeue, 
	   *   the function will time out immediately.
       */
      GetQueuedCompletionStatus(
		  g_hIOCP, 
          &dwBytesXferred, 
		  &CompKey, 
		  &po, 
		  INFINITE);

      /* The app is shutting down, exit this thread */
      fDone = (CompKey == COMPKEY_TERMINATE);

      HWND hwndLB = FindWindow(NULL, TEXT("Job Lab"));
      hwndLB = GetDlgItem(hwndLB, IDC_STATUS);

      if (CompKey == COMPKEY_JOBOBJECT) 
	  {
         _tcscpy_s(sz, _countof(sz), TEXT("--> Notification: "));
         PTSTR psz = sz + _tcslen(sz);
         switch (dwBytesXferred) 
		 {
			 /* We have reached an event when job time was reached */
             case JOB_OBJECT_MSG_END_OF_JOB_TIME:
                  StringCchPrintf(
					    psz, 
						_countof(sz) - _tcslen(sz), 
                        TEXT("Job time limit reached"));
             break;
			 /* The time allowed for process in the job is exceeded.
			  * So we get process name from the overlapped structure.
			  */
             case JOB_OBJECT_MSG_END_OF_PROCESS_TIME: 
			 {
                  TCHAR szProcessName[MAX_PATH];
                  
				  /* po is a process identifier. */
                  GetProcessName(
					  PtrToUlong(po), 
					  szProcessName, 
					  MAX_PATH);

                  StringCchPrintf(
					  psz, 
					  _countof(sz) - _tcslen(sz), 
                      TEXT("Job process %s (Id=%d) time limit reached"), 
                      szProcessName, 
					  po);
             }
             break;

             case JOB_OBJECT_MSG_ACTIVE_PROCESS_LIMIT:
                  StringCchPrintf(
					  psz, 
					  _countof(sz) - _tcslen(sz), 
                      TEXT("Too many active processes in job"));
             break;

             case JOB_OBJECT_MSG_ACTIVE_PROCESS_ZERO:
                  StringCchPrintf(
					  psz, 
					  _countof(sz) - _tcslen(sz), 
                      TEXT("Job contains no active processes"));
             break;

             case JOB_OBJECT_MSG_NEW_PROCESS: 
			 {
                  TCHAR szProcessName[MAX_PATH];
                  GetProcessName(
					  PtrToUlong(po), 
					  szProcessName, 
					  MAX_PATH);

                  StringCchPrintf(
					  psz, 
					  _countof(sz) - _tcslen(sz), 
                      TEXT("New process %s (Id=%d) in Job"), 
					  szProcessName, 
					  po);
             }
             break;

             case JOB_OBJECT_MSG_EXIT_PROCESS: 
			 {
                  TCHAR szProcessName[MAX_PATH];
                  GetProcessName(
					 PtrToUlong(po), 
					 szProcessName, 
					 MAX_PATH);

                  StringCchPrintf(
					  psz, 
					  _countof(sz) - _tcslen(sz), 
                      TEXT("Process %s (Id=%d) terminated"), 
					  szProcessName, 
					  po);
             }
             break;

             case JOB_OBJECT_MSG_ABNORMAL_EXIT_PROCESS: 
			 {
                 TCHAR szProcessName[MAX_PATH];
                 GetProcessName(
					 PtrToUlong(po), 
					 szProcessName, 
					 MAX_PATH);

                 StringCchPrintf(
					 psz, 
					 _countof(sz) - _tcslen(sz), 
                     TEXT("Process %s (Id=%d) terminated abnormally"), 
                     szProcessName, 
					 po);
             }
             break;

             case JOB_OBJECT_MSG_PROCESS_MEMORY_LIMIT: 
		     {
                 TCHAR szProcessName[MAX_PATH];
                 GetProcessName(
					 PtrToUlong(po), 
					 szProcessName, 
					 MAX_PATH);

                 StringCchPrintf(
					 psz, 
					 _countof(sz) - _tcslen(sz), 
                     TEXT("Process (%s Id=%d) exceeded memory limit"), 
                     szProcessName, 
					 po);
             }
             break;

             case JOB_OBJECT_MSG_JOB_MEMORY_LIMIT: 
			 {
                 TCHAR szProcessName[MAX_PATH];
                 GetProcessName(
					 PtrToUlong(po), 
					 szProcessName, 
					 MAX_PATH);

                 StringCchPrintf(
					 psz, 
					 _countof(sz) - _tcslen(sz),
                     TEXT("Process %s (Id=%d) exceeded job memory limit"), 
                     szProcessName, 
					 po);
             }
             break;

             default:
                 StringCchPrintf(
					 psz, 
					 _countof(sz) - _tcslen(sz),
                     TEXT("Unknown notification: %d"), 
					 dwBytesXferred);
             break;
         }
         ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));
         /* Force a status update when a notification arrives */
         CompKey = 1; 
      }

      if (CompKey == COMPKEY_STATUS) 
	  {
         static int s_nStatusCount = 0;

         StringCchPrintf(
			 sz, 
			 _countof(sz), 
             TEXT("--> Status Update (%u)"), 
			 s_nStatusCount++);

         ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));

         /* Show the basic accounting information */
         JOBOBJECT_BASIC_AND_IO_ACCOUNTING_INFORMATION jobai;

         g_job.QueryBasicAccountingInfo(&jobai);

         StringCchPrintf(
			 sz, 
			 _countof(sz), 
             TEXT("Total Time: User=%I64u, Kernel=%I64u        ")
             TEXT("Period Time: User=%I64u, Kernel=%I64u"), 
             jobai.BasicInfo.TotalUserTime.QuadPart, 
             jobai.BasicInfo.TotalKernelTime.QuadPart,
             jobai.BasicInfo.ThisPeriodTotalUserTime.QuadPart, 
             jobai.BasicInfo.ThisPeriodTotalKernelTime.QuadPart);

         ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));

         StringCchPrintf(
			 sz, 
			 _countof(sz), 
             TEXT("Page Faults=%u, Total Processes=%u, ")
             TEXT("Active Processes=%u, Terminated Processes=%u"), 
             jobai.BasicInfo.TotalPageFaultCount, 
             jobai.BasicInfo.TotalProcesses, 
             jobai.BasicInfo.ActiveProcesses, 
             jobai.BasicInfo.TotalTerminatedProcesses);

         ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));

         /* Show the I/O accounting information */
         StringCchPrintf(
			 sz, 
			 _countof(sz), 
             TEXT("Reads=%I64u (%I64u bytes), ")
             TEXT("Write=%I64u (%I64u bytes), Other=%I64u (%I64u bytes)"), 
             jobai.IoInfo.ReadOperationCount,  
			 jobai.IoInfo.ReadTransferCount, 
             jobai.IoInfo.WriteOperationCount, 
			 jobai.IoInfo.WriteTransferCount, 
             jobai.IoInfo.OtherOperationCount, 
			 jobai.IoInfo.OtherTransferCount);

         ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));

         /* Show the peak per-process and job memory usage */
         JOBOBJECT_EXTENDED_LIMIT_INFORMATION joeli;
         g_job.QueryExtendedLimitInfo(&joeli);

         StringCchPrintf(
			 sz, 
			 _countof(sz), 
             TEXT("Peak memory used: Process=%I64u, Job=%I64u"), 
             (__int64)joeli.PeakProcessMemoryUsed, 
             (__int64)joeli.PeakJobMemoryUsed);
         ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));

         /* Show the set of Process IDs */
         DWORD dwNumProcesses = 50;
         DWORD dwProcessIdList[50];

         g_job.QueryBasicProcessIdList(
			 dwNumProcesses, 
             dwProcessIdList, 
			 &dwNumProcesses);

         StringCchPrintf(
			 sz, 
			 _countof(sz), 
			 TEXT("PIDs: %s"), 
             (dwNumProcesses == 0) ? TEXT("(none)") : TEXT(""));

         ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));

         TCHAR szProcessName[MAX_PATH];

         for (DWORD x = 0; x < dwNumProcesses; x++) 
		 {
            GetProcessName(
				dwProcessIdList[x], 
                szProcessName, 
				_countof(szProcessName));

            StringCchPrintf(
				sz, 
				_countof(sz), 
				TEXT("   %d - %s"), 
                dwProcessIdList[x], 
				szProcessName);

            ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));
         }
      }
   }

   return(0);
}


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
   chSETDLGICONS(hwnd, IDI_JOBLAB);

   // Save our window handle so that the completion port thread can access it
   g_hwnd = hwnd; 
   
   HWND hwndPriorityClass = GetDlgItem(hwnd, IDC_PRIORITYCLASS);
   ComboBox_AddString(hwndPriorityClass, TEXT("No limit"));
   ComboBox_AddString(hwndPriorityClass, TEXT("Idle"));
   ComboBox_AddString(hwndPriorityClass, TEXT("Below normal"));
   ComboBox_AddString(hwndPriorityClass, TEXT("Normal"));
   ComboBox_AddString(hwndPriorityClass, TEXT("Above normal"));
   ComboBox_AddString(hwndPriorityClass, TEXT("High"));
   ComboBox_AddString(hwndPriorityClass, TEXT("Realtime"));
   ComboBox_SetCurSel(hwndPriorityClass, 0); // Default to "No Limit"

   HWND hwndSchedulingClass = GetDlgItem(hwnd, IDC_SCHEDULINGCLASS);
   ComboBox_AddString(hwndSchedulingClass, TEXT("No limit"));
   for (int n = 0; n <= 9; n++) {
      TCHAR szSchedulingClass[2];
      StringCchPrintf(szSchedulingClass, _countof(szSchedulingClass), 
         TEXT("%u"), n);
      ComboBox_AddString(hwndSchedulingClass, szSchedulingClass);
   }
   ComboBox_SetCurSel(hwndSchedulingClass, 0); // Default to "No Limit"
   SetTimer(hwnd, 1, 10000, NULL);             // 10 second accounting update
   return(TRUE);
}



void Dlg_ApplyLimits(HWND hwnd) {
   const int nNanosecondsPerSecond = 1000000000;
   const int nMillisecondsPerSecond = 1000;
   const int nNanosecondsPerMillisecond = 
      nNanosecondsPerSecond / nMillisecondsPerSecond;
   BOOL f;
   __int64 q;
   SIZE_T s;
   DWORD d;

   // Set Basic and Extended Limits
   JOBOBJECT_EXTENDED_LIMIT_INFORMATION joeli = { 0 };
   joeli.BasicLimitInformation.LimitFlags = 0;
   
   q = GetDlgItemInt(hwnd, IDC_PERPROCESSUSERTIMELIMIT, &f, FALSE);
   if (f) {
      joeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PROCESS_TIME;
      joeli.BasicLimitInformation.PerProcessUserTimeLimit.QuadPart = 
         q * nNanosecondsPerMillisecond / 100;
   }

   q = GetDlgItemInt(hwnd, IDC_PERJOBUSERTIMELIMIT, &f, FALSE);
   if (f) {
      joeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_TIME;
      joeli.BasicLimitInformation.PerJobUserTimeLimit.QuadPart = 
         q * nNanosecondsPerMillisecond / 100;
   }

   s = GetDlgItemInt(hwnd, IDC_MINWORKINGSETSIZE, &f, FALSE);
   if (f) {
      joeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_WORKINGSET;
      joeli.BasicLimitInformation.MinimumWorkingSetSize = s * 1024 * 1024;
      s = GetDlgItemInt(hwnd, IDC_MAXWORKINGSETSIZE, &f, FALSE);
      if (f) {
         joeli.BasicLimitInformation.MaximumWorkingSetSize = s * 1024 * 1024;
      } else {
         joeli.BasicLimitInformation.LimitFlags &=~JOB_OBJECT_LIMIT_WORKINGSET;
         chMB("Both minimum and maximum working set sizes must be set.\n"
            "The working set limits will NOT be in effect.");
      }
   }

   d = GetDlgItemInt(hwnd, IDC_ACTIVEPROCESSLIMIT, &f, FALSE);
   if (f) {
      joeli.BasicLimitInformation.LimitFlags |= 
         JOB_OBJECT_LIMIT_ACTIVE_PROCESS;
      joeli.BasicLimitInformation.ActiveProcessLimit = d;
   }

   s = GetDlgItemInt(hwnd, IDC_AFFINITYMASK, &f, FALSE);
   if (f) {
      joeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_AFFINITY;
      joeli.BasicLimitInformation.Affinity = s;
   }

   joeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_PRIORITY_CLASS;
   switch (ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_PRIORITYCLASS))) {
      case 0: 
         joeli.BasicLimitInformation.LimitFlags &= 
            ~JOB_OBJECT_LIMIT_PRIORITY_CLASS;
         break;

      case 1: 
         joeli.BasicLimitInformation.PriorityClass = 
            IDLE_PRIORITY_CLASS;
         break;

      case 2: 
         joeli.BasicLimitInformation.PriorityClass = 
            BELOW_NORMAL_PRIORITY_CLASS; 
         break;

      case 3: 
         joeli.BasicLimitInformation.PriorityClass = 
            NORMAL_PRIORITY_CLASS;       
         break;

      case 4: 
         joeli.BasicLimitInformation.PriorityClass = 
            ABOVE_NORMAL_PRIORITY_CLASS; 
         break;

      case 5: 
         joeli.BasicLimitInformation.PriorityClass = 
            HIGH_PRIORITY_CLASS;         
         break;

      case 6: 
         joeli.BasicLimitInformation.PriorityClass = 
            REALTIME_PRIORITY_CLASS;     
         break;
   }

   int nSchedulingClass = 
      ComboBox_GetCurSel(GetDlgItem(hwnd, IDC_SCHEDULINGCLASS));
   if (nSchedulingClass > 0) {
      joeli.BasicLimitInformation.LimitFlags |= 
         JOB_OBJECT_LIMIT_SCHEDULING_CLASS;
      joeli.BasicLimitInformation.SchedulingClass = nSchedulingClass - 1;
   }

   s = GetDlgItemInt(hwnd, IDC_MAXCOMMITPERJOB, &f, FALSE);
   if (f) {
      joeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_JOB_MEMORY;
      joeli.JobMemoryLimit = s * 1024 * 1024;
   }

   s = GetDlgItemInt(hwnd, IDC_MAXCOMMITPERPROCESS, &f, FALSE);
   if (f) {
      joeli.BasicLimitInformation.LimitFlags |= 
         JOB_OBJECT_LIMIT_PROCESS_MEMORY;
      joeli.ProcessMemoryLimit = s * 1024 * 1024;
   }

   if (IsDlgButtonChecked(hwnd, IDC_CHILDPROCESSESCANBREAKAWAYFROMJOB))
      joeli.BasicLimitInformation.LimitFlags |= JOB_OBJECT_LIMIT_BREAKAWAY_OK;

   if (IsDlgButtonChecked(hwnd, IDC_CHILDPROCESSESDOBREAKAWAYFROMJOB))
      joeli.BasicLimitInformation.LimitFlags |= 
      JOB_OBJECT_LIMIT_SILENT_BREAKAWAY_OK;

   if (IsDlgButtonChecked(hwnd, IDC_TERMINATEPROCESSONEXCEPTIONS))
      joeli.BasicLimitInformation.LimitFlags |= 
      JOB_OBJECT_LIMIT_DIE_ON_UNHANDLED_EXCEPTION;

   f = g_job.SetExtendedLimitInfo(&joeli, 
      ((joeli.BasicLimitInformation.LimitFlags & JOB_OBJECT_LIMIT_JOB_TIME) 
         != 0) ? FALSE : 
         IsDlgButtonChecked(hwnd, IDC_PRESERVEJOBTIMEWHENAPPLYINGLIMITS));
   chASSERT(f);

   // Set UI Restrictions
   DWORD jobuir =  JOB_OBJECT_UILIMIT_NONE;  // A fancy zero (0)
   if (IsDlgButtonChecked(hwnd, IDC_RESTRICTACCESSTOOUTSIDEUSEROBJECTS))
      jobuir |=  JOB_OBJECT_UILIMIT_HANDLES;

   if (IsDlgButtonChecked(hwnd, IDC_RESTRICTREADINGCLIPBOARD))
      jobuir |=  JOB_OBJECT_UILIMIT_READCLIPBOARD;

   if (IsDlgButtonChecked(hwnd, IDC_RESTRICTWRITINGCLIPBOARD))
      jobuir |=  JOB_OBJECT_UILIMIT_WRITECLIPBOARD;

   if (IsDlgButtonChecked(hwnd, IDC_RESTRICTEXITWINDOW))
      jobuir |=  JOB_OBJECT_UILIMIT_EXITWINDOWS;

   if (IsDlgButtonChecked(hwnd, IDC_RESTRICTCHANGINGSYSTEMPARAMETERS))
      jobuir |=  JOB_OBJECT_UILIMIT_SYSTEMPARAMETERS;
   
   if (IsDlgButtonChecked(hwnd, IDC_RESTRICTDESKTOPS))
      jobuir |= JOB_OBJECT_UILIMIT_DESKTOP;

   if (IsDlgButtonChecked(hwnd, IDC_RESTRICTDISPLAYSETTINGS))
      jobuir |= JOB_OBJECT_UILIMIT_DISPLAYSETTINGS;

   if (IsDlgButtonChecked(hwnd, IDC_RESTRICTGLOBALATOMS))
      jobuir |=  JOB_OBJECT_UILIMIT_GLOBALATOMS;
   
   chVERIFY(g_job.SetBasicUIRestrictions(jobuir));
}



void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         // User is terminating our app, kill the job too.
         KillTimer(hwnd, 1);
         g_job.Terminate(0);  
         EndDialog(hwnd, id);
         break;

      case IDC_PERJOBUSERTIMELIMIT:
         {
         // The job time must be reset if setting a job time limit
         BOOL f;
         GetDlgItemInt(hwnd, IDC_PERJOBUSERTIMELIMIT, &f, FALSE);
         EnableWindow(
            GetDlgItem(hwnd, IDC_PRESERVEJOBTIMEWHENAPPLYINGLIMITS), !f);
         }
         break;

      case IDC_APPLYLIMITS:
         Dlg_ApplyLimits(hwnd);
         PostQueuedCompletionStatus(g_hIOCP, 0, COMPKEY_STATUS, NULL);
         break;

      case IDC_TERMINATE:
         g_job.Terminate(0);  
         PostQueuedCompletionStatus(g_hIOCP, 0, COMPKEY_STATUS, NULL);
         break;

      case IDC_SPAWNCMDINJOB:
         {
         // Spawn a command shell and place it in the job
         STARTUPINFO si = { sizeof(si) };
         PROCESS_INFORMATION pi;
         TCHAR sz[] = TEXT("CMD");
         CreateProcess(NULL, sz, NULL, NULL, 
            FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi);
         g_job.AssignProcess(pi.hProcess);
         ResumeThread(pi.hThread);
         CloseHandle(pi.hProcess);
         CloseHandle(pi.hThread);
         }
         PostQueuedCompletionStatus(g_hIOCP, 0, COMPKEY_STATUS, NULL);
         break;

      case IDC_ASSIGNPROCESSTOJOB:
         {
         DWORD dwProcessId = GetDlgItemInt(hwnd, IDC_PROCESSID, NULL, FALSE);
         HANDLE hProcess = OpenProcess(
            PROCESS_SET_QUOTA | PROCESS_TERMINATE, FALSE, dwProcessId);
         if (hProcess != NULL) {
            chVERIFY(g_job.AssignProcess(hProcess));
            CloseHandle(hProcess);
         } else chMB("Could not assign process to job.");
         }
         PostQueuedCompletionStatus(g_hIOCP, 0, COMPKEY_STATUS, NULL);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


void WINAPI Dlg_OnTimer(HWND hwnd, UINT id) {

   PostQueuedCompletionStatus(g_hIOCP, 0, COMPKEY_STATUS, NULL);
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_TIMER,      Dlg_OnTimer);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
   }

   return(FALSE);
}

int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) 
{
   /* Check if we are not already associated with a job.
    * If this is the case, there is no way to switch to
    * another job.
	*/
   BOOL bInJob = FALSE;
   IsProcessInJob(GetCurrentProcess(), NULL, &bInJob);
   if (bInJob) 
   {
      MessageBox(
		  NULL, 
		  TEXT("Process already in a job"), 
          TEXT(""), 
		  MB_ICONINFORMATION | MB_OK);

      return(-1);
   }

   /* Create the completion port that receives job notifications 
    * Creates an input/output (I/O) completion port 
	* and associates it with a specified file handle, 
	* or creates an I/O completion port that is not 
	* yet associated with a file handle, allowing association at a later time.
    * 
	*  Associating an instance of an opened file handle with an I/O completion 
	*  port allows a process to receive notification of the completion 
	*  of asynchronous I/O operations involving that file handle.
	*
    * FileHandle [in] 
    *   An open file handle or INVALID_HANDLE_VALUE.
    *   The handle must be to an object that supports overlapped I/O.
    *   If a handle is provided, it has to have been opened 
	*   for overlapped I/O completion. 
	*   For example, you must specify the FILE_FLAG_OVERLAPPED flag
	*   when using the CreateFile function to obtain the handle.
	*
    *   If INVALID_HANDLE_VALUE is specified, 
	*   the function creates an I/O completion port without associating it 
	*   with a file handle. In this case, the ExistingCompletionPort parameter 
	*   must be NULL and the CompletionKey parameter is ignored.
    * 
	* ExistingCompletionPort [in, optional] 
    *   A handle to an existing I/O completion port or NULL.
    *   If this parameter is NULL, the function creates a new I/O completion port and, 
	*   if the FileHandle parameter is valid, associates it with the new I/O completion port. 
	*   Otherwise no file handle association occurs. 
	*   The function returns the handle to the new I/O completion port if successful.
	*  
	*  
    */
   g_hIOCP = CreateIoCompletionPort(
	   INVALID_HANDLE_VALUE, 
	   NULL, 
	   0, 
	   0);

   /* Create a thread that waits on the completion port */
   g_hThreadIOCP = chBEGINTHREADEX(
	   NULL, 
	   0, 
	   JobNotify, 
	   NULL, 
	   0, 
	   NULL);

   /* Create the job object */
   g_job.Create(NULL, TEXT("JobLab"));
   g_job.SetEndOfJobInfo(JOB_OBJECT_POST_AT_END_OF_JOB);
   g_job.AssociateCompletionPort(g_hIOCP, COMPKEY_JOBOBJECT);

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_JOBLAB), NULL, Dlg_Proc);

   // Post a special key that tells the completion port thread to terminate
   PostQueuedCompletionStatus(g_hIOCP, 0, COMPKEY_TERMINATE, NULL);

   // Wait for the completion port thread to terminate
   WaitForSingleObject(g_hThreadIOCP, INFINITE);
   
   // Clean up everything properly
   CloseHandle(g_hIOCP);
   CloseHandle(g_hThreadIOCP);

   // NOTE: The job is closed when the g_job's destructor is called.
   return(0);
}


///////////////////////////////// End Of File /////////////////////////////////