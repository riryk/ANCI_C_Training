
#include "winsocket.h"

void StartMain()
{
	SOCKADDR_IN internetAddr;
	int nPortId = 5150;
	WSADATA wsaData;
	int Ret;
	//Initialize Winsock version 2.2
	if ((Ret = WSAStartup(MAKEWORD(2,2), &wsaData)) != 0)
	{
        printf("WSAStartup failed with error %d\n", Ret);
	    return;
	}
	//Setup Winsock communication code here
    internetAddr.sin_family = AF_INET;
    //Convert the proposed dotted Internet address 136.149.3.29
	//to a four-byte integer, and assign it to sin_addr
    internetAddr.sin_addr.s_addr = inet_addr("136.149.3.29");
    internetAddr.sin_port = htons(nPortId);
	//When your application is finished call WSACleanup
	if (WSACleanup() == SOCKET_ERROR)
	{
		printf("WSAStartup failed with error %d\n", WSAGetLastError());
	}
}