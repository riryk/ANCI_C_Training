
#include "winsocketclient.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

void TcpClient()
{
   WSADATA      wsaData;
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