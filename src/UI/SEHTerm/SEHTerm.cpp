/******************************************************************************
Module:  SEHTerm.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/

#include "CmnHdr.h"     /* See Appendix A. */
#include <windows.h>
#include <tchar.h>


///////////////////////////////////////////////////////////////////////////////

BOOL CheckWindowsVersion(int Version)
{
	// Code from Chapter 4
   // Prepare the OSVERSIONINFOEX structure to indicate Windows Vista.
   OSVERSIONINFOEX osver = { 0 };
   osver.dwOSVersionInfoSize = sizeof(osver);
   osver.dwMajorVersion = 6;
   osver.dwMinorVersion = 0;
   osver.dwPlatformId = VER_PLATFORM_WIN32_NT;

   // Prepare the condition mask.
   DWORDLONG dwlConditionMask = 0;	// You MUST initialize this to 0.
   VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
   VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);
   VER_SET_CONDITION(dwlConditionMask, VER_PLATFORMID, VER_EQUAL);

   // Perform the version test.
   /* Compares a set of operating system version requirements 
    * to the corresponding values for the currently running version of the system. 
	* 
	* lpVersionInfo [in]
    *    A pointer to an OSVERSIONINFOEX structure containing 
	*    the operating system version requirements to compare. 
	*    The dwTypeMask parameter indicates the members of 
	*    this structure that contain information to compare.
    *    You must set the dwOSVersionInfoSize member of 
	*    this structure to sizeof(OSVERSIONINFOEX). 
	*    You must also specify valid data for the members indicated by dwTypeMask. 
	*    The function ignores structure members for which the corresponding dwTypeMask bit is not set.
	* 
	* dwTypeMask [in]
    *    A mask that indicates the members of the OSVERSIONINFOEX structure to be tested. 
	*    This parameter can be one or more of the following values.
    *    VER_MAJORVERSION
    *    0x0000002            dwMajorVersion
    *                    If you are testing the major version, you must also test the minor version 
	*                    and the service pack major and minor versions.   
	* 
	* dwlConditionMask [in]
    *    The type of comparison to be used for each lpVersionInfo member being compared. 
	*	 To build this value, call the VerSetConditionMask function 
	*	 or the VER_SET_CONDITION macro once for each OSVERSIONINFOEX member being compared.
	* 
	*/
   if (VerifyVersionInfo(&osver, VER_MAJORVERSION  | VER_MINORVERSION | 
      VER_PLATFORMID, dwlConditionMask)) {
      // The host system is Windows Vista exactly.
      return(TRUE);
   } else {
      // The host system is NOT Windows Vista.
      return(FALSE);
   }
}

BOOL IsWindows7()
{
   return CheckWindowsVersion(7);
}

BOOL IsWindowsVista() 
{
   return CheckWindowsVersion(6);
}

void TriggerException() {

   __try {
      int n = MessageBox(NULL, TEXT("Perform invalid memory access?"), 
         TEXT("SEHTerm: In try block"), MB_YESNO);
      
      if (n == IDYES) {
         * (PBYTE) NULL = 5;  // This causes an access violation
      }
   }
   __finally {
      PCTSTR psz = AbnormalTermination() 
         ? TEXT("Abnormal termination") : TEXT("Normal termination");
      MessageBox(NULL, psz, TEXT("SEHTerm: In finally block"), MB_OK);
   }

   MessageBox(NULL, TEXT("Normal function termination"), 
      TEXT("SEHTerm: After finally block"), MB_OK);
}


int WINAPI _tWinMain(HINSTANCE, HINSTANCE, PTSTR, int) {

   // In Windows Vista, a global unwind occurs if an except filter
   // returns EXCEPTION_EXECUTE_HANDLER. If an unhandled exception
   // occurs, the process is simply terminated and the finally blocks
   // are not exectuted.
   if (IsWindowsVista() || IsWindows7()) {

      DWORD n = MessageBox(NULL, TEXT("Protect with try/except?"), 
         TEXT("SEHTerm: workflow"), MB_YESNO);

      if (n == IDYES) {
         __try {
            TriggerException();
         }
         __except (EXCEPTION_EXECUTE_HANDLER) {
            // But the system dialog will not appear.
            // So, popup a message box.
            MessageBox(NULL, TEXT("Abnormal process termination"), 
               TEXT("Process entry point try/except handler"), MB_OK);

            // Exit with a dedicated error code
            return(-1);
         }
      } else {
         TriggerException();
      }
   } else {
      TriggerException();
   }
   
   MessageBox(NULL, TEXT("Normal process termination"), 
      TEXT("SEHTerm: before leaving the main thread"), MB_OK);

   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
