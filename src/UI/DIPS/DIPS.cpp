/******************************************************************************
Module:  DIPS.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "CmnHdr.h"     /* See Appendix A. */
#include <WindowsX.h>
#include <tchar.h>
#include "Resource.h"
#include "..\DIPSLib\DIPSLib.h"


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam) {

   chSETDLGICONS(hWnd, IDI_DIPS);
   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hWnd, int id, HWND hWndCtl, UINT codeNotify) {
   
   switch (id) {
      case IDC_SAVE:
      case IDC_RESTORE:
      case IDCANCEL:
         EndDialog(hWnd, id);
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

   // Convert command-line character to uppercase.
   CharUpperBuff(pszCmdLine, 1);
   TCHAR cWhatToDo = pszCmdLine[0];

   if ((cWhatToDo != TEXT('S')) && (cWhatToDo != TEXT('R'))) 
   {
      // An invalid command-line argument; prompt the user.
      cWhatToDo = 0;
   }

   if (cWhatToDo == 0) 
   {
      // No command-line argument was used to tell us what to
      // do; show usage dialog box and prompt the user.

      /* Creates a modal dialog box from a dialog box template resource. 
	   * DialogBox does not return control 
	   * until the specified callback function terminates 
	   * the modal dialog box by calling the EndDialog function. 
	   *
	   * hInstance [in, optional]
       *   Type: HINSTANCE
       *   A handle to the module whose executable file 
	   *   contains the dialog box template.
	   */
      switch (DialogBox(hInstExe, MAKEINTRESOURCE(IDD_DIPS), NULL, Dlg_Proc)) 
	  {
         case IDC_SAVE:    
            cWhatToDo = TEXT('S'); 
            break;

         case IDC_RESTORE: 
            cWhatToDo = TEXT('R');
            break;
      }
   }

   if (cWhatToDo == 0) 
   {
      // The user doesn't want to do anything.
      return(0);
   }
   
   // The Desktop ListView window is the grandchild of the ProgMan window.
   /* 
    * Retrieves a handle to the top-level window 
	* whose class name and window name match the specified strings. 
	* This function does not search child windows. 
	* This function does not perform a case-sensitive search.
    * To search child windows, beginning with a specified child window, 
	* use the FindWindowEx function.
	* 
	* --get desktop HWND 
    */
   HWND hWndLV = GetFirstChild(GetFirstChild(
      FindWindow(TEXT("ProgMan"), NULL)));
   chASSERT(IsWindow(hWndLV));

   // Set hook that injects our DLL into the Explorer's address space. After 
   // setting the hook, the DIPS hidden modeless dialog box is created. We 
   // send messages to this window to tell it what we want it to do.
   chVERIFY(SetDIPSHook(GetWindowThreadProcessId(hWndLV, NULL)));

   // Wait for the DIPS server window to be created.
   MSG msg;
   /* 
    * Retrieves a message from the calling thread's message queue. 
	* The function dispatches incoming sent messages 
	* until a posted message is available for retrieval.
    * Unlike GetMessage, the PeekMessage function 
	* does not wait for a message to be posted before returning.
    * 
	* lpMsg [out]
    *   Type: LPMSG
    *   A pointer to an MSG structure 
	*   that receives message information 
	*   from the thread's message queue.
	* 
	* hWnd [in, optional]
    *   Type: HWND
    *   A handle to the window whose messages are to be retrieved. 
	*   The window must belong to the current thread.
	*   If hWnd is NULL, GetMessage retrieves messages 
	*   for any window that belongs to the current thread, 
	*   and any messages on the current thread's message queue 
	*   whose hwnd value is NULL (see the MSG structure). 
	*   Therefore if hWnd is NULL, both window messages and thread messages are processed.
    */
   GetMessage(&msg, NULL, 0, 0);

   // Find the handle of the hidden dialog box window.
   HWND hWndDIPS = FindWindow(NULL, TEXT("Wintellect DIPS"));

   // Make sure that the window was created.
   chASSERT(IsWindow(hWndDIPS));

   // Tell the DIPS window which ListView window to manipulate
   // and whether the items should be saved or restored.
   BOOL bSave = (cWhatToDo == TEXT('S'));
   /* 
    * Sends the specified message to a window or windows. 
	* The SendMessage function calls the window procedure 
	* for the specified window and does not return 
	* until the window procedure has processed the message.
	* 
	* 
    */
   SendMessage(hWndDIPS, WM_APP, (WPARAM) hWndLV, bSave);

   // Tell the DIPS window to destroy itself. Use SendMessage 
   // instead of PostMessage so that we know the window is 
   // destroyed before the hook is removed.
   SendMessage(hWndDIPS, WM_CLOSE, 0, 0);

   // Make sure that the window was destroyed.
   chASSERT(!IsWindow(hWndDIPS));

   // Unhook the DLL, removing the DIPS dialog box procedure 
   // from the Explorer's address space.
   SetDIPSHook(0);  

   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
