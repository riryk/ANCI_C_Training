/******************************************************************************
Module:  IOCP.h
Notices: Copyright (c) 2007 Jeffrey Richter & Christophe Nasarre
Purpose: This class wraps an I/O Completion Port.
         See Appendix B.
******************************************************************************/


#pragma once   // Include this header file once per compilation unit


///////////////////////////////////////////////////////////////////////////////


#include "CmnHdr.h"              // See Appendix A.


///////////////////////////////////////////////////////////////////////////////


class CIOCP {
public:
   CIOCP(int nMaxConcurrency = -1) { 
      m_hIOCP = NULL; 
      if (nMaxConcurrency != -1)
         (void) Create(nMaxConcurrency);
   }

   ~CIOCP() { 
      if (m_hIOCP != NULL) 
         chVERIFY(CloseHandle(m_hIOCP)); 
   }


   BOOL Close() {
      BOOL bResult = CloseHandle(m_hIOCP);
      m_hIOCP = NULL;
      return(bResult);
   }

   BOOL Create(int nMaxConcurrency = 0) 
   {
       /* Creates an input/output (I/O) completion port 
	    * and associates it with a specified file handle, 
		* or creates an I/O completion port that 
		* is not yet associated with a file handle, 
		* allowing association at a later time.
        * Associating an instance of an opened file handle 
		* with an I/O completion port allows a process 
		* to receive notification of the completion of asynchronous I/O operations 
		* involving that file handle.
	    * 
		* The term file handle as used here refers to a system abstraction 
		* that represents an overlapped I/O endpoint, 
		* not only a file on disk. 
		* Any system objects that support overlapped I/O—
		* such as network endpoints, TCP sockets, named pipes, and mail slots
		* —can be used as file handles. For additional information, see the Remarks section.
		* 
        * NumberOfConcurrentThreads [in]
		*
        * The maximum number of threads that the operating system 
		* can allow to concurrently process I/O completion packets 
		* for the I/O completion port. 
		* This parameter is ignored if the ExistingCompletionPort parameter is not NULL.
        * If this parameter is zero, the system allows as many concurrently running threads 
		* as there are processors in the system.   
	    */
       m_hIOCP = 
		  CreateIoCompletionPort(
             INVALID_HANDLE_VALUE, 
			 NULL, 
			 0, 
			 nMaxConcurrency);

      chASSERT(m_hIOCP != NULL);
      return(m_hIOCP != NULL);
   }

   BOOL AssociateDevice(HANDLE hDevice, ULONG_PTR CompKey) 
   {
      BOOL fOk = 
		  (
		      /* ExistingCompletionPort [in, optional]
               *   A handle to an existing I/O completion port or NULL.
               *   If this parameter specifies an existing I/O completion port, 
			   *   the function associates it with the handle specified by the FileHandle parameter. 
			   *   The function returns the handle of the existing I/O completion port if successful; 
			   *   it does not create a new I/O completion port.
               *   If this parameter is NULL, 
			   *   the function creates a new I/O completion port and, 
			   *   if the FileHandle parameter is valid, 
			   *   associates it with the new I/O completion port. 
			   *   Otherwise no file handle association occurs. 
			   *   The function returns the handle to the new I/O completion port if successful.
			   *   
			   */  
		      CreateIoCompletionPort(hDevice, m_hIOCP, CompKey, 0) == m_hIOCP
		  );
      chASSERT(fOk);
      return(fOk);
   }

   BOOL AssociateSocket(SOCKET hSocket, ULONG_PTR CompKey) {
      return(AssociateDevice((HANDLE) hSocket, CompKey));
   }

   BOOL PostStatus(
	   ULONG_PTR CompKey, 
	   DWORD dwNumBytes = 0, 
       OVERLAPPED* po = NULL) 
   {
      /* Posts an I/O completion packet to an I/O completion port.
	   *
	   * CompletionPort [in]
       *   A handle to an I/O completion port to which 
	   *   the I/O completion packet is to be posted.
	   * 
	   * dwNumberOfBytesTransferred [in]
       *   The value to be returned through the lpNumberOfBytesTransferred parameter 
	   *   of the GetQueuedCompletionStatus function.
	   * 
	   * dwCompletionKey [in]
       *   The value to be returned through the lpCompletionKey parameter 
	   *   of the GetQueuedCompletionStatus function.
	   *
	   * lpOverlapped [in, optional]
       *   The value to be returned through the lpOverlapped parameter 
	   *   of the GetQueuedCompletionStatus function.  
	   */
      BOOL fOk = PostQueuedCompletionStatus(m_hIOCP, dwNumBytes, CompKey, po);
      chASSERT(fOk);
      return(fOk);
   }

   BOOL GetStatus(
	   ULONG_PTR* pCompKey, 
	   PDWORD pdwNumBytes,
       OVERLAPPED** ppo, 
	   DWORD dwMilliseconds = INFINITE) 
   {
      /* Attempts to dequeue an I/O completion packet 
	   * from the specified I/O completion port. 
	   * If there is no completion packet queued, 
	   * the function waits for a pending I/O operation associated with 
	   * the completion port to complete. */
      return (GetQueuedCompletionStatus(
		        m_hIOCP, 
				pdwNumBytes, 
                pCompKey, 
				ppo, 
				dwMilliseconds));
   }

private:
   HANDLE m_hIOCP;
};


///////////////////////////////// End of File /////////////////////////////////
