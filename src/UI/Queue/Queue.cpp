

#include "CmnHdr.h"    
#include <windowsx.h>
#include <tchar.h>
#include <StrSafe.h>
#include "Resource.h"


class CQueue 
{
public:
   struct ELEMENT 
   {
      /* Number of thread where this element has been created */
      int m_nThreadNum;
	  /* Request number by which the element has been supplied */
	  int m_nRequestNum;
   };
   typedef ELEMENT* PELEMENT;

private:
   /* Array of elements to be processed */
   PELEMENT m_pElements;     
   /* Maximum number of elements in the array */
   int      m_nMaxElements;   
   /* Mutex & semaphore handles */
   HANDLE   m_h[2];             
   /* Reference to m_h[0]. This is a reference to the mutex */
   HANDLE   &m_hmtxQ;      
   /* Reference to m_h[1]. This is a reference to the semafore  */
   HANDLE   &m_hsemNumElements;

public:
   CQueue(int nMaxElements);
   ~CQueue();

   BOOL Append(PELEMENT pElement, DWORD dwMilliseconds);
   BOOL Remove(PELEMENT pElement, DWORD dwMilliseconds);
};

CQueue::CQueue(int nMaxElements) 
   : m_hmtxQ(m_h[0]), m_hsemNumElements(m_h[1]) 
{
    /* Allocate memory for the inward array of elements. 
	 * We allocate the memory from the current process heap */
    m_pElements = 
	   (PELEMENT)HeapAlloc(
	      GetProcessHeap(), 
		  0, 
		  sizeof(ELEMENT) * nMaxElements);
    
	/* Set the max number of elements */
    m_nMaxElements = nMaxElements;
	/* Create a mutex for syncronization 
	 * The second parameter:
	 * bInitialOwner [in] 
     *   If this value is TRUE and the caller created the mutex, 
	 *   the calling thread obtains initial ownership of the mutex object. 
	 *   Otherwise, the calling thread does not obtain ownership of the mutex. 
	 *   To determine if the caller created the mutex, see the Return Values section.
     */
    m_hmtxQ = CreateMutex(NULL, FALSE, NULL);
	/* Create a semafore for syncronization. The semafore 
	 * can count the number of simultaneous threads.
	 *
	 * lInitialCount [in] 
     *   The initial count for the semaphore object. 
	 *   This value must be greater than or equal to zero 
	 *   and less than or equal to lMaximumCount. 
	 *   The state of a semaphore is signaled when its count 
	 *   is greater than zero and nonsignaled when it is zero. 
	 *   The count is decreased by one whenever a wait function releases a thread 
	 *   that was waiting for the semaphore. 
	 *   The count is increased by a specified amount by calling the ReleaseSemaphore function.
	 *
	 * lMaximumCount [in] 
     *   The maximum count for the semaphore object. 
	 *   This value must be greater than zero.
     *
	 */
    m_hsemNumElements = CreateSemaphore(NULL, 0, nMaxElements, NULL);
}

CQueue::~CQueue() 
{
	/* In destructor we close handlers to the mutex and semafore
	 * and free the memory that has been allocated for the elements 
	 * array in constructor */
    CloseHandle(m_hsemNumElements);
    CloseHandle(m_hmtxQ);
    HeapFree(GetProcessHeap(), 0, m_pElements);
}

BOOL CQueue::Append(PELEMENT pElement, DWORD dwTimeout) 
{
   BOOL fOk = FALSE;
   /* When mutex is non-signalled that means that there is another 
    * thread which owns this mutex and we must wait until it finishes its work.
	* From the start we do not have any other threads, so we will not wait.
	* Mutex contains ID - a thread which currently owns the mutex.
	* If the thread ID = 0 - the mutex is free and is signalled
	* If the thread ID is not 0 - the mutex is in nonsijgnalled state.
	* The difference between mutex and critical section is the mutex possibility
	* to set timeout
	* This wait function simply checks ID value for the mutex
	* If ID = 0 - it processes futher but when ID != 0 - it waits
	*/
   DWORD dw = WaitForSingleObject(m_hmtxQ, dwTimeout);

   /* If the state of the mutex becomes signaled the queue is available to use.
    * The other thread which was working the queue has just finished it work 
	* WAIT_OBJECT_0 indicates the successful state of the wait function.
	*/
   if (dw == WAIT_OBJECT_0) 
   {
      /* This thread has exclusive access to the queue
	   * Increment the number of elements in the queue
	   */
      LONG lPrevCount;
	  /* Increases the count of the specified semaphore object by a specified amount.
	   *
	   * hSemaphore [in] 
       *  A handle to the semaphore object. 
	   *  The CreateSemaphore or OpenSemaphore function returns this handle.
       *  This handle must have the SEMAPHORE_MODIFY_STATE access right.
	   *
       * lReleaseCount [in] 
       *  The amount by which the semaphore object's current count is to be increased. 
	   *  The value must be greater than zero. 
	   *  If the specified amount would cause the semaphore's count to exceed 
	   *  the maximum count that was specified when the semaphore was created, 
	   *  the count is not changed and the function returns FALSE.
       * 
	   * lpPreviousCount [out, optional] 
       *  A pointer to a variable to receive the previous count for the semaphore. 
	   *  This parameter can be NULL if the previous count is not required.
       *
	   * This function puts a new elements into queue. So read method should wait 
	   * until there is at least one element in the queue. Initially semafore
	   * has usage count 0 and it is in nonsignalled state. All reader threads 
	   * are waiting for the semafore to become signalled.
	   * The semafore keeps the total number of simultaneous request in queue.
	   * If the total number is the max possible value and we try to increment 
	   * this value - the function fails. It shows that the semafore is full
	   * and we send to client a message: The queue is full. Try, please, later.
	   */
      fOk = ReleaseSemaphore(m_hsemNumElements, 1, &lPrevCount);
      if (fOk) 
	  {
         /* The queue is not full, append the new element */
         m_pElements[lPrevCount] = *pElement;
      } 
	  else 
	  {
         /* The queue is full, set the error code and return failure */
         SetLastError(ERROR_DATABASE_FULL);
      }

      /* Allow other threads to access the queue 
	   * We have just made all needed work with queue and we can release the mutex.
	   * After releasing ID = 0 and one of the waiting threads gains access.
	   */
      ReleaseMutex(m_hmtxQ);
   } 
   /* Some error has happened, possibly timeout has expired.
    * In comparison to critical sections, mutex allows  
	* to set timeout. */
   else 
   {
      /* Timeout, set error code and return failure */
      SetLastError(ERROR_TIMEOUT);
   }

   return(fOk);   /* Call GetLastError for more info */
}

/* This is a reader function which extracts elements from the queue */
BOOL CQueue::Remove(PELEMENT pElement, DWORD dwTimeout) 
{
   /* Wait for exclusive access to queue and for queue to have element. 
    * First of all we wait for the semafore to become signalled, to have 
	* total count more than 0. 
	* And we also wait for the mutex to become signalled. Using this way we 
	* guarantee exclusive access to the queue. When the queue is being read or
	* being modified the mutex is non-signalled. It becomes signalled when one of 
	* threads releases the mutex.
	* The semafore total count is 0 - This thread waits for at least one item.
	* After an item has been added into queue, the semafore becomes signalled
	* and WaitForMultipleObjects decrements the total count in semafore.
    */
   BOOL fOk = 
	   (WaitForMultipleObjects(_countof(m_h), m_h, TRUE, dwTimeout) == WAIT_OBJECT_0);

   if (fOk) 
   {
      // The queue has an element, pull it from the queue
      *pElement = m_pElements[0];

      // Shift the remaining elements down
      MoveMemory(&m_pElements[0], &m_pElements[1], 
         sizeof(ELEMENT) * (m_nMaxElements - 1));

      // Allow other threads to access the queue
      ReleaseMutex(m_hmtxQ);

   } else 
   {
      // Timeout, set error code and return failure
      SetLastError(ERROR_TIMEOUT);
   }

   return(fOk);   // Call GetLastError for more info
}


///////////////////////////////////////////////////////////////////////////////


CQueue g_q(10);                     // The shared queue
volatile LONG g_fShutdown = FALSE;  // Signals client/server threads to die
HWND g_hwnd;                        // How client/server threads give status


// Handles to all client/server threads & number of client/server threads
HANDLE g_hThreads[MAXIMUM_WAIT_OBJECTS];  
int    g_nNumThreads = 0;


///////////////////////////////////////////////////////////////////////////////


DWORD WINAPI ClientThread(PVOID pvParam) {

   int nThreadNum = PtrToUlong(pvParam);
   HWND hwndLB = GetDlgItem(g_hwnd, IDC_CLIENTS);

   int nRequestNum = 0;
   while (1 != InterlockedCompareExchange(&g_fShutdown, 0, 0)) {

      // Keep track of the current processed element
      nRequestNum++;
      
      TCHAR sz[1024];
      CQueue::ELEMENT e = { nThreadNum, nRequestNum };

      // Try to put an element on the queue
      if (g_q.Append(&e, 200)) {

         // Indicate which thread sent it and which request
         StringCchPrintf(sz, _countof(sz), TEXT("Sending %d:%d"), 
            nThreadNum, nRequestNum);
      } else {

         // Couldn't put an element on the queue
         StringCchPrintf(sz, _countof(sz), TEXT("Sending %d:%d (%s)"), 
            nThreadNum, nRequestNum, (GetLastError() == ERROR_TIMEOUT) 
               ? TEXT("timeout") : TEXT("full"));
      }

      // Show result of appending element
      ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));
      Sleep(2500);   // Wait before appending another element
   }
   
   return(0);
}


///////////////////////////////////////////////////////////////////////////////


DWORD WINAPI ServerThread(PVOID pvParam) {

   int nThreadNum = PtrToUlong(pvParam);
   HWND hwndLB = GetDlgItem(g_hwnd, IDC_SERVERS);

   while (1 != InterlockedCompareExchange(&g_fShutdown, 0, 0)) {

      TCHAR sz[1024];
      CQueue::ELEMENT e;

      // Try to get an element from the queue
      if (g_q.Remove(&e, 5000)) {

         // Indicate which thread is processing it, which thread
         // sent it and which request we're processing
         StringCchPrintf(sz, _countof(sz), TEXT("%d: Processing %d:%d"), 
            nThreadNum, e.m_nThreadNum, e.m_nRequestNum);

         // The server takes some time to process the request
         Sleep(2000 * e.m_nThreadNum);

      } else {
         // Couldn't get an element from the queue
         StringCchPrintf(sz, _countof(sz), TEXT("%d: (timeout)"), nThreadNum);
      }

      // Show result of processing element
      ListBox_SetCurSel(hwndLB, ListBox_AddString(hwndLB, sz));
   }

   return(0);
}


///////////////////////////////////////////////////////////////////////////////


BOOL Dlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam) {

   chSETDLGICONS(hwnd, IDI_QUEUE);

   g_hwnd = hwnd; // Used by client/server threads to show status

   DWORD dwThreadID;

   // Create the client threads
   for (int x = 0; x < 4; x++)
      g_hThreads[g_nNumThreads++] = 
         chBEGINTHREADEX(NULL, 0, ClientThread, (PVOID) (INT_PTR) x, 
            0, &dwThreadID);

   // Create the server threads
   for (int x = 0; x < 2; x++)
      g_hThreads[g_nNumThreads++] = 
         chBEGINTHREADEX(NULL, 0, ServerThread, (PVOID) (INT_PTR) x, 
            0, &dwThreadID);

   return(TRUE);
}


///////////////////////////////////////////////////////////////////////////////


void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

   switch (id) {
      case IDCANCEL:
         EndDialog(hwnd, id);
         break;
   }
}


///////////////////////////////////////////////////////////////////////////////


INT_PTR WINAPI Dlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
   
   switch (uMsg) {
      chHANDLE_DLGMSG(hwnd, WM_INITDIALOG, Dlg_OnInitDialog);
      chHANDLE_DLGMSG(hwnd, WM_COMMAND,    Dlg_OnCommand);
   }
   return(FALSE);
}


///////////////////////////////////////////////////////////////////////////////


int WINAPI _tWinMain(HINSTANCE hinstExe, HINSTANCE, PTSTR pszCmdLine, int) {

   DialogBox(hinstExe, MAKEINTRESOURCE(IDD_QUEUE), NULL, Dlg_Proc);
   InterlockedExchange(&g_fShutdown, TRUE);

   // Wait for all the threads to terminate & then cleanup
   WaitForMultipleObjects(g_nNumThreads, g_hThreads, TRUE, INFINITE);
   while (g_nNumThreads--)
      CloseHandle(g_hThreads[g_nNumThreads]);

   return(0);
}


//////////////////////////////// End of File //////////////////////////////////
