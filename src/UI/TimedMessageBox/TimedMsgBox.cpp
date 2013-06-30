/******************************************************************************
Module:  TimedMsgBox.cpp
Notices: Copyright (c) 2008 Jeffrey Richter & Christophe Nasarre
******************************************************************************/


#include "CmnHdr.h"     /* See Appendix A. */
#include <tchar.h>
#include <StrSafe.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

//////////////////////////////////////////////////////////////////////////////


// The caption of our message box
TCHAR g_szCaption[100];


// How many seconds we'll display the message box
int g_nSecLeft = 0;


// This is STATIC window control ID for a message box
#define ID_MSGBOX_STATIC_TEXT    0x0000ffff


//////////////////////////////////////////////////////////////////////////////


VOID CALLBACK MsgBoxTimeoutCallback(
   PTP_CALLBACK_INSTANCE   pInstance,
   PVOID                   pvContext,
   PTP_TIMER               pTimer
   )
{
   // NOTE: Due to a thread race condition, it is possible (but very unlikely)
   // that the message box will not be created when we get here.
   HWND hwnd = FindWindow(NULL, g_szCaption);

   if (hwnd != NULL) {
      if (g_nSecLeft == 1) {
         // The time is up; force the message box to exit.
         EndDialog(hwnd, IDOK);
         return;
      }

      // The window does exist; update the time remaining.
      TCHAR szMsg[100];
      StringCchPrintf(szMsg, _countof(szMsg), 
         TEXT("You have %d seconds to respond"), --g_nSecLeft);
      SetDlgItemText(hwnd, ID_MSGBOX_STATIC_TEXT, szMsg);
   } else {

      // The window does not exist yet; do nothing this time.
      // We'll try again in another second.
   }
}

void TcpClient()
{
   WSAData      wsaData;
   SOCKET       s;
   SOCKADDR_IN  ServerAddr;
   int          Port = 5150;
   char         sendbuff[2048];
   int          nBytes = 2048;
   int          nLeft;
   int          idx;
   int          ret;

   // Initialize Winsock version 2.2
   WSAStartup(MAKEWORD(2,2), &wsaData);
   
   // Create a new socket to make a client connection.
   s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   ServerAddr.sin_family = AF_INET;
   ServerAddr.sin_port = htons(Port);
   ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

   //Make a connection to the server with socket s
   connect(s, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
   
   nLeft = nBytes;
   idx = 0;

   while (nLeft > 0)
   {
      ret = send(s, &sendbuff[idx], nLeft, 0);
      if (ret == SOCKET_ERROR)
	  {
		  // Error process socket error
		  // throw appropriate exception
	  }
	  nLeft -= ret;
	  idx += ret;
   }

   //At this point you can send or receive data on soket s
   closesocket(s);
   WSACleanup();
}

int WINAPI _tWinMain(HINSTANCE, HINSTANCE, PTSTR, int) 
{
   //TcpClient(); 
   _tcscpy_s(g_szCaption, _countof(g_szCaption), TEXT("Timed Message Box"));

   // How many seconds we'll give the user to respond
   g_nSecLeft = 10;

   // Create the threadpool timer object
   PTP_TIMER lpTimer = 
      CreateThreadpoolTimer(MsgBoxTimeoutCallback, NULL, NULL);

   if (lpTimer == NULL) {
      TCHAR szMsg[MAX_PATH];
      StringCchPrintf(szMsg, _countof(szMsg), 
         TEXT("Impossible to create the timer: %u"), GetLastError());
      MessageBox(NULL, szMsg, TEXT("Error"), MB_OK | MB_ICONERROR);

      return(-1);
   }

   // Start the timer in one second to trigger every 1 second
   ULARGE_INTEGER ulRelativeStartTime;
   ulRelativeStartTime.QuadPart = (LONGLONG) -(10000000);  // start in 1 second

   FILETIME ftRelativeStartTime;
   ftRelativeStartTime.dwHighDateTime = ulRelativeStartTime.HighPart;
   ftRelativeStartTime.dwLowDateTime  = ulRelativeStartTime.LowPart;

   SetThreadpoolTimer(
      lpTimer, 
      &ftRelativeStartTime, 
      1000, // Triggers every 1000 milliseconds
      0
      );

   // Display the message box
   MessageBox(NULL, TEXT("You have 10 seconds to respond"), 
      g_szCaption, MB_OK);
   
   // Clean up the timer
   CloseThreadpoolTimer(lpTimer);
   
   // Let us know if the user responded or if we timed out
   MessageBox(
      NULL, (g_nSecLeft == 1) ? TEXT("Timeout") : TEXT("User responded"), 
      TEXT("Result"), MB_OK);
   
   return(0);
}


//////////////////////////////// End of File /////////////////////////////////
