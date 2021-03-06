
// Description:
//
//    This sample illustrates how to develop a simple echo server Winsock
//    application using the Overlapped I/O model with event notification. This
//    sample is implemented as a console-style application and simply prints
//    messages when connections are established and removed from the server.
//    The application listens for TCP connections on port 5150 and accepts them
//    as they arrive. When this application receives data from a client, it
//    simply echoes (this is why we call it an echo server) the data back in
//    it's original form until the client closes the connection.
//
//    Note: There are no command line options for this sample.
//

#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "winsocketoverlapped1.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

#define DEFAULT_BUFLEN 512

typedef struct _SOCKET_INFORMATION_OVERLAPPED1 {
   CHAR Buffer[DEFAULT_BUFLEN];
   WSABUF DataBuf;
   SOCKET Socket;
   WSAOVERLAPPED Overlapped;
   DWORD BytesSEND;
   DWORD BytesRECV;
} SOCKET_INFORMATION_OVERLAPPED1, *LPSOCKET_INFORMATION_OVERLAPPED1;


int main_overlapped_acceptex()
{
    DWORD EventTotal = 0;
    WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
    LPSOCKET_INFORMATION_OVERLAPPED1 SocketArray[WSA_MAXIMUM_WAIT_EVENTS];
    CHAR AcceptBuffer[2 * (sizeof(SOCKADDR_IN) + 16)];
    WSAOVERLAPPED ListenOverlapped;
    DWORD Bytes;
    DWORD Index;
    DWORD Flags;
    DWORD BytesTransferred;
    LPSOCKET_INFORMATION_OVERLAPPED1 SI;
    WSADATA wsaData;
    SOCKET ListenSocket, AcceptSocket;
    SOCKADDR_IN InternetAddr;
    DWORD RecvBytes, SendBytes;
    DWORD i;
 
    if ((WSAStartup((2,2),&wsaData)) != 0)
    {
        printf("WSAStartup() failed with error %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
    if ((ListenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        printf("Failed to get a socket %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSASocket() is OK!\n");
 
    InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(5150);

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
        printf("listen() is OK! I am listening...\n");
 
    // Setup the listening socket for connections
    if ((AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
    {
        printf("Failed to get a socket %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSASocket() is OK!\n");
 
    ZeroMemory(&ListenOverlapped, sizeof(OVERLAPPED));
    if ((EventArray[0] = ListenOverlapped.hEvent = WSACreateEvent()) == WSA_INVALID_EVENT)
    {
        printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSACreateEvent() is OK!\n");
 
    EventTotal = 1;
 
	/* 
	 * The AcceptEx function accepts a new connection, 
	 * returns the local and remote address, 
	 * and receives the first block of data sent by the client application.
	 * 
     * Note  This function is a Microsoft-specific extension to the Windows Sockets specification.
	 * 
	 * sListenSocket [in]
     *   A descriptor identifying a socket 
	 *   that has already been called with the listen function. 
	 *   A server application waits for attempts to connect on this socket.
	 *
     * sAcceptSocket [in]
     *   A descriptor identifying a socket on which to accept an incoming connection. 
	 *   This socket must not be bound or connected.
	 *
     * lpOutputBuffer [in]
     *   A pointer to a buffer that receives the first block of data sent on a new connection, 
	 *   the local address of the server, and the remote address of the client. 
	 *   The receive data is written to the first part of the buffer starting at offset zero, 
	 *   while the addresses are written to the latter part of the buffer. 
	 *   This parameter must be specified.
	 * 
	 * dwReceiveDataLength [in]
     *   The number of bytes in lpOutputBuffer 
	 *   that will be used for actual receive data at the beginning of the buffer. 
	 * 
	 * dwLocalAddressLength [in]
     *   The number of bytes reserved for the local address information. 
	 *   This value must be at least 16 bytes more than the maximum address length 
	 *   for the transport protocol in use.
     *
     * dwRemoteAddressLength [in]
     *   The number of bytes reserved for the remote address information. 
	 *   This value must be at least 16 bytes more 
	 *   than the maximum address length for the transport protocol in use. Cannot be zero.
	 * 
	 * lpdwBytesReceived [out]
     *   A pointer to a DWORD that receives the count of bytes received. 
	 *   This parameter is set only if the operation completes synchronously. 
     * 
	 * lpOverlapped [in]
     *   An OVERLAPPED structure that is used to process the request. 
	 *   This parameter must be specified; it cannot be NULL.
	 */
    if (AcceptEx(
		  ListenSocket, 
		  AcceptSocket, 
		  (PVOID)AcceptBuffer, 
		  0,
          sizeof(SOCKADDR_IN) + 16, 
		  sizeof(SOCKADDR_IN) + 16, 
		  &Bytes, 
		  &ListenOverlapped) == FALSE)
	{
	    if (WSAGetLastError() != ERROR_IO_PENDING)
		{
		    printf("AcceptEx() failed with error %d\n", WSAGetLastError());
			return 1;
		}
		else
		    printf("AcceptEx() is OK!\n");
	}
 
	// Process asynchronous AcceptEx, WSASend, WSARecv requests
	while(TRUE)
	{
		if ((Index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
		{
		    printf("WSAWaitForMultipleEvents() failed %d\n", WSAGetLastError());
		    return 1;
		}
		else
		    printf("WSAWaitForMultipleEvents() is OK!\n");

        // If the event triggered was zero then a connection attempt was made
        // on our listening socket
        if ((Index - WSA_WAIT_EVENT_0) == 0)
        {
            // Check the returns from the overlapped I/O operation on the listening socket
            if (WSAGetOverlappedResult(ListenSocket, &(ListenOverlapped), &BytesTransferred, FALSE, &Flags) == FALSE)
            {
                printf("WSAGetOverlappedResult() failed with error %d\n", WSAGetLastError());
                return 1;
            }
            else
                printf("WSAGetOverlappedResult() is OK!\n");
 
            printf("Socket %d got connected...\n", AcceptSocket);
            if (EventTotal > WSA_MAXIMUM_WAIT_EVENTS)
            {
                printf("Too many connections - closing socket.\n");
                closesocket(AcceptSocket);
                continue;
            }
            else
            {
				// Create a socket information structure to associate with the accepted socket
				if ((SocketArray[EventTotal] = (LPSOCKET_INFORMATION_OVERLAPPED1) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION_OVERLAPPED1))) == NULL)
				{
				    printf("GlobalAlloc() failed with error %d\n", GetLastError());
				    return 1;
				}
				else
				    printf("GlobalAlloc() for LPSOCKET_INFORMATION is OK!\n");

				// Fill in the details of our accepted socket
				SocketArray[EventTotal]->Socket = AcceptSocket;
				ZeroMemory(&(SocketArray[EventTotal]->Overlapped), sizeof(OVERLAPPED));
				SocketArray[EventTotal]->BytesSEND = 0;
				SocketArray[EventTotal]->BytesRECV = 0;
				SocketArray[EventTotal]->DataBuf.len = DEFAULT_BUFLEN;
				SocketArray[EventTotal]->DataBuf.buf = SocketArray[EventTotal]->Buffer;

                if ((SocketArray[EventTotal]->Overlapped.hEvent = EventArray[EventTotal] = WSACreateEvent()) == WSA_INVALID_EVENT)
                {
                    printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
                    return 1;
                }
                else
                    printf("WSACreateEvent() is OK!\n");
 
                // Post a WSARecv request to to begin receiving data on the socket
                if (WSARecv(
					  SocketArray[EventTotal]->Socket, 
					  &(SocketArray[EventTotal]->DataBuf), 
					  1, 
					  &RecvBytes, 
					  &Flags,
                      &(SocketArray[EventTotal]->Overlapped), 
					  NULL) == SOCKET_ERROR)
                {
                    if (WSAGetLastError() != ERROR_IO_PENDING)
                    {
                        printf("WSARecv() failed with error %d\n", WSAGetLastError());
                        return 1;
                    }
                }
                else
                    printf("WSARecv() is OK!\n");
 
                EventTotal++;
            }
 
            // Make a new socket for accepting future connections and post another AcceptEx call
            if ((AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
            {
                printf("Failed to get a socket %d\n", WSAGetLastError());
                return 1;
            }
            else
                printf("WSASocket() is OK!\n");
 
            WSAResetEvent(EventArray[0]);
            ZeroMemory(&ListenOverlapped, sizeof(OVERLAPPED));
            ListenOverlapped.hEvent = EventArray[0];
 
            if (AcceptEx(ListenSocket, AcceptSocket, (PVOID) AcceptBuffer, 0,
                        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &Bytes, &ListenOverlapped) == FALSE)
            {
                if (WSAGetLastError() != ERROR_IO_PENDING)
                {
					printf("AcceptEx() failed with error %d\n", WSAGetLastError());
					return 1;
                }
            }
            else
                printf("AcceptEx() is OK!\n");

            continue;
        }
 
        SI = SocketArray[Index - WSA_WAIT_EVENT_0];
        WSAResetEvent(EventArray[Index - WSA_WAIT_EVENT_0]);
        if (WSAGetOverlappedResult(SI->Socket, &(SI->Overlapped), &BytesTransferred, FALSE, &Flags) == FALSE)
        {
            printf("WSAGetOverlappedResult() failed with error %d\n", WSAGetLastError());
            return 1;
        }
        else
            printf("WSAGetOverlappedResult() is OK!\n");
 
        // First check to see if the peer has closed the connection and if so
        // then close the socket and cleanup the SOCKET_INFORMATION structure
        // associated with the socket
        if (BytesTransferred == 0)
        {
            printf("Closing socket %d\n", SI->Socket);
            if (closesocket(SI->Socket) == SOCKET_ERROR)
            {
                printf("closesocket() failed with error %d\n", WSAGetLastError());
            }
            else
                printf("closesocket() is OK!\n");
 
            GlobalFree(SI);
            WSACloseEvent(EventArray[Index - WSA_WAIT_EVENT_0]);

            // Cleanup SocketArray and EventArray by removing the socket event handle
            // and socket information structure if they are not at the end of the arrays
            if ((Index - WSA_WAIT_EVENT_0) + 1 != EventTotal)
                for (i = Index - WSA_WAIT_EVENT_0; i < EventTotal; i++)
                {
                    EventArray[i] = EventArray[i + 1];
                    SocketArray[i] = SocketArray[i + 1];
                }

            EventTotal--;
            continue;
        }
 
		// Check to see if the BytesRECV field equals zero. If this is so, then
		// this means a WSARecv call just completed so update the BytesRECV field
		// with the BytesTransferred value from the completed WSARecv() call
		if (SI->BytesRECV == 0)
		{
			SI->BytesRECV = BytesTransferred;
			SI->BytesSEND = 0;
		}
		else
		{
			SI->BytesSEND += BytesTransferred;
		}
 
        if (SI->BytesRECV > SI->BytesSEND)
        {
            // Post another WSASend() request
            // Since WSASend() is not guaranteed to send all of the bytes requested,
            // continue posting WSASend() calls until all received bytes are sent
            ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
            SI->Overlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];
            SI->DataBuf.buf = SI->Buffer + SI->BytesSEND;
            SI->DataBuf.len = SI->BytesRECV - SI->BytesSEND;
 
            if (WSASend(SI->Socket, &(SI->DataBuf), 1, &SendBytes, 0, &(SI->Overlapped), NULL) == SOCKET_ERROR)
            {
				if (WSAGetLastError() != ERROR_IO_PENDING)
				{
					printf("WSASend() failed with error %d\n", WSAGetLastError());
					return 1;
				}
            }
            else
                printf("WSASend() is OK!\n");
        }
        else
        {
            SI->BytesRECV = 0;
            // Now that there are no more bytes to send post another WSARecv() request
            Flags = 0;
            ZeroMemory(&(SI->Overlapped), sizeof(WSAOVERLAPPED));
            SI->Overlapped.hEvent = EventArray[Index - WSA_WAIT_EVENT_0];
            SI->DataBuf.len = DEFAULT_BUFLEN;
            SI->DataBuf.buf = SI->Buffer;
 
            if (WSARecv(SI->Socket, &(SI->DataBuf), 1, &RecvBytes, &Flags, &(SI->Overlapped), NULL) == SOCKET_ERROR)
            {
                if (WSAGetLastError() != ERROR_IO_PENDING)
                {
                    printf("WSARecv() failed with error %d\n", WSAGetLastError());
                    return 1;
                }
            }
            else
                printf("WSARecv() is OK!\n");
        }
  }   
}

