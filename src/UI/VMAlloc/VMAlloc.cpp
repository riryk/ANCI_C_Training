/******************************************************************************
Module:  VMAlloc.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "CmnHdr.h"     /* See Appendix A. */
#include <WindowsX.h>
#include <tchar.h>
#include "Resource.h"
#include <StrSafe.h>


///////////////////////////////////////////////////////////////////////////////


// The number of bytes in a page on this host machine.
UINT g_uPageSize = 0;


// A dummy data structure used for the array.
typedef struct {
   BOOL bInUse;
   BYTE bOtherData[2048 - sizeof(BOOL)];
} SOMEDATA, *PSOMEDATA;


// The number of structures in the array
#define MAX_SOMEDATA    (50)

// Pointer to an array of data structures
PSOMEDATA g_pSomeData = NULL;

// The rectangular area in the window occupied by the memory map
RECT g_rcMemMap;


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {
   
   chSETDLGICONS(hWnd, IDI_VMALLOC);

   // Initialize the dialog box by disabling all the nonsetup controls.
   EnableWindow(GetDlgItem(hWnd, IDC_INDEXTEXT),      FALSE);
   EnableWindow(GetDlgItem(hWnd, IDC_INDEX),          FALSE);
   EnableWindow(GetDlgItem(hWnd, IDC_USE),            FALSE);
   EnableWindow(GetDlgItem(hWnd, IDC_CLEAR),          FALSE);
   EnableWindow(GetDlgItem(hWnd, IDC_GARBAGECOLLECT), FALSE);

   // Get the coordinates of the memory map display.
   GetWindowRect(GetDlgItem(hWnd, IDC_MEMMAP), &g_rcMemMap);
   MapWindowPoints(NULL, hWnd, (LPPOINT) &g_rcMemMap, 2);

   // Destroy the window that identifies the location of the memory map
   DestroyWindow(GetDlgItem(hWnd, IDC_MEMMAP));
   
   // Put the page size in the dialog box just for the user's information.
   TCHAR szBuf[10];
   StringCchPrintf(szBuf, _countof(szBuf), TEXT("%d KB"), g_uPageSize / 1024);
   SetDlgItemText(hWnd, IDC_PAGESIZE, szBuf);

   // Initialize the edit control.
   SetDlgItemInt(hWnd, IDC_INDEX, 0, FALSE);
   
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnDestroy(HWND hWnd) {

   if (g_pSomeData != NULL)
      VirtualFree(g_pSomeData, 0, MEM_RELEASE);
}


///////////////////////////////////////////////////////////////////////////////


VOID GarbageCollect(PVOID pvBase, DWORD dwNum, DWORD dwStructSize) {

   UINT uMaxPages = dwNum * dwStructSize / g_uPageSize;
   for (UINT uPage = 0; uPage < uMaxPages; uPage++) 
   {
      BOOL bAnyAllocsInThisPage = FALSE;
      UINT uIndex     = uPage  * g_uPageSize / dwStructSize;
      UINT uIndexLast = uIndex + g_uPageSize / dwStructSize;

      for (; uIndex < uIndexLast; uIndex++) 
	  {
         MEMORY_BASIC_INFORMATION mbi;
         VirtualQuery(&g_pSomeData[uIndex], &mbi, sizeof(mbi));

         bAnyAllocsInThisPage = 
			  (mbi.State == MEM_COMMIT) && 
            * (PBOOL) ((PBYTE) pvBase + dwStructSize * uIndex);

         // Stop checking this page, we know we can't decommit it.
         if (bAnyAllocsInThisPage) 
			 break;
      }

      if (!bAnyAllocsInThisPage) 
	  {
         // No allocated structures in this page; decommit it.
         VirtualFree(&g_pSomeData[uIndexLast - 1], dwStructSize, MEM_DECOMMIT);
      }
   }
}


///////////////////////////////////////////////////////////////////////////////

         
void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {

   UINT uIndex = 0;

   switch (id) {
      case IDCANCEL:
         EndDialog(hWnd, id);
         break;

      case IDC_RESERVE:
         // Reserve enough address space to hold the array of structures.
         g_pSomeData = (PSOMEDATA) VirtualAlloc(NULL, 
            MAX_SOMEDATA * sizeof(SOMEDATA), MEM_RESERVE, PAGE_READWRITE);

         // Disable the Reserve button and enable all the other controls.
         EnableWindow(GetDlgItem(hWnd, IDC_RESERVE),        FALSE);
         EnableWindow(GetDlgItem(hWnd, IDC_INDEXTEXT),      TRUE);
         EnableWindow(GetDlgItem(hWnd, IDC_INDEX),          TRUE);
         EnableWindow(GetDlgItem(hWnd, IDC_USE),            TRUE);
         EnableWindow(GetDlgItem(hWnd, IDC_GARBAGECOLLECT), TRUE);

         // Force the index edit control to have the focus.
         SetFocus(GetDlgItem(hWnd, IDC_INDEX));
         
         // Force the memory map to update
         InvalidateRect(hWnd, &g_rcMemMap, FALSE);
         break;

      case IDC_INDEX:
         if (codeNotify != EN_CHANGE)
            break;

         uIndex = GetDlgItemInt(hWnd, id, NULL, FALSE);

         if ((g_pSomeData != NULL) && chINRANGE(0, uIndex, MAX_SOMEDATA - 1)) 
		 {
            MEMORY_BASIC_INFORMATION mbi;
			/* Retrieves information about a range of pages 
			 * in the virtual address space of the calling process.
             * To retrieve information about a range of pages 
			 * in the address space of another process, use the VirtualQueryEx function.
			 *
			 * lpAddress [in, optional]
             *    A pointer to the base address of the region of pages to be queried. 
			 *    This value is rounded down to the next page boundary. 
			 *    To determine the size of a page on the host computer, use the GetSystemInfo function.
             *    If lpAddress specifies an address above the highest memory address accessible to the process, 
			 *    the function fails with ERROR_INVALID_PARAMETER.
			 * 
			 * lpBuffer [out]
             *    A pointer to a MEMORY_BASIC_INFORMATION structure 
			 *    in which information about the specified page range is returned. 
			 */
            VirtualQuery(&g_pSomeData[uIndex], &mbi, sizeof(mbi));
			/* State
             *    The state of the pages in the region. 
			 *    This member can be one of the following values.
			 * MEM_COMMIT
             *    0x1000
             *    Indicates committed pages for which physical storage has been allocated, 
			 *    either in memory or in the paging file on disk.
			 * 
			 */
            BOOL bOk = (mbi.State == MEM_COMMIT);
            if (bOk)
               bOk = g_pSomeData[uIndex].bInUse;

            EnableWindow(GetDlgItem(hWnd, IDC_USE),  !bOk);
            EnableWindow(GetDlgItem(hWnd, IDC_CLEAR), bOk);

         } 
		 else 
		 {
            EnableWindow(GetDlgItem(hWnd, IDC_USE),   FALSE);
            EnableWindow(GetDlgItem(hWnd, IDC_CLEAR), FALSE);
         }

         break;
         

      case IDC_USE:
         uIndex = GetDlgItemInt(hWnd, IDC_INDEX, NULL, FALSE);
         // NOTE: New pages are always zeroed by the system
		 /* Commit memory allocated for g_pSomeData[uIndex] */
         VirtualAlloc(
			 &g_pSomeData[uIndex], 
			 sizeof(SOMEDATA), 
             MEM_COMMIT, 
			 PAGE_READWRITE);

		 /* Mark this part of array as used */
         g_pSomeData[uIndex].bInUse = TRUE;

         EnableWindow(GetDlgItem(hWnd, IDC_USE),   FALSE);
         EnableWindow(GetDlgItem(hWnd, IDC_CLEAR), TRUE);

         // Force the Clear button control to have the focus.
         SetFocus(GetDlgItem(hWnd, IDC_CLEAR));
      
         // Force the memory map to update
         InvalidateRect(hWnd, &g_rcMemMap, FALSE);
         break;

      case IDC_CLEAR:
         uIndex = GetDlgItemInt(hWnd, IDC_INDEX, NULL, FALSE);
         g_pSomeData[uIndex].bInUse = FALSE;
         EnableWindow(GetDlgItem(hWnd, IDC_USE),   TRUE);
         EnableWindow(GetDlgItem(hWnd, IDC_CLEAR), FALSE);

         // Force the Use button control to have the focus.
         SetFocus(GetDlgItem(hWnd, IDC_USE));
         break;

      case IDC_GARBAGECOLLECT:
         GarbageCollect(g_pSomeData, MAX_SOMEDATA, sizeof(SOMEDATA));

         // Force the memory map to update
         InvalidateRect(hWnd, &g_rcMemMap, FALSE);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnPaint(HWND hWnd) {    // Update the memory map

   PAINTSTRUCT ps;
   BeginPaint(hWnd, &ps);

   UINT uMaxPages = MAX_SOMEDATA * sizeof(SOMEDATA) / g_uPageSize;
   UINT uMemMapWidth = g_rcMemMap.right - g_rcMemMap.left;
   
   if (g_pSomeData == NULL) {

      // The memory has yet to be reserved.
      Rectangle(ps.hdc, g_rcMemMap.left, g_rcMemMap.top,
         g_rcMemMap.right - uMemMapWidth % uMaxPages, g_rcMemMap.bottom);

   } else {

      // Walk the virtual address space, painting the memory map
      for (UINT uPage = 0; uPage < uMaxPages; uPage++) {

         UINT uIndex = uPage * g_uPageSize / sizeof(SOMEDATA);
         UINT uIndexLast = uIndex + g_uPageSize / sizeof(SOMEDATA);
         for (; uIndex < uIndexLast; uIndex++) {

            MEMORY_BASIC_INFORMATION mbi;
            VirtualQuery(&g_pSomeData[uIndex], &mbi, sizeof(mbi));

            int nBrush = 0;
            switch (mbi.State) {
               case MEM_FREE:    nBrush = WHITE_BRUSH; break;
               case MEM_RESERVE: nBrush = GRAY_BRUSH;  break;
               case MEM_COMMIT:  nBrush = BLACK_BRUSH; break;
            }

            SelectObject(ps.hdc, GetStockObject(nBrush));
            Rectangle(ps.hdc,
               g_rcMemMap.left + uMemMapWidth / uMaxPages * uPage,
               g_rcMemMap.top,
               g_rcMemMap.left + uMemMapWidth / uMaxPages * (uPage + 1),
               g_rcMemMap.bottom);
         }
      }
   }

   EndPaint(hWnd, &ps);
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
   
   switch (uMsg) {
      chHANDLE_DLGMSG(hWnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hWnd, WM_COMMAND,    Dlg_OnCommand);
      chHANDLE_DLGMSG(hWnd, WM_PAINT,      Dlg_OnPaint);
      chHANDLE_DLGMSG(hWnd, WM_DESTROY,    Dlg_OnDestroy);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hInstExe, HINSTANCE, PTSTR, int) {

   // Get the page size used on this CPU.
   SYSTEM_INFO si;
   /* 
    * Retrieves information about the current system.
    * To retrieve accurate information for an application running on WOW64, 
	* call the GetNativeSystemInfo function.
    */
   GetSystemInfo(&si);
   g_uPageSize = si.dwPageSize;

   DialogBox(hInstExe, MAKEINTRESOURCE(IDD_VMALLOC), NULL, Dlg_Proc);
   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
