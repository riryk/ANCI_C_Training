// Description:
//
//    This sample illustrates how to develop a simple echo server Winsock
//    application using the WSAAsyncSelect() I/O model. This sample is
//    implemented as a console-style application (to reduce the programming
//    complexity of writing a real Windows application) and simply prints
//    messages when connections are established and removed from the server.
//    The application listens for TCP connections on port 5150 and accepts them
//    as they arrive. When this application receives data from a client, it
//    simply echoes the data back in it's original form until the client
//            closes the connection.
//
//    Since the WSAAsyncSelect I/O model requires an application to manage
//    window messages when network event occur, this application creates
//    a window for the I/O model only. The window stays hidden during the
//    entire execution of this application.
//
//    Note: There are no command line options for this sample.
//
// Discard unnecessary/unused headers
#define WIN32_LEAN_AND_MEAN
// Take note that windows.h already contained winsock.h! And by
// putting winsock2.h first, it will block the winsock.h re-inclusion
// Link to ws2_32.lib
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include "winsocketasyncwindow.h"

#pragma comment(lib, "Ws2_32.lib")

#define PORT 5150
#define DATA_BUFSIZE 100
 
// typedef definition
typedef struct _SOCKET_INFORMATION {
   BOOL RecvPosted;
   CHAR Buffer[DATA_BUFSIZE];
   WSABUF DataBuf;
   SOCKET Socket;
   DWORD BytesSEND;
   DWORD BytesRECV;
   struct _SOCKET_INFORMATION *Next;
} SOCKET_INFORMATION, *LPSOCKET_INFORMATION;
 
#define WM_SOCKET (WM_USER + 1)
 
// Prototypes
void CreateSocketInformationAsync(SOCKET s);
LPSOCKET_INFORMATION GetSocketInformation(SOCKET s);
void FreeSocketInformationAsync(SOCKET s);
HWND MakeWorkerWindow(void);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
 
// Global var
LPSOCKET_INFORMATION SocketInfoList;


void CreateSocketInformationAsync(SOCKET s)
{
   LPSOCKET_INFORMATION SI;
 
   if ((SI = (LPSOCKET_INFORMATION) GlobalAlloc(GPTR, sizeof(SOCKET_INFORMATION))) == NULL)
   {
      printf("GlobalAlloc() failed with error %d\n", GetLastError());
      return;
   }
   else
     printf("GlobalAlloc() for SOCKET_INFORMATION is OK!\n");
 
   // Prepare SocketInfo structure for use
   SI->Socket = s;
   SI->RecvPosted = FALSE;
   SI->BytesSEND = 0;
   SI->BytesRECV = 0;
   SI->Next = SocketInfoList;
   SocketInfoList = SI;
}

LPSOCKET_INFORMATION GetSocketInformation(SOCKET s)
{
   SOCKET_INFORMATION *SI = SocketInfoList;
 
   while(SI)
   {
      if (SI->Socket == s)
         return SI;
 
      SI = SI->Next;
   }
 
   return NULL;
}
 
void FreeSocketInformationAsync(SOCKET s)
{
   SOCKET_INFORMATION *SI = SocketInfoList;
   SOCKET_INFORMATION *PrevSI = NULL;
 
   while(SI)
   {
      if (SI->Socket == s)
      {
         if (PrevSI)
            PrevSI->Next = SI->Next;
         else
            SocketInfoList = SI->Next;
 
         closesocket(SI->Socket);
         GlobalFree(SI);
         return;
      }
 
      PrevSI = SI;
      SI = SI->Next;
   }
}
 
HWND MakeWorkerWindow(void)
{
   WNDCLASS wndclass;
   CHAR *ProviderClass = "AsyncSelect";
   HWND Window;
 
   wndclass.style = CS_HREDRAW | CS_VREDRAW;
   wndclass.lpfnWndProc = (WNDPROC)WindowProc;
   wndclass.cbClsExtra = 0;
   wndclass.cbWndExtra = 0;
   wndclass.hInstance = NULL;
   wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
   wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
   wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
   wndclass.lpszMenuName = NULL;
   wndclass.lpszClassName = (LPCWSTR)ProviderClass;
 
   /* Registers a window class for subsequent use 
    * in calls to the CreateWindow or CreateWindowEx function.
    * Note  The RegisterClass function has been superseded by the RegisterClassEx function. 
	* You can still use RegisterClass, however, 
	* if you do not need to set the class small icon.
    */
   if (RegisterClass(&wndclass) == 0)
   {
      printf("RegisterClass() failed with error %d\n", GetLastError());
      return NULL;
   }
   else
      printf("RegisterClass() is OK!\n");
 
   // Create a window
   if ((Window = CreateWindow(
      (LPCWSTR)ProviderClass,
      L"",
      WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      NULL,
      NULL,
      NULL,
      NULL)) == NULL)
   {
      printf("CreateWindow() failed with error %d\n", GetLastError());
      return NULL;
   }
   else
      printf("CreateWindow() is OK!\n");
 
   return Window;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
   SOCKET Accept;
   LPSOCKET_INFORMATION SocketInfo;
   DWORD RecvBytes;
   DWORD SendBytes;
   DWORD Flags;
 
   if (uMsg == WM_SOCKET)
   {
      /* Additional error codes can be set 
	   * when an application window receives a message. 
	   * This error code is extracted from the lParam in the reply message 
	   * using the WSAGETSELECTERROR macro. 
	   * Possible error codes for each network event are listed in the following table.
	   */
      if (WSAGETSELECTERROR(lParam))
      {
         printf("Socket failed with error %d\n", WSAGETSELECTERROR(lParam));
         FreeSocketInformation(wParam);
      }
      else
      {
		 WORD param = WSAGETSELECTEVENT(lParam);
         printf("Socket looks fine!\n");
		 /* The error and event codes can be extracted 
		  * from the lParam using the macros WSAGETSELECTERROR and WSAGETSELECTEVENT, 
		  * defined in Winsock2.h a */
         switch(WSAGETSELECTEVENT(lParam))
         {
            case FD_ACCEPT:
               if ((Accept = accept(wParam, NULL, NULL)) == INVALID_SOCKET)
               {                   
                  printf("accept() failed with error %d\n", WSAGetLastError());
                  break;
               }
               else
                  printf("accept() is OK!\n");

               // Create a socket information structure to associate with the socket for processing I/O
               CreateSocketInformationAsync(Accept);
               printf("Socket number %d connected\n", Accept);
			   /* The WSAAsyncSelect function requests Windows 
			    * message-based notification of network events for a socket. 
				* s [in]
                *   A descriptor that identifies the socket for which event notification is required.
				* hWnd [in]
                *   A handle that identifies the window 
				*   that will receive a message when a network event occurs.
                * wMsg [in]
                *   A message to be received when a network event occurs.
                * lEvent [in]
                *   A bitmask that specifies a combination of network events 
				*   in which the application is interested.
			    */
               WSAAsyncSelect(Accept, hwnd, WM_SOCKET, FD_READ|FD_WRITE|FD_CLOSE);
               break;
            case FD_READ:
               SocketInfo = GetSocketInformation(wParam);
               // Read data only if the receive buffer is empty
               if (SocketInfo->BytesRECV != 0)
               {
                  SocketInfo->RecvPosted = TRUE;
                  return 0;
               }
               else
               {
                  SocketInfo->DataBuf.buf = SocketInfo->Buffer;
                  SocketInfo->DataBuf.len = DATA_BUFSIZE;
 
                  Flags = 0;
                  if (WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes,
                     &Flags, NULL, NULL) == SOCKET_ERROR)
                  {
                     if (WSAGetLastError() != WSAEWOULDBLOCK)
                     {
                        printf("WSARecv() failed with error %d\n", WSAGetLastError());
                        FreeSocketInformation(wParam);
                        return 0;
                     }
                  }
                  else // No error so update the byte count
                  {
                     printf("WSARecv() is OK!\n");
                     SocketInfo->BytesRECV = RecvBytes;
                  }
               }
               // DO NOT BREAK HERE SINCE WE GOT A SUCCESSFUL RECV. Go ahead
               // and begin writing data to the client
            case FD_WRITE:
               SocketInfo = GetSocketInformation(wParam);
               if (SocketInfo->BytesRECV > SocketInfo->BytesSEND)
               {
                  SocketInfo->DataBuf.buf = SocketInfo->Buffer + SocketInfo->BytesSEND;
                  SocketInfo->DataBuf.len = SocketInfo->BytesRECV - SocketInfo->BytesSEND;
 
                  if (WSASend(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &SendBytes, 0,
                     NULL, NULL) == SOCKET_ERROR)
                  {
                     if (WSAGetLastError() != WSAEWOULDBLOCK)
                     {
                        printf("WSASend() failed with error %d\n", WSAGetLastError());
                        FreeSocketInformation(wParam);
                        return 0;
                     }
                  }
                  else // No error so update the byte count
                  {
                     printf("WSASend() is OK!\n");
                     SocketInfo->BytesSEND += SendBytes;
                  }
               }
 
               if (SocketInfo->BytesSEND == SocketInfo->BytesRECV)
               {
                  SocketInfo->BytesSEND = 0;
                  SocketInfo->BytesRECV = 0;
                  // If a RECV occurred during our SENDs then we need to post an FD_READ notification on the socket
                  if (SocketInfo->RecvPosted == TRUE)
                  {
                     SocketInfo->RecvPosted = FALSE;
					 /* 
					  * Places (posts) a message in the message queue 
					  * associated with the thread that created the specified 
					  * window and returns without waiting 
					  * for the thread to process the message.
					  * 
                      * To post a message in the message queue associated with a thread, 
					  * use the PostThreadMessage function. 
					  */
                     PostMessage(hwnd, WM_SOCKET, wParam, FD_READ);
                  }
               }
               break;
            case FD_CLOSE:
               printf("Closing socket %d\n", wParam);
               FreeSocketInformation(wParam);
               break;
         }
      }
      return 0;
   }
   /* 
    * Calls the default window procedure to provide 
	* default processing for any window messages 
	* that an application does not process. 
	*
	* This function ensures that every message is processed. 
	* DefWindowProc is called with the same parameters 
	* received by the window procedure.
    */
   return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main_winsock(int argc, char **argv)
{
   MSG msg;
   DWORD Ret;
   SOCKET Listen;
   SOCKADDR_IN InternetAddr;
   HWND Window;
   WSADATA wsaData;
 
   if ((Window = MakeWorkerWindow()) == NULL)
   {
      printf("MakeWorkerWindow() failed!\n");
      return 1;
   }
   else
      printf("MakeWorkerWindow() is OK!\n");
 
   // Prepare echo server
   if (WSAStartup((2,2), &wsaData) != 0)
   {
      printf("WSAStartup() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("WSAStartup() is OK!\n");
 
   if ((Listen = socket(PF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
   {
      printf("socket() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("socket() is pretty fine!\n");
 
   if(WSAAsyncSelect(Listen, Window, WM_SOCKET, FD_ACCEPT|FD_CLOSE) == 0)
      printf("WSAAsyncSelect() is OK lol!\n");
   else
      printf("WSAAsyncSelect() failed with error code %d\n", WSAGetLastError());
 
   InternetAddr.sin_family = AF_INET;
   InternetAddr.sin_addr.s_addr = htonl(INADDR_ANY);
   InternetAddr.sin_port = htons(PORT);
 
   if (bind(Listen, (PSOCKADDR) &InternetAddr, sizeof(InternetAddr)) == SOCKET_ERROR)
   {
      printf("bind() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("bind() is OK maaa!\n");
 
   if (listen(Listen, 5))
   {
      printf("listen() failed with error %d\n", WSAGetLastError());
      return 1;
   }
   else
      printf("listen() is also OK! I am listening now...\n");
                       
   // Translate and dispatch window messages for the application thread
   /*
      Retrieves a message from the calling thread's message queue. 
	  The function dispatches incoming sent messages 
	  until a posted message is available for retrieval.
      Unlike GetMessage, the PeekMessage function 
	  does not wait for a message to be posted before returning. 

	  lpMsg [out]
      Type: LPMSG
         A pointer to an MSG structure that receives 
		 message information from the thread's message queue.

	  hWnd [in, optional]
      Type: HWND
         A handle to the window whose messages are to be retrieved. 
		 The window must belong to the current thread.

         If hWnd is NULL, GetMessage retrieves messages for any window 
		 that belongs to the current thread, 
		 and any messages on the current thread's message queue 
		 whose hwnd value is NULL (see the MSG structure). 

		 Therefore if hWnd is NULL, both window messages and thread messages are processed.
         If hWnd is -1, GetMessage retrieves only messages 
		 on the current thread's message queue whose hwnd value is NULL, 
		 that is, thread messages as posted by PostMessage 
		 (when the hWnd parameter is NULL) or PostThreadMessage.

	     If wMsgFilterMin and wMsgFilterMax are both zero, 
		 GetMessage returns all available messages 
		 (that is, no range filtering is performed).
    */
   while(Ret = GetMessage(&msg, NULL, 0, 0))
   {
      if (Ret == -1)
      {
         printf("\nGetMessage() failed with error %d\n", GetLastError());
         return 1;
      }
      else
        printf("\nGetMessage() is pretty fine!\n");
 
      printf("Translating a message...\n");

	  /* 
	   * Translates virtual-key messages into character messages. 
	     The character messages are posted to the calling thread's message queue, 
		 to be read the next time the thread calls the GetMessage or PeekMessage function.
	   */
      TranslateMessage(&msg);
      printf("Dispatching a message...\n");
	  /* Dispatches a message to a window procedure. 
	   * It is typically used to dispatch 
	   * a message retrieved by the GetMessage function. 
	   */
      DispatchMessage(&msg);
   }
}
 