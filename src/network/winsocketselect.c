
#include "winsocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <rpc.h>
#include <af_irda.h>
#include <stdio.h>
#include <wsnetbs.h>
#include <winsock.h>
#include <atalkwsh.h>
#include "winsocketselect.h"



// You can use the Netbios function to list all the NetBIOS names
// on a LANA. The following example uses a unique name as the name
// in the ncb_callname member of the NCB structure. This causes the
// adapter status to be treated as a remote call, which enables you
// to retrieve names added by other processes
//
// Link to ws2_32.lib
// Link to Netapi32.lib
#include <Nb30.h>
 


#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")
#pragma comment(lib, "Netapi32.lib")


// Description:
//    
//    This sample illustrates how to develop a simple echo server Winsock
//    application using the select() API I/O model. This sample is
//    implemented as a console-style application and simply prints
//    messages when connections are established and removed from the server.
//    The application listens for TCP connections on port 5150 and accepts
//    them as they arrive. When this application receives data from a client,
//    it simply echos (this is why we call it an echo server) the data back in
//    it's original form until the client closes the connection.
//
//    Note: There are no command line options for this sample.
//
// Link to ws2_32.lib

#define PORT 5150
//#define DATA_BUFSIZE 8192
#define DATA_BUFSIZE 100
 
typedef struct _SOCKET_INFORMATION 
{
   CHAR Buffer[DATA_BUFSIZE];
   WSABUF DataBuf;
   SOCKET Socket;
   OVERLAPPED Overlapped;
   DWORD BytesSEND;
   DWORD BytesRECV;
} SOCKET_INFORMATION, * LPSOCKET_INFORMATION;
 
// Prototypes
BOOL CreateSocketInformation(SOCKET s);
void FreeSocketInformation(DWORD Index);
 
// Global var
DWORD TotalSockets = 0;
LPSOCKET_INFORMATION SocketArray[FD_SETSIZE];

BOOL CreateSocketInformation(SOCKET s)
{
   LPSOCKET_INFORMATION SI;
 
   printf("Accepted socket number %d\n", s);
 
   if ((SI = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
   {
      printf("GlobalAlloc() failed with error %d\n", GetLastError());
      return FALSE;
   }
   else
      printf("GlobalAlloc() for SOCKET_INFORMATION is OK!\n");
 
   // Prepare SocketInfo structure for use
   SI->Socket = s;
   SI->BytesSEND = 0;
   SI->BytesRECV = 0;
 
   SocketArray[TotalSockets] = SI;
   TotalSockets++;
   return(TRUE);
}
 
void FreeSocketInformation(DWORD Index)
{
   LPSOCKET_INFORMATION SI = SocketArray[Index];
   DWORD i;
 
   closesocket(SI->Socket);
   printf("Closing socket number %d\n", SI->Socket);
   GlobalFree(SI);
 
   // Squash the socket array
   for (i = Index; i < TotalSockets; i++)
   {
      SocketArray[i] = SocketArray[i + 1];
   }
 
   TotalSockets--;
}

int WinsockSelectServer(int argc, char **argv)
{
   SOCKET ListenSocket;
   SOCKET AcceptSocket;
   SOCKADDR_IN InternetAddr;
   WSADATA wsaData;
   INT Ret;
   FD_SET WriteSet;
   FD_SET ReadSet;
   DWORD i;
   DWORD Total;
   ULONG NonBlock;
   DWORD Flags;
   DWORD SendBytes;
   DWORD RecvBytes;
 
   if ((Ret = WSAStartup(0x0202,&wsaData)) != 0)
   {
      printf("WSAStartup() failed with error %d\n", Ret);
      WSACleanup();
      return 1;
   }
   else
      printf("WSAStartup() is fine!\n");
 
   // Prepare a socket to listen for connections
   if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
   {
      printf("WSASocket() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("WSASocket() is OK!\n");
 
   InternetAddr.sin_family = AF_INET;
   InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   InternetAddr.sin_port = htons(PORT);
 
   if (bind(ListenSocket, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
   {
      printf("bind() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("bind() is OK!\n");
 
   if (listen(ListenSocket, 5))
   {
      printf("listen() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("listen() is OK!\n");
 
   // Change the socket mode on the listening socket from blocking to
   // non-block so the application will not block waiting for requests
   NonBlock = 1;
   /* The ioctlsocket function can be used on any socket in any state. 
    * It is used to set or retrieve some 
	* operating parameters associated with the socket, 
	* independent of the protocol and communications subsystem. 
	* Here are the supported commands to use in the cmd parameter and their semantics: 
	*  
	* FIONBIO
    * The *argp parameter is a pointer to an unsigned long value. 
	* Set *argp to a nonzero value if the nonblocking mode should be enabled, 
	* or zero if the nonblocking mode should be disabled. 
	* When a socket is created, it operates in blocking mode by default 
	* (nonblocking mode is disabled). 
	* This is consistent with BSD sockets.
    *     *argp value	                        Nonblocking mode
    *          0	                                 Disabled
    *        nonzero	                             Enabled
    * 
    * The WSAAsyncSelect and WSAEventSelect functions automatically 
	* set a socket to nonblocking mode. 
	* If WSAAsyncSelect or WSAEventSelect has been issued on a socket, 
	* then any attempt to use ioctlsocket to set the socket back to blocking mode 
	* will fail with WSAEINVAL.
    * To set the socket back to blocking mode, 
	* an application must first disable WSAAsyncSelect 
	* by calling WSAAsyncSelect with the lEvent parameter equal to zero, 
	* or disable WSAEventSelect by calling WSAEventSelect 
	* with the lNetworkEvents parameter equal to zero.
	*/
   if (ioctlsocket(ListenSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
   {
      printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("ioctlsocket() is OK!\n");
 
   while(TRUE)
   {
      // Prepare the Read and Write socket sets for network I/O notification
      FD_ZERO(&ReadSet);
      FD_ZERO(&WriteSet);
 
      // Always look for connection attempts
      FD_SET(ListenSocket, &ReadSet);
 
      // Set Read and Write notification for each socket based on the
      // current state the buffer.  If there is data remaining in the
      // buffer then set the Write set otherwise the Read set
      for (i = 0; i < TotalSockets; i++)
         if (SocketArray[i]->BytesRECV > SocketArray[i]->BytesSEND)
            FD_SET(SocketArray[i]->Socket, &WriteSet);
         else
            FD_SET(SocketArray[i]->Socket, &ReadSet);
 
	  /* The select function determines the status of one or more sockets, 
	   * waiting if necessary, to perform synchronous I/O. 
	   * nfds [in]
       *   Ignored. The nfds parameter is included 
	   *   only for compatibility with Berkeley sockets.
	   * readfds [in, out]
       *   An optional pointer to a set of sockets to be checked for readability.
	   * writefds [in, out]
       *   An optional pointer to a set of sockets to be checked for writability.
	   * exceptfds [in, out]
       *   An optional pointer to a set of sockets to be checked for errors.
	   * timeout [in]
	   *   The maximum time for select to wait, 
	   *   provided in the form of a TIMEVAL structure. 
	   *   Set the timeout parameter to null for blocking operations.
	   */
      if ((Total = select(0, &ReadSet, &WriteSet, NULL, NULL)) == SOCKET_ERROR)
      {
         printf("select() returned with error %d\n", WSAGetLastError());
         return 1;
      }
      else
         printf("select() is OK!\n");
 
      // Check for arriving connections on the listening socket.
	  /* FD_ISSET–Return a value for the file descriptor in the file descriptor set
       * This function returns a value for the file descriptor in the file descriptor set.
	   */
      if (FD_ISSET(ListenSocket, &ReadSet))
      {
         Total--;
         if ((AcceptSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
         {
            // Set the accepted socket to non-blocking mode so the server will
            // not get caught in a blocked condition on WSASends
            NonBlock = 1;
            if (ioctlsocket(AcceptSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
            {
               printf("ioctlsocket(FIONBIO) failed with error %d\n", WSAGetLastError());
               return 1;
            }
            else
               printf("ioctlsocket(FIONBIO) is OK!\n");
 
            if (CreateSocketInformation(AcceptSocket) == FALSE)
            {
               printf("CreateSocketInformation(AcceptSocket) failed!\n");
               return 1;
            }
            else
               printf("CreateSocketInformation() is OK!\n");
         }
         else
         {
            if (WSAGetLastError() != WSAEWOULDBLOCK)
            {
               printf("accept() failed with error %d\n", WSAGetLastError());
               return 1;
            }
            else
               printf("accept() is fine!\n");
         }
      }
 
      // Check each socket for Read and Write notification until the number
      // of sockets in Total is satisfied
      for (i = 0; Total > 0 && i < TotalSockets; i++)
	  {
		 LPSOCKET_INFORMATION SocketInfo = SocketArray[i];
 
         // If the ReadSet is marked for this socket then this means data
         // is available to be read on the socket
         if (FD_ISSET(SocketInfo->Socket, &ReadSet))
         {
            Total--;
 
            SocketInfo->DataBuf.buf = SocketInfo->Buffer;
            SocketInfo->DataBuf.len = DATA_BUFSIZE;
 
            Flags = 0;
			/* 
			 * The WSARecv function receives data 
			 * from a connected socket or a bound connectionless socket.
			 *
			 * s [in]
             *  A descriptor identifying a connected socket.
			 *
			 * lpBuffers [in, out]
             *  A pointer to an array of WSABUF structures. 
			 *  Each WSABUF structure contains a pointer 
			 *  to a buffer and the length, in bytes, of the buffer.
			 *
             * dwBufferCount [in]
             *  The number of WSABUF structures in the lpBuffers array.
			 * 
			 * lpNumberOfBytesRecvd [out]
             *  A pointer to the number, in bytes, of data received by 
			 *  this call if the receive operation completes immediately.
			 * 
			 * lpFlags [in, out]
             *  A pointer to flags used to modify the behavior of the WSARecv function call. 
             *  For more information, see the Remarks section.
			 */
            if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
            {
               if (WSAGetLastError() != WSAEWOULDBLOCK)
               {
                  printf("WSARecv() failed with error %d\n", WSAGetLastError());
                  FreeSocketInformation(i);
               }
               else
                  printf("WSARecv() is OK!\n");
               continue;
            }
            else
            {
               SocketInfo->BytesRECV = RecvBytes;
 
               // If zero bytes are received, this indicates the peer closed the connection.
               if (RecvBytes == 0)
               {
                  FreeSocketInformation(i);
                  continue;
               }
            }
         }
 
         // If the WriteSet is marked on this socket then this means the internal
         // data buffers are available for more data
         if (FD_ISSET(SocketInfo->Socket, &WriteSet))
         {
            Total--;
 
            SocketInfo->DataBuf.buf = SocketInfo->Buffer + SocketInfo->BytesSEND;
            SocketInfo->DataBuf.len = SocketInfo->BytesRECV - SocketInfo->BytesSEND;
 
            if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
            {
               if (WSAGetLastError() != WSAEWOULDBLOCK)
               {
                  printf("WSASend() failed with error %d\n", WSAGetLastError());
                  FreeSocketInformation(i);
               }
               else
                  printf("WSASend() is OK!\n");
 
               continue;
            }
            else
            {
               SocketInfo->BytesSEND += SendBytes;
 
               if (SocketInfo->BytesSEND == SocketInfo->BytesRECV)
               {
                  SocketInfo->BytesSEND = 0;
                  SocketInfo->BytesRECV = 0;
               }
            }
         }
      }
   }
}