
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

void BindSocket()
{
	SOCKET       s;
    SOCKADDR_IN  tcpaddr;
	int          port = 5150;

	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    tcpaddr.sin_family = AF_INET;
    tcpaddr.sin_port = htons(port);
    tcpaddr.sin_addr.s_addr = hton1("136.149.3.29");

	bind(s, (SOCKADDR*)&tcpaddr, sizeof(tcpaddr));
}

void TcpServer()
{
	WSDATA        wsaData;
	SOCKET        ListeningSocket;
	SOCKET        NewConnection;
	SOCKADDR_IN   ServerAddr;
    SOCKADDR_IN   ClientAddr;
    int           ClientAddrLen;
	int           Port = 5150;

    // Initialize Winsock version 2.2
	WSAStartup(MAKEWORD(2,2), &wsaData);
	ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Set up a SOCKADDR_IN structure 
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(Port);
	ServerAddr.sin_addr.s_addr = hton1(INADDR_ANY);

	// Associate the address information with the socket using bind.
	bind(ListeningSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));

	// Listen for client connections. We used a backlog of 5, which
	// is normal for many applications.
	listen(ListeningSocket, 5);

	// Accept a new connection when one arrives
	NewConnection = accept(ListeningSocket, &ClientAddr, &ClientAddrLen);

	closesocket(NewConnection);
    closesocket(ListeningSocket);

    WSACleanup();
}