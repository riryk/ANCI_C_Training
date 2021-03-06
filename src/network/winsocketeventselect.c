

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "winsocketeventselect.h"

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512


void socket_server_event()
{
	SOCKET SocketArray [WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT EventArray [WSA_MAXIMUM_WAIT_EVENTS], NewEvent;
	SOCKADDR_IN InternetAddr;
	WSANETWORKEVENTS NetworkEvents;
	SOCKET Accept, Listen;
	DWORD EventTotal = 0;
	DWORD Index, i;
	char buffer[DEFAULT_BUFLEN];
	WSADATA wsaData;

	// Prepare echo server
    if (WSAStartup((2,2), &wsaData) != 0)
    {
        printf("WSAStartup() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
	// Set up a TCP socket for listening on port 5150
    if ((Listen = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("socket() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("socket() is pretty fine!\n");

	InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(5150);

    if (bind(Listen, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
    {
        printf("bind() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("bind() is OK maaa!\n");

    /* The WSACreateEvent function creates a new event object.
	 * The WSACreateEvent function creates a manual-reset event object 
	 * with an initial state of nonsignaled. 
	 * The handle of the event object returned cannot be inherited by child processes. 
	 * The event object is unnamed.
	 * 
	 * If no error occurs, WSACreateEvent returns the handle of the event object. 
	 * Otherwise, the return value is WSA_INVALID_EVENT. 
	 * To get extended error information, call WSAGetLastError.
	 */
    NewEvent = WSACreateEvent();

    if (NewEvent == WSA_INVALID_EVENT)
    {
        printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSACreateEvent() is also OK! I am listening now...\n");

	/* The WSAEventSelect function specifies an event object 
	 * to be associated with the specified set of FD_XXX network events. 
	 * s [in]
     *   A descriptor identifying the socket.
	 * hEventObject [in]
     *   A handle identifying the event object 
	 *   to be associated with the specified 
	 *   set of FD_XXX network events.
	 * lNetworkEvents [in]
     *   A bitmask that specifies the combination of FD_XXX 
	 *   network events in which the application has interest.
	 */
    if (WSAEventSelect(Listen, NewEvent, FD_ACCEPT|FD_CLOSE) == SOCKET_ERROR)
    {
        printf("WSAEventSelect() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAEventSelect() is OK maaa!\n");

	if (listen(Listen, 5))
    {
        printf("listen() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("listen() is also OK! I am listening now...\n");

    SocketArray[EventTotal] = Listen;
    EventArray[EventTotal] = NewEvent;
    EventTotal++;

    while(TRUE)
    {
       // Wait for network events on all sockets
	   /* The WSAWaitForMultipleEvents function returns 
	    * when one or all of the specified event objects 
		* are in the signaled state, when the time-out interval expires, 
		* or when an I/O completion routine has executed.
		*
		* cEvents [in]
        *  The number of event object handles in the array pointed to by lphEvents. 
		*
		* lphEvents [in]
        *  A pointer to an array of event object handles. 
		*  The array can contain handles of objects of different types. 
		*  It may not contain multiple copies of the same handle 
		*  if the fWaitAll parameter is set to TRUE. 
		*
		* fWaitAll [in]
        *  A value that specifies the wait type. 
		*  If TRUE, the function returns when the state of 
		*  all objects in the lphEvents array is signaled. 
		*  If FALSE, the function returns when any of the event objects is signaled. 
		*  In the latter case, the return value minus WSA_WAIT_EVENT_0 
		*  indicates the index of the event object whose state caused the function to return.
		*  
		* dwTimeout [in]
        *  The time-out interval, in milliseconds. 
		*  WSAWaitForMultipleEvents returns if the time-out interval expires, 
		*  even if conditions specified by the fWaitAll parameter are not satisfied.
		*  If dwTimeout is WSA_INFINITE, WSAWaitForMultipleEvents waits forever; 
		*  that is, the time-out interval never expires.
		* 
		* fAlertable [in]
        *  A value that specifies whether the thread is placed 
		*  in an alertable wait state so the system can execute I/O completion routines. 
		*  If TRUE, the thread is placed in an alertable wait state 
		*  and WSAWaitForMultipleEvents can return 
		*  when the system executes an I/O completion routine. 
		*  In this case, WSA_WAIT_IO_COMPLETION is returned and the event that was being waited on is not signaled yet. The application must call the WSAWaitForMultipleEvents function again. If FALSE, the thread is not placed in an alertable wait state and I/O completion routines are not executed.
		* 
	    */
       Index = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, WSA_INFINITE, FALSE);
       Index = Index - WSA_WAIT_EVENT_0;
 
       // Iterate through all events to see if more than one is signaled
       for(i=Index; i < EventTotal ;i++)
       {
          Index = WSAWaitForMultipleEvents(1, &EventArray[i], TRUE, 1000, FALSE);
          if ((Index == WSA_WAIT_FAILED) || (Index == WSA_WAIT_TIMEOUT))
             continue;
          else
          {
             Index = i;
			 /* The WSAEnumNetworkEvents function discovers occurrences 
			  * of network events for the indicated socket, 
			  * clear internal network event records, 
			  * and reset event objects (optional). 
			  * s [in]
              *  A descriptor identifying the socket.
              * hEventObject [in]
              *  An optional handle identifying an associated event object to be reset.
              * lpNetworkEvents [out]
              *  A pointer to a WSANETWORKEVENTS structure 
			  *  that is filled with a record of network events 
			  *  that occurred and any associated error codes.
			  * 
			  * The return value is zero if the operation was successful. 
			  * Otherwise, the value SOCKET_ERROR is returned, 
			  * and a specific error number can be retrieved by calling WSAGetLastError.
			  */
             WSAEnumNetworkEvents(SocketArray[Index], EventArray[Index], &NetworkEvents);

             // Check for FD_ACCEPT messages
             if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
             {
				 if (NetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
				 {
					 printf("FD_ACCEPT failed with error %d\n", NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
					 break;
				 }
				 // Accept a new connection, and add it to the socket and event lists
				 Accept = accept(SocketArray[Index], NULL, NULL);
				 // We cannot process more than
				 // WSA_MAXIMUM_WAIT_EVENTS sockets, so close the accepted socket
				 if (EventTotal > WSA_MAXIMUM_WAIT_EVENTS)
				 {
					 printf("Too many connections");
					 closesocket(Accept);
					 break;
				 }

                 NewEvent = WSACreateEvent();
                 WSAEventSelect(Accept, NewEvent, FD_READ | FD_WRITE | FD_CLOSE);
                 EventArray[EventTotal] = NewEvent;
                 SocketArray[EventTotal] = Accept;
                 EventTotal++;
                 printf("Socket %d connected\n", Accept);
             }

			 // Process FD_READ notification
			 if (NetworkEvents.lNetworkEvents & FD_READ)
			 {
				 if (NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
				 {
					 printf("FD_READ failed with error %d\n",NetworkEvents.iErrorCode[FD_READ_BIT]);
					 break;
				 }
				 // Read data from the socket
				 recv(SocketArray[Index - WSA_WAIT_EVENT_0], buffer, sizeof(buffer), 0);
			 }

             // Process FD_WRITE notification
             if (NetworkEvents.lNetworkEvents & FD_WRITE)
             {
                 if (NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
                 {
                     printf("FD_WRITE failed with error %d\n",  NetworkEvents.iErrorCode[FD_WRITE_BIT]);
                     break;
                 }
                 send(SocketArray[Index - WSA_WAIT_EVENT_0], buffer, sizeof(buffer), 0);
             }
 
             if (NetworkEvents.lNetworkEvents & FD_CLOSE)
             {
                 if (NetworkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
                 {
                    printf("FD_CLOSE failed with error %d\n",  NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
                    break;
                 }
                 closesocket(SocketArray[Index]);
                 // Remove socket and associated event from
                 // the Socket and Event arrays and decrement EventTotal
                 //CompressArrays(EventArray, SocketArray, &EventTotal);
             }
	      }
	   }
    }
}

// Description:
//
//    This sample illustrates how to develop a simple echo server Winsock
//    application using the WSAEventSelect() I/O model. This sample is
//    implemented as a console-style application and simply prints
//    messages when connections are established and removed from the server.
//    The application listens for TCP connections on port 5150 and accepts them
//    as they arrive. When this application receives data from a client, it
//    simply echoes (this is why we call it an echo server) the data back in
//    it's original form until the client closes the connection.
//
//    Note: There are no command line options for this sample.
// Link to ws2_32.lib


typedef struct _SOCKET_INFORMATION_EVENT {
   CHAR Buffer[DEFAULT_BUFLEN];
   WSABUF DataBuf;
   SOCKET Socket;
   DWORD BytesSEND;
   DWORD BytesRECV;
} SOCKET_INFORMATION_EVENT, * LPSOCKET_INFORMATION_EVENT;


BOOL CreateSocketInformation_Event(SOCKET s);
void FreeSocketInformation_Event(DWORD Event);
 
DWORD EventTotal_Event = 0;
WSAEVENT EventArray_Event[WSA_MAXIMUM_WAIT_EVENTS];
LPSOCKET_INFORMATION_EVENT SocketArray_Event[WSA_MAXIMUM_WAIT_EVENTS];

BOOL CreateSocketInformation_Event(SOCKET s)
{
    LPSOCKET_INFORMATION_EVENT SI;

    if ((EventArray_Event[EventTotal_Event] = WSACreateEvent()) == WSA_INVALID_EVENT)
    {
        printf("WSACreateEvent() failed with error %d\n", WSAGetLastError());
        return FALSE;
    }
    else
        printf("WSACreateEvent() is OK!\n");

    if ((SI = (LPSOCKET_INFORMATION_EVENT) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION_EVENT))) == NULL)
    {
        printf("GlobalAlloc() failed with error %d\n", GetLastError());
        return FALSE;
    }
    else
        printf("GlobalAlloc() for LPSOCKET_INFORMATION is OK!\n");

    // Prepare SocketInfo structure for use
    SI->Socket = s;
    SI->BytesSEND = 0;
    SI->BytesRECV = 0;

    SocketArray_Event[EventTotal_Event] = SI;
    EventTotal_Event++;
    return(TRUE);
}

void FreeSocketInformation_Event(DWORD Event)
{
    LPSOCKET_INFORMATION_EVENT SI = SocketArray_Event[Event];
    DWORD i;

    closesocket(SI->Socket);
    GlobalFree(SI);
 
    if(WSACloseEvent(EventArray_Event[Event]) == TRUE)
		printf("WSACloseEvent() is OK!\n\n");
    else
        printf("WSACloseEvent() failed miserably!\n\n");
 
    // Squash the socket and event arrays
    for (i = Event; i < EventTotal_Event; i++)
    {
		EventArray_Event[i] = EventArray_Event[i + 1];
		SocketArray_Event[i] = SocketArray_Event[i + 1];
    }
    EventTotal_Event--;
}

void socket_server_event1()
{ 
    SOCKET Listen;
	SOCKET Accept;
	SOCKADDR_IN InternetAddr;
	LPSOCKET_INFORMATION_EVENT SocketInfo;
	DWORD Event;
	WSANETWORKEVENTS NetworkEvents;
	WSADATA wsaData;
	DWORD Flags;
	DWORD RecvBytes;
	DWORD SendBytes;
 
    if (WSAStartup(0x0202, &wsaData) != 0)
    {
        printf("WSAStartup() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
    if ((Listen = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("socket() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("socket() is OK!\n");
 
    if(CreateSocketInformation_Event(Listen) == FALSE)
        printf("CreateSocketInformation() failed!\n");
    else
        printf("CreateSocketInformation() is OK lol!\n");
 
    if (WSAEventSelect(Listen, EventArray_Event[EventTotal_Event - 1], FD_ACCEPT|FD_CLOSE) == SOCKET_ERROR)
    {
        printf("WSAEventSelect() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAEventSelect() is pretty fine!\n");
 
    InternetAddr.sin_family = AF_INET;
    InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    InternetAddr.sin_port = htons(5150);
 
    if (bind(Listen, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
    {
        printf("bind() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("bind() is OK!\n");
 
    if (listen(Listen, 5))
    {
        printf("listen() failed with error %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("listen() is OK!\n");
    
	while (TRUE)
    {
        // Wait for one of the sockets to receive I/O notification and
        if ((Event = WSAWaitForMultipleEvents(EventTotal_Event, EventArray_Event, FALSE,WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
        {
            printf("WSAWaitForMultipleEvents() failed with error %d\n", WSAGetLastError());
            return 1;
        }
        else
            printf("WSAWaitForMultipleEvents() is pretty damn OK!\n");

        if (WSAEnumNetworkEvents(
			 SocketArray_Event[Event - WSA_WAIT_EVENT_0]->Socket,
             EventArray_Event[Event - WSA_WAIT_EVENT_0], 
			 &NetworkEvents) == SOCKET_ERROR)
        {
            printf("WSAEnumNetworkEvents() failed with error %d\n", WSAGetLastError());
            return 1;
        }
        else
            printf("WSAEnumNetworkEvents() should be fine!\n");

        if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
        {
            if (NetworkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
            {
				printf("FD_ACCEPT failed with error %d\n", NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
				break;
            }
 
            if ((Accept = accept(SocketArray_Event[Event - WSA_WAIT_EVENT_0]->Socket, NULL, NULL)) == INVALID_SOCKET)
            {
                printf("accept() failed with error %d\n", WSAGetLastError());
                break;
            }
            else
                printf("accept() should be OK!\n");
 
            if (EventTotal_Event > WSA_MAXIMUM_WAIT_EVENTS)
            {
                printf("Too many connections - closing socket...\n");
                closesocket(Accept);
                break;
            }

            CreateSocketInformation_Event(Accept);
 
            if (WSAEventSelect(Accept, EventArray_Event[EventTotal_Event - 1], FD_READ|FD_WRITE|FD_CLOSE) == SOCKET_ERROR)
            {
                printf("WSAEventSelect() failed with error %d\n", WSAGetLastError());
                return 1;
            }
            else
                printf("WSAEventSelect() is OK!\n");

            printf("Socket %d got connected...\n", Accept);
        }

        // Try to read and write data to and from the data buffer if read and write events occur
        if (NetworkEvents.lNetworkEvents & FD_READ || NetworkEvents.lNetworkEvents & FD_WRITE)
        {
            if (NetworkEvents.lNetworkEvents & FD_READ && NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
            {
                printf("FD_READ failed with error %d\n", NetworkEvents.iErrorCode[FD_READ_BIT]);
                break;
            }
            else
                printf("FD_READ is OK!\n");
 
            if (NetworkEvents.lNetworkEvents & FD_WRITE && NetworkEvents.iErrorCode[FD_WRITE_BIT] != 0)
            {
                printf("FD_WRITE failed with error %d\n", NetworkEvents.iErrorCode[FD_WRITE_BIT]);
                break;
            }
            else
                printf("FD_WRITE is OK!\n");

            SocketInfo = SocketArray_Event[Event - WSA_WAIT_EVENT_0];
            
			// Read data only if the receive buffer is empty
            if (SocketInfo->BytesRECV == 0)
            {
                SocketInfo->DataBuf.buf = SocketInfo->Buffer;
                SocketInfo->DataBuf.len = DEFAULT_BUFLEN;

                Flags = 0;
                
                if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
                {
                    if (WSAGetLastError() != WSAEWOULDBLOCK)
                    {
                        printf("WSARecv() failed with error %d\n", WSAGetLastError());
                        FreeSocketInformation(Event - WSA_WAIT_EVENT_0);
                        return 1;
                    }
                }
				else
				{
					printf("WSARecv() is working!\n");
					SocketInfo->BytesRECV = RecvBytes;
				} 
			}

            // Write buffer data if it is available
            if (SocketInfo->BytesRECV > SocketInfo->BytesSEND)
            {
                SocketInfo->DataBuf.buf = SocketInfo->Buffer + SocketInfo->BytesSEND;
                SocketInfo->DataBuf.len = SocketInfo->BytesRECV - SocketInfo->BytesSEND;

                if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
                {
                    if (WSAGetLastError() != WSAEWOULDBLOCK)
                    {
                        printf("WSASend() failed with error %d\n", WSAGetLastError());
                        FreeSocketInformation(Event - WSA_WAIT_EVENT_0);
                        return 1;
                    }
 
					// A WSAEWOULDBLOCK error has occurred. An FD_WRITE event will be posted
					// when more buffer space becomes available
                }
                else
                {
                    printf("WSASend() is fine! Thank you...\n");
                    SocketInfo->BytesSEND += SendBytes;

                    if (SocketInfo->BytesSEND == SocketInfo->BytesRECV)
                    {
                        SocketInfo->BytesSEND = 0;
                        SocketInfo->BytesRECV = 0;
                    }
                }
            }    
	    }

        if (NetworkEvents.lNetworkEvents & FD_CLOSE)
        {
            if (NetworkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
            {
                printf("FD_CLOSE failed with error %d\n", NetworkEvents.iErrorCode[FD_CLOSE_BIT]);
                break;
            }
            else
                printf("FD_CLOSE is OK!\n");

            printf("Closing socket information %d\n", SocketArray_Event[Event - WSA_WAIT_EVENT_0]->Socket);
            FreeSocketInformation_Event(Event - WSA_WAIT_EVENT_0);
        }  
	}
}




