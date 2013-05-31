/******************************************************************************
Module:  SysInfo.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "CmnHdr.h"     /* See Appendix A. */
#include <windowsx.h>
#include <tchar.h>
#include <stdio.h>
#include "Resource.h"
#include <StrSafe.h>



///////////////////////////////////////////////////////////////////////////////



// This function accepts a number and converts it to a
// string, inserting commas where appropriate.
PTSTR BigNumToString(LONG lNum, PTSTR szBuf, DWORD chBufSize) {

   TCHAR szNum[100];
   StringCchPrintf(szNum, _countof(szNum), TEXT("%d"), lNum);
   NUMBERFMT nf;
   nf.NumDigits = 0;
   nf.LeadingZero = FALSE;
   nf.Grouping = 3;
   nf.lpDecimalSep = TEXT(".");
   nf.lpThousandSep = TEXT(",");
   nf.NegativeOrder = 0;
   GetNumberFormat(LOCALE_USER_DEFAULT, 0, szNum, &nf, szBuf, chBufSize);
   return(szBuf);
}


///////////////////////////////////////////////////////////////////////////////


void ShowCPUInfo(HWND hWnd, WORD wProcessorArchitecture, WORD wProcessorLevel, 
   WORD wProcessorRevision) {

   TCHAR szCPUArch[64]  = TEXT("(unknown)");
   TCHAR szCPULevel[64] = TEXT("(unknown)");
   TCHAR szCPURev[64]   = TEXT("(unknown)");

   switch (wProcessorArchitecture) {
      // Notice that AMD processors are seen as PROCESSOR_ARCHITECTURE_INTEL.
      // In the Registry, the content of the "VendorIdentifier" key under 
      // HKEY_LOCAL_MACHINE\HARDWARE\DESCRIPTION\System\CentralProcessor\0
      // is either "GenuineIntel" or "AuthenticAMD"
      // 
      // Read http://download.intel.com/design/Xeon/applnots/24161831.pdf 
      // for Model numeric codes.
      // http://www.amd.com/us-en/assets/content_type/white_papers_and_tech_docs/20734.pdf
      // should be used for AMD processors Model numeric codes.
      //
      case PROCESSOR_ARCHITECTURE_INTEL: 
         _tcscpy_s(szCPUArch, _countof(szCPUArch), TEXT("Intel")); 
         switch (wProcessorLevel) {
         case 3: case 4:
            StringCchPrintf(szCPULevel, _countof(szCPULevel), TEXT("80%c86"), wProcessorLevel + '0');
            StringCchPrintf(szCPURev, _countof(szCPURev), TEXT("%c%d"), 
               HIBYTE(wProcessorRevision) + TEXT('A'), 
               LOBYTE(wProcessorRevision));
            break;

         case 5:
            _tcscpy_s(szCPULevel, _countof(szCPULevel), TEXT("Pentium"));
            StringCchPrintf(szCPURev, _countof(szCPURev), TEXT("Model %d, Stepping %d"),
               HIBYTE(wProcessorRevision), LOBYTE(wProcessorRevision));
            break;

         case 6:
            switch (HIBYTE(wProcessorRevision)) { // Model
               case 1: 
                  _tcscpy_s(szCPULevel, _countof(szCPULevel), 
                     TEXT("Pentium Pro"));
                  break; 
                  
               case 3:
               case 5: 
                  _tcscpy_s(szCPULevel, _countof(szCPULevel), 
                     TEXT("Pentium II"));
                  break; 
               
               case 6: 
                  _tcscpy_s(szCPULevel, _countof(szCPULevel), 
                     TEXT("Celeron"));
                  break; 
               
               case 7:
               case 8:
               case 11: 
                  _tcscpy_s(szCPULevel, _countof(szCPULevel), 
                     TEXT("Pentium III"));
                  break; 

               case 9:
               case 13:
                  _tcscpy_s(szCPULevel, _countof(szCPULevel), 
                     TEXT("Pentium M"));
                  break; 
               
               case 10:
                  _tcscpy_s(szCPULevel, _countof(szCPULevel), 
                     TEXT("Pentium Xeon"));
                  break; 

               case 15:
                  _tcscpy_s(szCPULevel, _countof(szCPULevel), 
                     TEXT("Core 2 Duo"));
                  break; 

               default:
                  _tcscpy_s(szCPULevel, _countof(szCPULevel), 
                     TEXT("Unknown Pentium"));
                  break;
            } 
            StringCchPrintf(szCPURev, _countof(szCPURev), TEXT("Model %d, Stepping %d"),
               HIBYTE(wProcessorRevision), LOBYTE(wProcessorRevision));
            break;

         case 15: 
            _tcscpy_s(szCPULevel, _countof(szCPULevel), TEXT("Pentium 4"));
            StringCchPrintf(szCPURev, _countof(szCPURev), TEXT("Model %d, Stepping %d"),
               HIBYTE(wProcessorRevision), LOBYTE(wProcessorRevision));
            break; 
         }
         break;

      case PROCESSOR_ARCHITECTURE_IA64:
         _tcscpy_s(szCPUArch, _countof(szCPUArch), TEXT("IA-64"));
         StringCchPrintf(szCPULevel, _countof(szCPULevel), TEXT("%d"), wProcessorLevel);
         StringCchPrintf(szCPURev, _countof(szCPURev), TEXT("Model %c, Pass %d"), 
            HIBYTE(wProcessorRevision) + TEXT('A'), 
            LOBYTE(wProcessorRevision));
         break;

      case PROCESSOR_ARCHITECTURE_AMD64:
         _tcscpy_s(szCPUArch, _countof(szCPUArch), TEXT("AMD64"));
         StringCchPrintf(szCPULevel, _countof(szCPULevel), TEXT("%d"), wProcessorLevel);
         StringCchPrintf(szCPURev, _countof(szCPURev), TEXT("Model %c, Pass %d"), 
            HIBYTE(wProcessorRevision) + TEXT('A'), 
            LOBYTE(wProcessorRevision));
         break;


      case PROCESSOR_ARCHITECTURE_UNKNOWN:
      default:
         _tcscpy_s(szCPUArch, _countof(szCPUArch), TEXT("Unknown"));
         break;
   }
   SetDlgItemText(hWnd, IDC_PROCARCH,  szCPUArch);
   SetDlgItemText(hWnd, IDC_PROCLEVEL, szCPULevel);
   SetDlgItemText(hWnd, IDC_PROCREV,   szCPURev);
}


void ShowBitness(HWND hWnd) {
   TCHAR szFullTitle[100];
   TCHAR szTitle[32];
   GetWindowText(hWnd, szTitle, _countof(szTitle));

#if defined(_WIN64)
// 64-bit applications can only run on 64-bit Windows, 
// so there is nothing special to check except the
// _WIN64 symbol set by the compiler.
   StringCchPrintf(szFullTitle, _countof(szFullTitle), 
      TEXT("64-bit %s"), szTitle);
#else
	BOOL bIsWow64 = FALSE;
   if (!IsWow64Process(GetCurrentProcess(), &bIsWow64)) {
      chFAIL("Failed to get WOW64 state.");
      return;
   }
   
   if (bIsWow64) {
      StringCchPrintf(szFullTitle, _countof(szFullTitle), 
         TEXT("32-bit %s on WOW64"), szTitle);
   } else {
      StringCchPrintf(szFullTitle, _countof(szFullTitle), 
         TEXT("32-bit %s on 32-bit Windows"), szTitle);
   }
#endif

   SetWindowText(hWnd, szFullTitle);
}



///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   chSETDLGICONS(hWnd, IDI_SYSINFO);
   
   /* wProcessorArchitecture
         The processor architecture of the installed operating system. 
		 This member can be one of the following values.

         Value	                                   Meaning

       PROCESSOR_ARCHITECTURE_AMD64          x64 (AMD or Intel)
       9

       PROCESSOR_ARCHITECTURE_ARM            ARM
       Reserved

       PROCESSOR_ARCHITECTURE_IA64           Intel Itanium-based 
       6

       PROCESSOR_ARCHITECTURE_INTEL          x86
       0

       PROCESSOR_ARCHITECTURE_UNKNOWN        Unknown architecture.
	   0xffff

	   wProcessorLevel
          The architecture-dependent processor level. 

		  It should be used only for display purposes. 
		  To determine the feature set of a processor, use the IsProcessorFeaturePresent function.

          If wProcessorArchitecture is PROCESSOR_ARCHITECTURE_INTEL, 
		  wProcessorLevel is defined by the CPU vendor.

          If wProcessorArchitecture is PROCESSOR_ARCHITECTURE_IA64, 
		  wProcessorLevel is set to 1.
       
	   wProcessorRevision
          The architecture-dependent processor revision. 
		  The following table shows how the revision value 
		  is assembled for each type of processor architecture.

          Processor	                                  Value

          Intel Pentium, 
		  Cyrix, 
		  or NextGen 586	                  The high byte is the model and the low byte is the stepping. 
		                                      For example, if the value is xxyy, 
											  the model number and stepping can be displayed as follows:
                                              Model xx, Stepping yy

          Intel 80386 or 80486	              A value of the form xxyz.
                                              If xx is equal to 0xFF, y - 0xA is the model number, 
											  and z is the stepping identifier.
                                              If xx is not equal to 0xFF, xx + 'A' is the stepping letter and yz is the minor stepping.
                                              ARM	Reserved.

    * 
    */
   SYSTEM_INFO sinf;
   /* To retrieve accurate information for an application 
    * running on WOW64, call the GetNativeSystemInfo function. */
   GetSystemInfo(&sinf);

   ShowCPUInfo(hWnd, sinf.wProcessorArchitecture,
      sinf.wProcessorLevel, sinf.wProcessorRevision);

   TCHAR szBuf[50];
   SetDlgItemText(hWnd, IDC_PAGESIZE,
      BigNumToString(sinf.dwPageSize, szBuf, _countof(szBuf)));

   StringCchPrintf(szBuf, _countof(szBuf), TEXT("%p"), 
      sinf.lpMinimumApplicationAddress);
   SetDlgItemText(hWnd, IDC_MINAPPADDR, szBuf);

   StringCchPrintf(szBuf, _countof(szBuf), TEXT("%p"), 
      sinf.lpMaximumApplicationAddress);
   SetDlgItemText(hWnd, IDC_MAXAPPADDR, szBuf);

   StringCchPrintf(szBuf, _countof(szBuf), TEXT("0x%016I64X"), 
      (__int64) sinf.dwActiveProcessorMask);
   SetDlgItemText(hWnd, IDC_ACTIVEPROCMASK, szBuf);

   SetDlgItemText(hWnd, IDC_NUMOFPROCS,
      BigNumToString(sinf.dwNumberOfProcessors, szBuf, _countof(szBuf)));

   SetDlgItemText(hWnd, IDC_ALLOCGRAN,
      BigNumToString(sinf.dwAllocationGranularity, szBuf, _countof(szBuf)));

   ShowBitness(hWnd);

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hWnd, id);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

   switch (uMsg) {
      chHANDLE_DLGMSG(hDlg, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hDlg, WM_COMMAND,    Dlg_OnCommand);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR, int) {

   DialogBox(hInstExe, MAKEINTRESOURCE(IDD_SYSINFO), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
