/* Include this header file once per compilation unit */
#pragma once  

#include <Windowsx.h>
#include <tchar.h>
#include "Resource.h"

#pragma warning(push, 3)
#include <Windows.h>
#pragma warning(pop) 
#pragma warning(push, 4)
#include <CommCtrl.h>
/* For _beginthreadex */
#include <process.h>  


#define ESM_POKECODEANDLOOKUP (WM_USER + 100)
const TCHAR g_szAppName[] = TEXT("Error Show");

/* Initialize dialog */
BOOL Dialog_OnInitDialog(HWND hWnd, HWND hwndFocus, LPARAM lParam) 
{
	LONG_PTR windowLongPtr;
	HICON errorIcon;
	HWND dlgItem;
	/* Retrieve information about the specific  */
	windowLongPtr = GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
	/* Loads the specified icon resource from the executable (.exe)
	 * file associated with an application instance */
    errorIcon = LoadIcon((HINSTANCE)windowLongPtr, MAKEINTRESOURCE(IDI_ERRORSHOW));
    /* Sends the specified message to a window or windows. 
	 * The SendMessage function calls the window procedure for the specified window 
	 * and does not return until the window procedure has processed the message 
	 * HWND hWnd - a handle to the window whose window procedure will receive the message
	 * UINT Msg - The message to be sent.
	 * WPARAM wParam - Additional message-specific information
	 * LPARAM lParam - Additional message-specific information
	 */
    SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)errorIcon);
    /* Send the same but only a big icon */
    SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)errorIcon);
    /* Retrieves a handle to a control in the specified dialog box.  */
	dlgItem = GetDlgItem(hWnd, IDC_ERRORCODE);
	/* Limits the length of text that can be entered into an edit control.
	 * You can use this macro or send the EM_LIMITTEXT message explicitly. 
	 */
    Edit_LimitText(dlgItem, 5);
    /* Look up the command-line passed error number */
    SendMessage(hWnd, ESM_POKECODEANDLOOKUP, lParam, 0);
    /* When everything is ok - return true to the user */
    return(TRUE);
}


void Dialog_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) 
{
   switch (id) {

   case IDCANCEL:
      EndDialog(hwnd, id);
      break;

   case IDC_ALWAYSONTOP:
      SetWindowPos(hwnd, IsDlgButtonChecked(hwnd, IDC_ALWAYSONTOP) 
         ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
      break;

   case IDC_ERRORCODE: 
      HWND dlgItem;
	  int textLength;
	  /* Retrieves a handle to a OK button control in the specified dialog box.  */
      dlgItem = GetDlgItem(hwnd, IDOK);
      /* Get text length of control*/
      textLength = Edit_GetTextLength(hwndCtl);
      /* Enable or disable a control */
      EnableWindow(dlgItem, textLength > 0);
      break;

   case IDOK:
	  /* Translates the text of a specified control in a dialog box into an integer value. 
	   * Get the error code.
	   * hDlg HWND - a handle to the dialog box that contains the control of interest.  
       * nIDDlgItem int - The identifier of the control whose text is to be translated. 
	   */
      DWORD dwError = GetDlgItemInt(hwnd, IDC_ERRORCODE, NULL, FALSE);
      /* Buffer that gets the error message string */
      HLOCAL hlocal = NULL; 
      /* Use the default system locale since we look for Windows messages.
       * Note: this MAKELANGID combination has 0 as value
	   */
      DWORD systemLocale = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
      /* Get the error code's textual description */
      BOOL fOk = FormatMessage(
           FORMAT_MESSAGE_FROM_SYSTEM 
		 | FORMAT_MESSAGE_IGNORE_INSERTS 
		 | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
         NULL, 
		 dwError, 
		 systemLocale, 
         (PTSTR)&hlocal, 
		 0, 
		 NULL);
      /* If there was an exception  */
      if (!fOk) 
	  {
         /* Is it a network-related error? */
         HMODULE hDll = LoadLibraryEx(
			 TEXT("netmsg.dll"), 
			 NULL, 
             DONT_RESOLVE_DLL_REFERENCES);

         if (hDll != NULL) 
		 {
            fOk = FormatMessage(
                 FORMAT_MESSAGE_FROM_HMODULE 
			   | FORMAT_MESSAGE_IGNORE_INSERTS 
			   | FORMAT_MESSAGE_ALLOCATE_BUFFER,
               hDll, 
			   dwError, 
			   systemLocale,
               (PTSTR)&hlocal, 
			   0, 
			   NULL);

            FreeLibrary(hDll);
         }
      }

      if (fOk && (hlocal != NULL)) 
	  {
         SetDlgItemText(hwnd, IDC_ERRORTEXT, (PCTSTR) LocalLock(hlocal));
         LocalFree(hlocal);
      } 
	  else 
	  {
         SetDlgItemText(
			 hwnd, 
			 IDC_ERRORTEXT, 
             TEXT("No text found for this error number."));
      }

      break;
   }
}


/* The normal HANDLE_MSG macro in WindowsX.h does not work properly for dialog
 * boxes because DlgProc returns a BOOL instead of an LRESULT (like
 * WndProcs). This chHANDLE_DLGMSG macro corrects the problem:
 * A possible way to do it without macro
 * UINT uMsg
 * switch (uMsg)
 * {
 *    case WM_INITDIALOG:
 *    {
 *        return SetDlgMsgResult(
 *                    hwndDlg, 
 *                    uMsg,
 *                    HANDLE_WM_INITDIALOG(hwndDlg, wParam, lParam, Dlg_OnInitDialog));
 *    }
 *    case WM_COMMAND:
 *    {
 *        return SetDlgMsgResult(
 *                    hwndDlg, 
 *                    uMsg,
 *                    HANDLE_WM_COMMAND(hwndDlg, wParam, lParam, Dlg_OnCommand));
 *    }
 *    case ESM_POKECODEANDLOOKUP:
 *    {
 *      ....
 *    }
 * }
 * After using this macro we can make our code shorter
 *  switch (uMsg) 
 *  {
 *     chHANDLE_DLGMSG(hwndDlg, WM_INITDIALOG, Dlg_OnInitDialog);
 *     chHANDLE_DLGMSG(hwndDlg, WM_COMMAND,    Dlg_OnCommand);
 *
 *     case ESM_POKECODEANDLOOKUP:
 *     {
 *       ....
 *     }
 *  }
 */
#define chHANDLE_DLGMSG(hWnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hWnd, uMsg,     \
      HANDLE_##message((hWnd), (wParam), (lParam), (fn))))

/* The callback function for a dialog box show.
 * This function has a specific format
 * 1. HWND hwndDlg - a handle to a dialog box
 * 2. UINT uMsg - a message to show
 * 3. WPARAM wParam - an additional message-specific information 
 * 4. LPARAM lParam - additional message-specific information  
 */
INT_PTR WINAPI DialogProcedure(
	HWND hwndDlg, 
	UINT uMsg, 
	WPARAM wParam, 
	LPARAM lParam) 
{

	LPARAM lParam1;

   switch (uMsg) 
   {
	   /* Set up init dialog calback to a specific function Dialog_OnInitDialog */
       chHANDLE_DLGMSG(hwndDlg, WM_INITDIALOG, Dialog_OnInitDialog);
       /* Set up init dialog calback to a specific function Dialog_OnInitDialog */
       chHANDLE_DLGMSG(hwndDlg, WM_COMMAND, Dialog_OnCommand);

       case ESM_POKECODEANDLOOKUP:
          /* Sets the text of a control in a dialog box to the string representation of a specified integer value.
		   * HWND hDlg - A handle to the dialog box that contains the control.
		   * int nIDDlgItem - The control to be changed.
		   * UINT uValue - The integer value used to generate the item text. 
		   * BOOL bSigned - Indicates whether the uValue parameter is signed or unsigned.
		   */
          SetDlgItemInt(hwndDlg, IDC_ERRORCODE, (UINT)wParam, FALSE);
          FORWARD_WM_COMMAND(hwndDlg, IDOK, GetDlgItem(hwndDlg, IDOK), BN_CLICKED, PostMessage);
          SetForegroundWindow(hwndDlg);
          break;
   }

   return(FALSE);
}

/* This is a main function to start WinApi process.
 * _tWinMain is an application entry point.
 * Parameters description:
 * HINSTANCE hinstExe - a handle to the current instance of the application
 * HINSTANCE hPrevInstance - a previous instance to this application. This is always null
 * PTSTR lpCmdLine - the command line for the application. If, for example, you need to 
 *                   pass some parameters 
 * int nCmdShow - controls how the window is to be show. It is not a required parameter.
 */
int WINAPI _tWinMain(
		HINSTANCE hInstance, 
		HINSTANCE hPrevInstance, 
		PTSTR lpCmdLine, 
		int nCmdShow) 
{
   /* Try to find a top level window by a class name and a window name */
   HWND hwnd = FindWindow(TEXT("#32770"), TEXT("Error Show"));
   /* Check if our handler points to an existing window. If the window does not
    * exist, we launch an dialog box.
    */
   if (IsWindow(hwnd)) 
   {
      // An instance is already running, activate it and send it the new #
      SendMessage(hwnd, ESM_POKECODEANDLOOKUP, _ttoi(lpCmdLine), 0);
   } 
   /* Launch a dialog box */
   else 
   {
	   /* This function launches a dialog box
	    * Parameters description:
		* 1. HINSTANCE hInstance - a handle to the current application instance
		* 2. LPCTSTR lpTemplateName - is the dialog box template. In our case we show an error
		* 3. HWND hwndParent - a handle to a parent window, which owns this dialog box. 
		*    In our case it is NULL.
		* 4. DLGPROC lpDialogFunc - a dialog procedure, which is executed before 
		*    showing an error message.
		* 5. LPARAM dwInitParam - some parameters for the dialog
	    */
       DialogBoxParam
       (
		   hInstance, 
		   MAKEINTRESOURCE(IDD_ERRORSHOW), 
           NULL, 
		   DialogProcedure, 
		   _ttoi(lpCmdLine)
	   );
   }

   return(0);
}

