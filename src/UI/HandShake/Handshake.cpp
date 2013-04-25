
#include "CmnHdr.h"  
#include <windowsx.h>
#include <tchar.h>
#include "Resource.h"


/* This event is signaled when the client has a request for the server */
HANDLE g_hevtRequestSubmitted;

/* This event is signaled when the server has a result for the client */
HANDLE g_hevtResultReturned;

/* The buffer shared between the client and server threads */
TCHAR  g_szSharedRequestAndResultBuffer[1024];

/* The special value sent from the client that causes the 
 * server thread to terminate cleanly. */
TCHAR  g_szServerShutdown[] = TEXT("Server Shutdown");

/* The server thread will check that the main dialog is no longer alive
 * when the shutdown message is received.
 */
HWND   g_hMainDlg;

/* This is the code executed by the server thread */
DWORD WINAPI ServerThread(PVOID pvParam) 
{
   /* Assume that the server thread is to run forever */
   BOOL fShutdown = FALSE;

   while (!fShutdown) 
   {
      /* Wait for the client to submit a request.
	   * g_hevtRequestSubmitted is not signalled from the start
	   * And here we wait for client to set this event on after 
	   * posting a request
	   */
      WaitForSingleObject(g_hevtRequestSubmitted, INFINITE);

      /* Check to see if the client wants the server to terminate */
      fShutdown = 
         (g_hMainDlg == NULL) &&
         (_tcscmp(g_szSharedRequestAndResultBuffer, g_szServerShutdown) == 0);

      if (!fShutdown) 
	  {
         /* Process the client's request (reverse the string) */
         _tcsrev(g_szSharedRequestAndResultBuffer);
      }

      /* Let the client process the request's result 
	   * A client thread can wait for the server to finish 
	   * processing the request. After finishing it the server
	   * signals the event
	   */
      SetEvent(g_hevtResultReturned);
   }

   return(0);
}

BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) 
{
   chSETDLGICONS(hwnd, IDI_HANDSHAKE);

   /* Initialize the edit control with some test data request */
   Edit_SetText(GetDlgItem(hwnd, IDC_REQUEST), TEXT("Some test data"));

   /* Store the main dialog window handle */
   g_hMainDlg = hwnd;

   return(TRUE);
}

void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) 
{
   switch (id) 
   {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;

      /* Submit a request to the server thread */
      case IDC_SUBMIT:   
         /* Copy the request string into the shared data buffer */
         Edit_GetText(GetDlgItem(hwnd, IDC_REQUEST), 
                      g_szSharedRequestAndResultBuffer, 
                      _countof(g_szSharedRequestAndResultBuffer));

         /* Let the server thread know that a request is ready in the buffer
          * Wait for the server to process the request and give us the result
		  * Signals one object and waits on another object as a single operation.
		  *
		  * hObjectToSignal [in] 
          *   A handle to the object to be signaled. 
		  *   This object can be a semaphore, a mutex, or an event.
          *   If the handle is a semaphore, the SEMAPHORE_MODIFY_STATE access right is required. 
		  *   If the handle is an event, the EVENT_MODIFY_STATE access right is required. 
		  *   If the handle is a mutex and the caller does not own the mutex, 
		  *   the function fails with ERROR_NOT_OWNER. 
          *  
		  * hObjectToWaitOn [in] 
          *   A handle to the object to wait on. 
		  *   The SYNCHRONIZE access right is required; for more information, 
		  *   see Synchronization Object Security and Access Rights. 
		  *   For a list of the object types whose handles you can specify, see the Remarks section.
          * 
		  * Firstly we signal RequestSubmitted event. which indicates to the server thread
		  * that a new request has been committed and we need to process it. After that the client
		  * thread is put into suspended state and waits until the server thread process the request
		  * After processing a request the server thread signals g_hevtResultReturned event.
		  */  
         SignalObjectAndWait(
			 g_hevtRequestSubmitted, 
			 g_hevtResultReturned, 
			 INFINITE, 
			 false);

         /* Let the user know the result */
         Edit_SetText(
			 GetDlgItem(hwnd, IDC_RESULT), 
             g_szSharedRequestAndResultBuffer);

         break;
   }
}

INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{   
   switch (uMsg) 
   {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
   }

   return(FALSE);
}

int WINAPI _tWinMain(HINSTANCE hInstanceExe, HINSTANCE, PTSTR, int) 
{
   /* Create & initialize the 2 nonsignaled, auto-reset events */
   g_hevtRequestSubmitted = CreateEvent(NULL, FALSE, FALSE, NULL);
   g_hevtResultReturned   = CreateEvent(NULL, FALSE, FALSE, NULL);

   /* Spawn the server thread */
   DWORD dwThreadID;
   /* Server thread will be waiting for at least one request to process */
   HANDLE hThreadServer = chBEGINTHREADEX(NULL, 0, ServerThread, NULL, 0, &dwThreadID);

   /* Execute the client thread's user interface */
   DialogBox(hInstanceExe, MAKEINTRESOURCE(IDD_HANDSHAKE), NULL, Dlg_Proc);

   g_hMainDlg = NULL;

   /* The client's UI is closing, have the server thread shutdown */
   _tcscpy_s(g_szSharedRequestAndResultBuffer, 
             _countof(g_szSharedRequestAndResultBuffer), 
			 g_szServerShutdown);

   SetEvent(g_hevtRequestSubmitted);

   /* Wait for the server thread to acknowledge the shutdown AND
    * wait for the server thread to fully terminate
	*/
   HANDLE h[2];
   /* Wait for the server thread to process the request
    * and signal the event
    */
   h[0] = g_hevtResultReturned;
   /* Wait the server thread to finish its work completely */
   h[1] = hThreadServer;

   WaitForMultipleObjects(2, h, TRUE, INFINITE);
   
   /* Properly clean up everything */
   CloseHandle(hThreadServer);      
   CloseHandle(g_hevtRequestSubmitted);      
   CloseHandle(g_hevtResultReturned);      

   /* The client thread terminates with the whole process */
   return(0);
}

void TimerTest()
{
	HANDLE hTimer;
	SYSTEMTIME st;
	FILETIME ftLocal, ftUTC;
	LARGE_INTEGER liUTC;

	/* bManualReset [in] 
     *  If this parameter is TRUE, the timer is a manual-reset notification timer. 
	 *  Otherwise, the timer is a synchronization timer.
     */
    hTimer = CreateWaitableTimer(NULL, FALSE, NULL);
    /*  */
}
