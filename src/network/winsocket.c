
#include "winsocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <rpc.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Rpcrt4.lib")

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
    tcpaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	bind(s, (SOCKADDR*)&tcpaddr, sizeof(tcpaddr));
}

// A sample of the select() return value
int recvTimeOutTCP(SOCKET socket, long sec, long usec)
{
    // Setup timeval variable
    struct timeval timeout;
    struct fd_set fds;
 
     // assign the second and microsecond variables
    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    // Setup fd_set structure
    FD_ZERO(&fds);
    FD_SET(socket, &fds);
    // Possible return values:
    // -1: error occurred
    // 0: timed out
    // > 0: data ready to be read
    return select(0, &fds, 0, 0, &timeout);
}

int TcpServer()
{
	WSADATA        wsaData;
	SOCKET         ListeningSocket;
	SOCKET         NewConnection;
	SOCKADDR_IN    ServerAddr, SenderInfo;
    SOCKADDR_IN    ClientAddr;
    int            ClientAddrLen;
	int            Port = 5150;
	char           recvbuff[1024];
	int            ret;
	int            nLeft, nlen;
	int            idx, i;
	int            ByteReceived, SelectTiming;

    // Initialize Winsock version 2.2
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
	{
        // The WSAGetLastError() function is one of the only functions
        // in the Winsock 2.2 DLL that can be called in the case of a WSAStartup failure
        printf("Server: WSAStartup failed with error %ld.\n", WSAGetLastError());
        // Exit with error
        return 1;
	}
	else
	{
        printf("Server: The Winsock DLL found!\n");
        printf("Server: The current status is %s.\n", wsaData.szSystemStatus);
	}

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2 )
    {
        //Tell the user that we could not find a usable WinSock DLL
        printf("Server: The dll do not support the Winsock version %u.%u!\n",
                    LOBYTE(wsaData.wVersion),HIBYTE(wsaData.wVersion));
        // Do the clean up
        WSACleanup();
        // and exit with error
        return 1;
    }
    else
    {
        printf("Server: The dll supports the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion),
                    HIBYTE(wsaData.wVersion));
        printf("Server: The highest version this dll can support is %u.%u\n",
                    LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));
    }

	ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Check for errors to ensure that the socket is a valid socket.
    if (ListeningSocket == INVALID_SOCKET)
    {
        printf("Server: Error at socket(), error code: %ld.\n", WSAGetLastError());
        // Clean up
        WSACleanup();
        // and exit with error
        return 1;
    }
    else
        printf("Server: socket() is OK!\n");

	// Set up a SOCKADDR_IN structure 
	// Set up a SOCKADDR_IN structure that will tell bind that we
    // want to listen for connections on all interfaces using port 7171.
    // The IPv4 family
    ServerAddr.sin_family = AF_INET;
    // host-to-network byte order
    ServerAddr.sin_port = htons(Port);
    // Listen on all interface, host-to-network byte order
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Associate the address information with the socket using bind.
	// Associate the address information with the socket using bind.
    // Call the bind function, passing the created socket and the sockaddr_in
    // structure as parameters. Check for general errors.
	if (bind(ListeningSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
        printf("Server: bind() failed! Error code: %ld.\n", WSAGetLastError());
        // Close the socket
        closesocket(ListeningSocket);
        // Do the clean up
        WSACleanup();
        // and exit with error
        return 1;
	}
	else
        printf("Server: bind() is OK!\n");

	// Listen for client connections. We used a backlog of 5, which
	// is normal for many applications.
	// Listen for client connections with a backlog of 5
	if (listen(ListeningSocket, 5) == SOCKET_ERROR)
	{
        printf("Server: listen(): Error listening on socket %ld.\n", WSAGetLastError());
        // Close the socket
        closesocket(ListeningSocket);
        // Do the clean up
        WSACleanup();
        // Exit with error
        return 1;
	}
    else
        printf("Server: listen() is OK, I'm listening for connections...\n");

    // Set 10 seconds 10 useconds timeout
    SelectTiming = recvTimeOutTCP(ListeningSocket, 10000, 10000);
 
    switch (SelectTiming)
    {
        case 0:
           // Timed out, do whatever you want to handle this situation
           printf("\nServer: Timeout lor while waiting you retard client!...\n");
           break;
        case -1:
           // Error occurred, more tweaking here and the recvTimeOutTCP()...
           printf("\nServer: Some error encountered with code number: %ld\n", WSAGetLastError());
           break;
        default:
        {
           // Accept a new connection when available. 'while' always true
           while(1)
           {
			   // Reset the NewConnection socket to SOCKET_ERROR
               // Take note that the NewConnection socket in not listening
               NewConnection = SOCKET_ERROR;
			   // While the NewConnection socket equal to SOCKET_ERROR
               // which is always true in this case...
			   while (NewConnection == SOCKET_ERROR)
               {
                  // Accept a new connection when one arrives
				  // Accept connection on the ListeningSocket socket and assign
                  // it to the NewConnection socket, let the ListeningSocket
                  // do the listening for more connection
	              NewConnection = accept(ListeningSocket, NULL, NULL);
                  printf("\nServer: accept() is OK...\n");
                  printf("Server: New client got connected, ready to receive and send data...\n");
                  
	              // At this point you can do two things with these sockets
                  // 1. Wait for more connections by calling accept again
                  //    on ListeningSocket (loop)
                  // 2. Start sending or receiving data on NewConnection.

				  
			      memset(&recvbuff, 0, sizeof(recvbuff));
                  ByteReceived = recv(NewConnection, recvbuff, sizeof(recvbuff), 0);
			    
                  if (ByteReceived > 0)
				  {
                      printf("Server: recv() looks fine....\n");

                      // Some info on the receiver side...
                      getsockname(ListeningSocket, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));
                      printf("Server: Receiving IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
                      printf("Server: Receiving port used: %d\n", htons(ServerAddr.sin_port));
 
                      // Some info on the sender side
                      // Allocate the required resources
                      memset(&SenderInfo, 0, sizeof(SenderInfo));
                      nlen = sizeof(SenderInfo);
 
                      getpeername(NewConnection, (SOCKADDR *)&SenderInfo, &nlen);
                      printf("Server: Sending IP used: %s\n", inet_ntoa(SenderInfo.sin_addr));
                      printf("Server: Sending port used: %d\n", htons(SenderInfo.sin_port));
 
                      // Print the received bytes. Take note that this is the total
                      // byte received, it is not the size of the declared buffer
                      printf("Server: Bytes received: %d\n", ByteReceived);
                      // Print what those bytes represent
                      printf("Server: Those bytes are: \"");
                      // Print the string only, discard other
                      // remaining 'rubbish' in the 1024 buffer size
                      for(i=0;i < ByteReceived;i++)
                         printf("%c", recvbuff[i]);
                      printf("\"");
				  }
                  // No data
				  else if (ByteReceived == 0)
                      printf("Server: Connection closed!\n");
                  // Others
				  else
                      printf("Server: recv() failed with error code: %d\n", WSAGetLastError());
			   }
               
               // Clean up all the send/recv communication, get ready for new one
               if (shutdown(NewConnection, SD_SEND) != 0)
                   printf("\nServer: Well, there is something wrong with the shutdown(). The error code: %ld\n", WSAGetLastError());
               else
                   printf("\nServer: shutdown() looks OK...\n");
 
               // Well, if there is no more connection in 15 seconds,
               // just exit this listening loop...
               if (recvTimeOutTCP(ListeningSocket, 10000, 10000) == 0)
                   break;
		   }
		}
	}

	printf("\nServer: The listening socket is timeout...\n");
    // When all the data communication and listening finished, close the socket
    if(closesocket(ListeningSocket) != 0)
        printf("Server: Cannot close \"ListeningSocket\" socket. Error code: %ld\n", WSAGetLastError());
    else
        printf("Server: Closing \"ListeningSocket\" socket...\n");
 
    // Finally and optionally, clean up all those WSA setup
    if(WSACleanup() != 0)
        printf("Server: WSACleanup() failed! Error code: %ld\n", WSAGetLastError());
    else
        printf("Server: WSACleanup() is OK...\n");
 
    return 0;
}

int recvfromTimeOutUDP(SOCKET socket, long sec, long usec)
{
    // Setup timeval variable
    struct timeval timeout;
    struct fd_set fds;

    timeout.tv_sec = sec;
    timeout.tv_usec = usec;
    // Setup fd_set structure
    FD_ZERO(&fds);
    FD_SET(socket, &fds);
    // Return value:
    // -1: error occurred
    // 0: timed out
    // > 0: data ready to be read
    return select(0, &fds, 0, 0, &timeout);
}

void UdpReceiver()
{
    WSADATA            wsaData;
    SOCKET             ReceivingSocket;
    SOCKADDR_IN        ReceiverAddr;
    int                Port = 5150;
    char               ReceiveBuf[1024];
    int                BufLength = 1024;
    SOCKADDR_IN        SenderAddr;
    int                SenderAddrSize = sizeof(SenderAddr);
    int                ByteReceived = 5, SelectTiming, ErrorCode;
    char               ch = 'Y';
 
    // Initialize Winsock version 2.2
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
       printf("Server: WSAStartup failed with error %ld\n", WSAGetLastError());
       return -1;
    }
    else
       printf("Server: The Winsock DLL status is %s.\n", wsaData.szSystemStatus);   

	// Create a new socket to receive datagrams on.
    ReceivingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
 
    if (ReceivingSocket == INVALID_SOCKET)
    {
       printf("Server: Error at socket(): %ld\n", WSAGetLastError());
       // Clean up
       WSACleanup();
       // Exit with error
       return -1;
    }
    else
       printf("Server: socket() is OK!\n");
    
    // Set up a SOCKADDR_IN structure that will tell bind that we
    // want to receive datagrams from all interfaces using port 5150.
 
    // The IPv4 family
    ReceiverAddr.sin_family = AF_INET;
    // Port no. 5150
    ReceiverAddr.sin_port = htons(Port);
    // From all interface (0.0.0.0)
    ReceiverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
    // Associate the address information with the socket using bind.
    // At this point you can receive datagrams on your bound socket.
    if (bind(ReceivingSocket, (SOCKADDR *)&ReceiverAddr, sizeof(ReceiverAddr)) == SOCKET_ERROR)
    {
        printf("Server: bind() failed! Error: %ld.\n", WSAGetLastError());
        // Close the socket
        closesocket(ReceivingSocket);
        // Do the clean up
        WSACleanup();
        // and exit with error
        return -1;
    }
    else
        printf("Server: bind() is OK!\n");   
    
    // Some info on the receiver side...
    getsockname(ReceivingSocket, (SOCKADDR *)&ReceiverAddr, (int *)sizeof(ReceiverAddr));
 
    printf("Server: Receiving IP(s) used: %s\n", inet_ntoa(ReceiverAddr.sin_addr));
    printf("Server: Receiving port used: %d\n", htons(ReceiverAddr.sin_port));
    printf("Server: I\'m ready to receive a datagram...\n");
 
    SelectTiming = recvfromTimeOutUDP(ReceivingSocket, 1000, 1000);

	switch (SelectTiming)
    {
       case 0:
         // Timed out, do whatever you want to handle this situation
         printf("Server: Timeout lor while waiting you bastard client!...\n");
         break;
       case -1:
         // Error occurred, maybe we should display an error message?
         // Need more tweaking here and the recvfromTimeOutUDP()...
         printf("Server: Some error encountered with code number: %ld\n", WSAGetLastError());
         break;
       default:
       {
         while (1)
         {
            // Call recvfrom() to get it then display the received data...
            ByteReceived = recvfrom(ReceivingSocket, ReceiveBuf, BufLength, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
            if (ByteReceived > 0)
            {
                printf("Server: Total Bytes received: %d\n", ByteReceived);
                printf("Server: The data is \"%s\"\n", ReceiveBuf);
            }
            else if ( ByteReceived <= 0 )
                printf("Server: Connection closed with error code: %ld\n", WSAGetLastError());
            else
                printf("Server: recvfrom() failed with error code: %d\n", WSAGetLastError());

            // Some info on the sender side
            getpeername(ReceivingSocket, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
            printf("Server: Sending IP used: %s\n", inet_ntoa(SenderAddr.sin_addr));
            printf("Server: Sending port used: %d\n", htons(SenderAddr.sin_port));
         }
       }
    }

    // When your application is finished receiving datagrams close the socket.
    printf("Server: Finished receiving. Closing the listening socket...\n");
    if (closesocket(ReceivingSocket) != 0)
        printf("Server: closesocket() failed! Error code: %ld\n", WSAGetLastError());
    else
        printf("Server: closesocket() is OK...\n");
 
    // When your application is finished call WSACleanup.
    printf("Server: Cleaning up...\n");
    if(WSACleanup() != 0)
        printf("Server: WSACleanup() failed! Error code: %ld\n", WSAGetLastError());
    else
        printf("Server: WSACleanup() is OK\n");
    // Back to the system
    return 0;
}

// Print out the contents of a WSAPROTOCOL_INFO structure in
// a readable format.
void PrintBufContent(WSAPROTOCOL_INFO *pProtocolBuf)
{
      unsigned char *pszUuid;
      int i;
     
      printf("\nProtocol <%s>\n", pProtocolBuf->szProtocol);
      // A guid is the same as a uuid.
      UuidToString(&pProtocolBuf->ProviderId, (RPC_WSTR *)&pszUuid);
      printf("  ProviderId {%s}\n", pszUuid);
      RpcStringFree((RPC_WSTR *)&pszUuid);
     
      if (!pProtocolBuf->dwServiceFlags1)
            printf("  dwServiceFlags1: 0\n");
      else
            printf("  dwServiceFlags1: 0x%08X\n",pProtocolBuf->dwServiceFlags1);
     
      // Check which bit flags are set.
      printf("\nChecking the set bit flags...\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_CONNECTIONLESS)
            printf("    XP1_CONNECTIONLESS\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_GUARANTEED_DELIVERY)
            printf("    XP1_GUARANTEED_DELIVERY\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_GUARANTEED_ORDER)
            printf("    XP1_GUARANTEED_ORDER\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_MESSAGE_ORIENTED)
            printf("    XP1_MESSAGE_ORIENTED\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_PSEUDO_STREAM)
            printf("    XP1_PSEUDO_STREAM\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_GRACEFUL_CLOSE)
            printf("    XP1_GRACEFUL_CLOSE\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_EXPEDITED_DATA)
            printf("    XP1_EXPEDITED_DATA\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_CONNECT_DATA)
            printf("    XP1_CONNECT_DATA\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_DISCONNECT_DATA)
            printf("    XP1_DISCONNECT_DATA\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_SUPPORT_BROADCAST)
            printf("    XP1_SUPPORT_BROADCAST\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_SUPPORT_MULTIPOINT)
            printf("    XP1_SUPPORT_MULTIPOINT\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_MULTIPOINT_CONTROL_PLANE)
            printf("    XP1_MULTIPOINT_CONTROL_PLANE\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_MULTIPOINT_DATA_PLANE)
            printf("    XP1_MULTIPOINT_DATA_PLANE\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_QOS_SUPPORTED)
            printf("    XP1_QOS_SUPPORTED\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_INTERRUPT)
            printf("    XP1_INTERRUPT\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_UNI_SEND)
            printf("    XP1_UNI_SEND\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_UNI_RECV)
            printf("    XP1_UNI_RECV\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_IFS_HANDLES)
            printf("    XP1_IFS_HANDLES\n");
      if (pProtocolBuf->dwServiceFlags1 & XP1_PARTIAL_MESSAGE)
            printf("    XP1_PARTIAL_MESSAGE\n");
 
      printf("  dwServiceFlags2: reserved\n");
      printf("  dwServiceFlags3: reserved\n");
      printf("  dwServiceFlags4: reserved\n");
 
      printf("\nChecking the Provider flags...\n");
      printf("  dwProviderFlags:\n");
      if (pProtocolBuf->dwProviderFlags & PFL_MULTIPLE_PROTO_ENTRIES)
            printf("    PFL_MULTIPLE_PROTO_ENTRIES\n");
      if (pProtocolBuf->dwProviderFlags & PFL_RECOMMENDED_PROTO_ENTRY)
            printf("    PFL_RECOMMENDED_PROTO_ENTRY\n");
      if (pProtocolBuf->dwProviderFlags & PFL_HIDDEN)
            printf("    PFL_HIDDEN\n");
      if (pProtocolBuf->dwProviderFlags & PFL_MATCHES_PROTOCOL_ZERO)
            printf("    PFL_MATCHES_PROTOCOL_ZERO\n");
 
      printf("\nMore digging...\n");
      printf("  dwCatalogEntryId = %u\n", pProtocolBuf->dwCatalogEntryId);
 
      printf("  ProtocolChain.ChainLen = %d ",  pProtocolBuf->ProtocolChain.ChainLen);
      if (pProtocolBuf->ProtocolChain.ChainLen == 1)
            printf("  (This is a base service provider)\n");
      else if (pProtocolBuf->ProtocolChain.ChainLen > 1)
      {
            printf("  (ProtocolChain layered to base protocol)\n");
            for (i=0; i<pProtocolBuf->ProtocolChain.ChainLen; i++)
            {
                  printf("    Chain Catalog Entry Id = %u\n", pProtocolBuf->ProtocolChain.ChainEntries[i]);
            }
      }
      else if (pProtocolBuf->ProtocolChain.ChainLen == 0)
            printf("  (This is a layered service provider)\n");
      else
            printf("  Invalid\n");
 
      printf("  iVersion = %d\n", pProtocolBuf->iVersion);
      printf("  iAddressFamily = %d\n", pProtocolBuf->iAddressFamily);
      printf("  iMaxSockAddr = %d\n", pProtocolBuf->iMaxSockAddr);
      printf("  iMinSockAddr = %d\n", pProtocolBuf->iMinSockAddr);
 
      // iProtocols returns a negative number for Microsoft NetBIOS
      // service providers corresponding to the lana number * -1 (for
      // example, -2 implies lana 2), except for lana 0 which is equal to
      // 0x80000000 because protocol 0 is reserved for special use.
      printf("  iProtocol = %d\n", pProtocolBuf->iProtocol);
      printf("  iProtocolMaxOffset = %d\n", pProtocolBuf->iProtocolMaxOffset);
      printf("  iNetworkByteOrder = %s\n", ((pProtocolBuf->iNetworkByteOrder == LITTLEENDIAN) ?
            "LittleEndian" : "BigEndian"));
      printf("  iSecurityScheme = %d\n", pProtocolBuf->iSecurityScheme);
      printf("  dwMessageSize = %u\n", pProtocolBuf->dwMessageSize);
      printf("  dwProviderReserved = reserved\n");
      return;
}

void EnumInstalledProtocols()
{
      WSADATA WSAData;
      int i, nRet;
      DWORD dwErr;
      WSAPROTOCOL_INFO *lpProtocolBuf = NULL;
      DWORD dwBufLen = 0;
 
      // The WSAStartup() function directly returns the extended error code
      // in the return value for this function. A call to the WSAGetLastError
      // function is not needed and should not be used
      if (WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
            printf("WSAStartup() error with code %d\n", WSAGetLastError());
      else
      {
            printf("WSAStartup() is OK!\n");
            // First, have WSAEnumProtocols tell you how big a buffer you need.
			/* The WSAEnumProtocols function retrieves information 
			 * about available transport protocols. */
            nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
           
            if (nRet != SOCKET_ERROR)
                  printf("WSAEnumProtocols() looks OK!\n");
            else if ((dwErr = WSAGetLastError()) != WSAENOBUFS)
                  // WSAEnumProtocols() failed for some reason not relating to buffer size
                  printf("WSAEnumProtocols() failed with code %d\n", WSAGetLastError());
            else
            {
                  // WSAEnumProtocols failed for the "expected" reason.
                  // Now you need to allocate a buffer that is the right size.
                  lpProtocolBuf = (WSAPROTOCOL_INFO *)malloc(dwBufLen);
                  if (lpProtocolBuf)
                  {
                        // Now you can call WSAEnumProtocols again with the
                        // expectation that it will succeed
                        // because you have allocated a big enough buffer.
                        nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
 
                        if (nRet == SOCKET_ERROR)
                              printf("WSAEnumProtocols() failed with code %d\n", WSAGetLastError());
                        else
                        {
                              // Enumerate the protocols.
                              printf("Enumerating the protocols...");
                              for (i=0; i<nRet; i++)
                              {
                                    PrintBufContent(&lpProtocolBuf[i]);
                                    printf("\nPress <Enter> for more info, if any...\n");
                                    _fgetchar();
                              }
                        }
                        // Free up the allocated resource
                        free(lpProtocolBuf);
                  }
            }
      }
      if(WSACleanup() != 0)
            printf("WSACleanup() failed!...\n");
      else
            printf("WSACleanup() is OK...\n");
      return 0;
}

void CreateWSASocket()
{
      WSADATA WSAData;
      int i, nRet;
      WSAPROTOCOL_INFO *lpProtocolBuf = NULL;
      DWORD dwBufLen = 0;
      SOCKET SocDescriptor;
      DWORD dwErr;
      BOOL bProtocolFound;
 
      if (WSAStartup(MAKEWORD(2,2), &WSAData) != 0)
            printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
      else
      {
            printf("WSAStartup() seems OK!\n");
 
            // First, have WSAEnumProtocols() tell you how big a buffer you need
            nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
 
            printf("The buffer size is %d bytes...\n", dwBufLen);
 
            // If failed, returns SOCKET_ERROR
            if (nRet != SOCKET_ERROR)
                  printf("WSAEnumProtocols() does not return SOCKET_ERROR!\n");
            // WSAENOBUFS - The buffer length was too small
            else if ((dwErr = WSAGetLastError()) != WSAENOBUFS)
                  // WSAEnumProtocols() failed for some reason not relating to buffer size
                  printf("WSAEnumProtocols() does not return WSAENOBUFS,\n"
                  "    however failed with error code %ld\n", WSAGetLastError());
            else
            {
                  // WSAEnumProtocols() failed for the "expected" reason. Therefore,
                  // you need to allocate a buffer of the appropriate size.
                  printf("Allocating the buffer of the appropriate size...\n");
                  lpProtocolBuf = (WSAPROTOCOL_INFO *)malloc(dwBufLen);
                  printf("Recheck, the buffer size is %d bytes...\n", dwBufLen);
                  if (lpProtocolBuf)
                  {
                        // Now you can call WSAEnumProtocols() again with the expectation
                        // that it will succeed because you have allocated a big enough buffer.
                        nRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
                        if (nRet == SOCKET_ERROR)
                              printf("WSAEnumProtocols() failed with error code %d\n", WSAGetLastError());
                        else
                        {
                              printf("WSAEnumProtocols() looks pretty fine!\n");
                              // Loop through protocols, looking for the first service
                              // provider that meets the matching criteria.
                              // Originally FALSE, if found change to TRUE
                              printf("Matching the specified service provider...\n");
                              printf("    IPPROTO_TCP and XP1_QOS_SUPPORTED...\n");
                              bProtocolFound = FALSE;
                              for (i=0; i<nRet; i++)
                              {
                                    if ((lpProtocolBuf[i].iProtocol == IPPROTO_TCP) &&
                                          ((XP1_QOS_SUPPORTED & lpProtocolBuf[i].dwServiceFlags1) ==
                                                    XP1_QOS_SUPPORTED))
                                    {
                                          bProtocolFound = TRUE;
                                          // first service provider matched, exit the loop
                                          break;
                                    }
                              }
                              printf("The specified service provider found/matched! buffer index, i is
                                        %d...\n", i);
 
                              // Once you have chosen a service provider, the following sample code
                              // demonstrates how to actually create a socket using a supplied
                              // WSAPROTOCOL_INFO structure
                              // from the service provider you have chosen
 
							  /* The WSASocket function creates a socket 
							   * that is bound to a specific transport-service provider. 
							   *
                               * The  following  may  be used in place of the address family, socket type, or
                               * protocol  in  a  call  to WSASocket to indicate that the corresponding value
                               * should  be taken from the supplied WSAPROTOCOL_INFO structure instead of the
                               * parameter itself.
                               */
                              SocDescriptor = WSASocket(
                                    FROM_PROTOCOL_INFO, // manifest constant
								    FROM_PROTOCOL_INFO, // manifest constant
                                    FROM_PROTOCOL_INFO, // manifest constant
                                    &lpProtocolBuf[i], // from WSAPROTOCOL_INFO
                                    0, // reserved
                                    WSA_FLAG_OVERLAPPED);
 
                              printf("Reconfirm the buffer index, i is %d\n", i);
                              if (SocDescriptor == INVALID_SOCKET)
                              {
                                    printf("WSASocket() failed with error code %d\n", WSAGetLastError());
                                    WSACleanup();
                              }
                              else
                                    printf("WSASocket() looks fine, continue on next tasks!\n");
                        }
                  }
            }
      }
      // If not needed anymore, deallocate memory to the system's heap
      free(lpProtocolBuf);
      // WSA cleanup
      if(WSACleanup() != 0)
            printf("WSACleanup() failed!...\n");
      else
            printf("WSACleanup() is OK...\n");
      return 0;
}