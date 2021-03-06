
#include "winsocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <rpc.h>
#include <af_irda.h>
#include <stdio.h>
#include <wsnetbs.h>
#include <winsock.h>
#include <atalkwsh.h>



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

// Some constants used by this program
#define SERVER_PORT     7171
#define BUFFER_LENGTH    256
#define FALSE              0

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
                              printf("The specified service provider found/matched! buffer index, i is %d...\n", i);
 
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

void TestNameResolutionFunctions()
{
      WSADATA                   wsaData;
	  SOCKET                    s;
	  int                       rc, iResult, RetCode;
      struct addrinfo*          result = NULL;
      struct addrinfo           hints;

	  // Initialize Winsock
	  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	  if (iResult != 0)
	  {
		  printf("Client: WSAStartup() failed with error code %d\n", iResult);
		  return 1;
	  }
	  else
		  printf("Client: WSAStartup() is OK...\n");

	  // Setup the hints address info structure
	  // which is passed to the getaddrinfo() function
	  ZeroMemory( &hints, sizeof(hints) );

	  hints.ai_flags = AI_CANONNAME;
	  hints.ai_family = AF_INET;
	  hints.ai_socktype = SOCK_STREAM;
	  hints.ai_protocol = IPPROTO_TCP;

	  // rc = getaddrinfo("206.190.60.37", "7171", &hints, &result);
	  rc = getaddrinfo("www.ipv6tf.org", "80", &hints, &result);

	  if (rc != 0)
	  {
		  printf("Client: getaddrinfo() failed with error code %ld\n", WSAGetLastError());
		  WSACleanup();
		  return 1;
	  }

	  printf("Client: getaddrinfo() looks OK!\n");

	  // Create a new socket to make a client connection.
	  s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	  if (s == INVALID_SOCKET)
	  {
		  printf("Client: socket() failed! Error code: %ld\n", WSAGetLastError());
		  // Do the clean up
		  WSACleanup();
		  // Exit with error
		  return 1;
	  }
	  else
		  printf("Client: socket() is OK!\n");

	  // Make a connection to the server with socket SendingSocket.
	  RetCode = connect(s, result->ai_addr, result->ai_addrlen);
	  if(RetCode != 0)
	  {
	      printf("Client: connect() failed! Error code: %ld\n", WSAGetLastError());
		  // Close the socket
		  closesocket(s);
		  // Do the clean up
		  WSACleanup();
		  // Exit with error
		  return 1;
      }
	  else
	  {
		  printf("Client: connect() is OK, got connected...\n");
		  printf("Client: Ready for sending and/or receiving data...\n");
	  }

      // Return the allocated resource to the heap
	  freeaddrinfo(result);
	  // When your application is finished handling the connection, call WSACleanup.
	  if(WSACleanup() != 0)
		  printf("Client: WSACleanup() failed!...\n");
	  else
		  printf("Client: WSACleanup() is OK...\n");

      return 0;      
}

int AI_NUMERICHOST_test(int argc, char **argv)
{
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;
    DWORD dwRetval;
    int i = 1;   
    struct addrinfo *result = NULL;
    struct addrinfo *ptr = NULL;
    struct addrinfo hints;
 
    // Validate the parameters
    if (argc != 2) {
        printf("Usage: %s <IP Address String>\n", argv[0]);
        printf("  getaddrinfo() determines the IP binary network address\n");
        printf("Example: %s 206.190.60.37\n", argv[0]);
        return 1;
    }
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %d\n", iResult);
        return 1;
    }
 
    printf("WSAStartup() looks OK!\n");
    // Setup the hints address info structure which is passed to the getaddrinfo() function
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_flags = AI_NUMERICHOST;
    hints.ai_family = AF_UNSPEC;
 
    // Call getaddrinfo(). If the call succeeds,
    // the result variable will hold a linked list
    // of addrinfo structures containing response information
    dwRetval = getaddrinfo(argv[1], NULL, &hints, &result);
    if ( dwRetval != 0 )
    {
        printf("getaddrinfo failed with error: %d\n", dwRetval);
        WSACleanup();
        return 1;
    }
 
    printf("getaddrinfo() returned success!\n");
    // Retrieve each address and print out the hex bytes
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next)
    {
        printf("getaddrinfo() response %d\n", i++);
        printf("\tFlags: 0x%x\n", ptr->ai_flags);
 
        printf("\tFamily: ");
        switch (ptr->ai_family)
        {
            case AF_UNSPEC:
                printf("Unspecified\n");
                break;
            case AF_INET:
                printf("AF_INET (IPv4)\n");
                break;
            case AF_INET6:
                printf("AF_INET6 (IPv6)\n");
                break;
            case AF_NETBIOS:
                printf("AF_NETBIOS (NetBIOS)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_family);
                break;
        }
 
        printf("\tSocket type: ");
        switch (ptr->ai_socktype)
        {
            case 0:
                printf("Unspecified\n");
                break;
            case SOCK_STREAM:
                printf("SOCK_STREAM (stream)\n");
                break;
            case SOCK_DGRAM:
                printf("SOCK_DGRAM (datagram) \n");
                break;
            case SOCK_RAW:
                printf("SOCK_RAW (raw) \n");
                break;
            case SOCK_RDM:
                printf("SOCK_RDM (reliable message datagram)\n");
                break;
            case SOCK_SEQPACKET:
                printf("SOCK_SEQPACKET (pseudo-stream packet)\n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_socktype);
                break;
        }
        printf("\tProtocol: ");
        switch (ptr->ai_protocol)
        {
            case 0:
                printf("Unspecified\n");
                break;
            case IPPROTO_TCP:
                printf("IPPROTO_TCP (TCP)\n");
                break;
            case IPPROTO_UDP:
                printf("IPPROTO_UDP (UDP) \n");
                break;
            default:
                printf("Other %ld\n", ptr->ai_protocol);
                break;
        }
        printf("\tLength of this sockaddr: %d\n", ptr->ai_addrlen);
        printf("\tCanonical name: %s\n", ptr->ai_canonname);
    }
    // Release the allocated resource
    freeaddrinfo(result);
    // WSA clean up
    WSACleanup();
    return 0;
}


int AI_NUMERICHOST_test1(int argc, char **argv)
{
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;
 
    DWORD dwRetval;
 
    struct sockaddr_in saGNI;
    char hostname[NI_MAXHOST]; // Set to the max value
    char servInfo[NI_MAXSERV]; // Set to the max value
    // Assume we plan to use TCP port 7777
    u_short port = 7777;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <IPv4 address>\n", argv[0]);
        printf("  This program return hostname\n");
        printf("Example: %s 127.0.0.1\n", argv[0]);
        return 1;
    }
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %d\n", iResult);
        return 1;
    }
 
    printf("WSAStartup() looks OK...\n");
    // Set up sockaddr_in structure which is passed to the getnameinfo function
    saGNI.sin_family = AF_INET; // AF_INET6
    saGNI.sin_addr.s_addr = inet_addr(argv[1]);
    saGNI.sin_port = htons(port);
 
    // Call getnameinfo()
    dwRetval = getnameinfo((struct sockaddr *) &saGNI,
                           sizeof (struct sockaddr),
                           hostname,
                           NI_MAXHOST, servInfo,
                           NI_MAXSERV, NI_NUMERICSERV);
 
    if (dwRetval != 0)
    {
        printf("getnameinfo() failed with error code %ld\n", WSAGetLastError());   
    }
    else
    {
        printf("getnameinfo() returned hostname \"%s\"\n", hostname);
    }
 
    // Terminate the use of Winsock
    WSACleanup();
    if (dwRetval != 0)
        return 1;
    else
        return 0; 
}

int AI_NUMERICHOST_test2(int argc, char **argv)
{
    WSADATA                wsaData;
    int                    iResult, RetVal, RetVal2;
    // SOCKADDR_IN  MyAddr;
    SOCKADDR_IN6           MyAddrValue;
    // Storage buffer, using LPWSTR pointer failed lor!
    // WCHAR  MyAddrString[256] = L"For IPv4...";
    WCHAR  MyAddrString[256] = L"Some dummy initializer";
    // Take note that if the address is in FQDN/Name/hostname
    // we need to convert it to a dotted IP address. For example:
    // WCHAR IPAddr[256] = L"localhost"; // this should fail lor!
    // WCHAR IPAddr[256] = L"209.131.36.158";
    WCHAR IPAddr[256] = L"2a01:48:1:0:2e0:81ff:fe05:4658";
 
    // These buffer sizes must be allocated properly else
    // the WSAStringToAddress() and WSAAddressToString() will
    // fail miserably!
    // int MyAddrSize = sizeof(struct sockaddr_storage);
    int MyAddrSize = sizeof(struct sockaddr_storage);
    // DWORD MyAddrSize2 = sizeof(struct sockaddr_storage);
    DWORD MyAddrSize2 = sizeof(struct sockaddr_storage);
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        // Exit with error
        return 1;
    }
    else
        printf("WSAStartup() is OK...\n");
 
    // Initialize the address family
    // MyAddrValue.sin_family = AF_INET;
    MyAddrValue.sin6_family = AF_INET6;
 
    // This part should convert the server/hostname/FQDN to a standard dotted IP address
    // string first (if any) using other helper functions such as getaddressinfo()
 
    //  ==============Convert address string to value using WSAStringToAddress()==============
    printf("\nWSAStringToAddress() - string to address value\n");
    RetVal = WSAStringToAddress(
                        IPAddr,                        // Pointer to address string to be converted
                        AF_INET6 /* AF_INET*/,         // address family
                        NULL,                          // Protocol info structure, not useable here
                        (LPSOCKADDR)&MyAddrValue,      // Socket address string buffer [out]
                        &MyAddrSize                    // Length of socket structure
                        );
 
    // Something wrong
    if (RetVal != 0)
    {
        printf("WSAStringToAddress() failed with error code %ld\n", WSAGetLastError());
        // Clean-up
        WSACleanup();
        // Exit with error
        return 1;
    }
    else
        printf("WSAStringToAddress() looks fine! Return value is %ld\n", RetVal);

    // printf("The IP Address part of the buffer filled with %ul\n", MyAddr.sin_addr);
    printf("The IP address value is %ul\n", MyAddrValue.sin6_addr);
    // and other info
    printf("The address family value is %ul\n", MyAddrValue.sin6_family);
    printf("The port value is %ul\n", MyAddrValue.sin6_port);
    printf("The scopeID is %ul\n", MyAddrValue.sin6_scope_id);

    // =========Do the re-conversion using WSAAddressToString()===============
    printf("\nWSAAddressToString() - address value to string\n");
    RetVal2 = WSAAddressToString(
                (LPSOCKADDR)&MyAddrValue,  // Pointer to address value to be converted
                MyAddrSize,              // Length of pointer to address value, in bytes
                NULL,                         // A pointer to the WSAPROTOCOL_INFO structure for a particular provider
                MyAddrString,           // A pointer to the buffer that receives the human-readable address string
                &MyAddrSize2          // length of the buffer pointed to by the MyAddrString parameter
                );

    // Something wrong
    if( RetVal2 != 0)
    {
           printf("WSAAddressToString() failed with error code %ld\n", WSAGetLastError());
           // Clean-up
           WSACleanup();
           // Exit with error
           return 1;
    }
    else
           printf("WSAAddressToString() looks fine! Return value is %ld\n", RetVal2);

    // printf("The IP Address is %ul\n", MyAddr.sin_addr);
    printf("The address string is %S\n", MyAddrString);
    // and other info
    printf("The address family is %ul\n", MyAddrValue.sin6_family);
    printf("The port value is %ul\n", MyAddrValue.sin6_port);
    printf("The scopeID is %ul\n", MyAddrValue.sin6_scope_id);

    // Call WSACleanup for WSA* cleanup
    if(WSACleanup() != 0)
        printf("\nWSACleanup() failed!...\n");
    else
        printf("\nWSACleanup() is OK...\n");

    return 0;   
}

int AI_NUMERICHOST_test3(int argc, char **argv)
{
    WSADATA          wsaData;
    SOCKADDR_IN      addr;
    char             AddrValue[256] = "216.239.61.104";
    char             AddrString[256]  = "Some dummy value";
    DWORD            dwSizeOfStr = sizeof(AddrValue);
    int              nSizeOfInput = sizeof(AddrString), iResult;
 
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        // Exit with error
        return 1;
    }
    else
        printf("WSAStartup() is OK...\n");
        // Initialize address family
        addr.sin_family = AF_INET;
 
        if(WSAStringToAddressA(AddrValue, AF_INET, NULL, (LPSOCKADDR)&addr, &nSizeOfInput) != 0)
        {
             printf("\nWSAStringToAddressA failed with error num %ld\n", WSAGetLastError());
             WSACleanup();
             return 1;
        }
        else
        {
             printf("\nAddress in value = %ul\n", addr.sin_addr);
        }

        if(WSAAddressToStringA((LPSOCKADDR)&addr, sizeof(addr), NULL, AddrString, &dwSizeOfStr) != 0)
        {
             printf("WSAAddressToStringA() failed miserably with error code %ld\n", WSAGetLastError());
             WSACleanup();
             return 1;
        }
        else
             printf("Address string = %s\n", AddrString);

        // Do the WSACleanup...
        if(WSACleanup() != 0)
             printf("\nWSACleanup() failed!...\n");
        else
             printf("\nWSACleanup() is OK...\n");
        return 0;
}

int AI_NUMERICHOST_test4(int argc, char **argv)
{
    WSADATA wsaData;
 
    struct addrinfo hints, *res, *p;
    int status;
    // Provide big enough buffer, ipv6 should be the biggest
    char ipstr[INET6_ADDRSTRLEN];
    char ipstr2[INET6_ADDRSTRLEN];

    if (argc != 2)
    {
        printf("Usage: %s <hostname>\n", argv[0]);
        printf("Example: %s www.yahoo.com\n", argv[0]);
        return 1;
    }

    // Initialization
    if (WSAStartup(MAKEWORD(1,1), &wsaData) != 0)
    {
        printf("WSAStartup() failed miserably! With error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() looks fine!\n");

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0)
    {
         printf("getaddrinfo() failed lor! with error code %ld\n", WSAGetLastError());
         WSACleanup();
         return 1;
    }

    printf("The IP addresses for %s:\n", argv[1]);

    for(p = res;p != NULL; p = p->ai_next)
    {
        void *addr;
        char *ipver;

        // Get the pointer to the address itself, different fields in IPv4 and IPv6
        if (p->ai_family == AF_INET)
        {
            // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        }
        else
        {
            // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // Convert the IP to a string and print it
        printf("String format: ");
        // InetNtop(address_family, IP_address_in_network_byte_to_convert_to_a_string,
        //         buffer_to_store_the_IP_address_string, the_IP_string_length_in_character);
		/* The InetNtop function converts an IPv4 or IPv6 
		 * Internet network address into a string in Internet standard format. 
		 * The ANSI version of this function is inet_ntop. */
        //inet_ntop(p->ai_family, addr, (PSTR)ipstr, sizeof(ipstr));
        printf(" %s: %s\n", ipver, ipstr);

        printf("Value format: ");
        // InetPton(address_family, string_to_be_converted, buffer_to_store_the_converted_string);
        printf(" %s: %ul\n", ipver, ipstr2);
    }

    // Deallocate the resource...
    freeaddrinfo(res);
    // Cleanup the WSA...
    WSACleanup();
    return 0;  
}

int AI_NUMERICHOST_test5(int argc, char **argv)
{
    // Declare and initialize variables
    WSADATA wsaData;
    int iResult;
    unsigned long ulAddr = INADDR_NONE;
    ULONG TempBuff = 0;
    struct sockaddr_in saSample;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <IPv4 address>\n", argv[0]);
        printf("  inet_addr() converts a string containing an\n");
        printf("  IPv4 address in one of the supported formats\n");
        printf("  to a unsigned long representing an IN_ADDR\n");
                        printf("Example: %s 209.131.36.158\n", argv[0]);
        return 1;
    }
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0)
    {
        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
 
    // Just for demonstration, call inet_addr(). If the call
    // succeeds, the result variable will hold a IN_ADDR structure
    ulAddr = inet_addr(argv[1]);
 
    if (ulAddr == INADDR_NONE)
    {
        printf("inet_addr() failed and returned INADDR_NONE\n");
        WSACleanup();
        return 1;
    }
 
    if (ulAddr == INADDR_ANY) 
	{
        printf("inet_addr() failed and returned INADDR_ANY\n");
        WSACleanup();
        return 1; 
    }
 
    printf("inet_addr() returned success!\n");
    // Retrieve address and print out the hex/dec/octal bytes
    printf("  Hex:\t\t0X%0X\n", ulAddr);
    printf("  Decimal:\t%ul\n", ulAddr);
    printf("  Octal:\t0%o\n", ulAddr);

    // Then, ready to be used
    saSample.sin_family = AF_INET;
    // In typical app. we can be directly use:
    // usesaSample.sin_addr.s_addr = inet_addr(argv[1]);
    saSample.sin_addr.s_addr = ulAddr;
    saSample.sin_port = htons(1234);

    // More codes here, create socket, bind, connect etc.

    // Revert the process using inet_ntoa() just for the address part

	/* The inet_ntoa function converts an (Ipv4) 
	 * Internet network address into an ASCII string 
	 * in Internet standard dotted-decimal format.
	 */
    if(inet_ntoa(saSample.sin_addr) != NULL)
    {
        printf("inet_ntoa() return success!\n");
        printf("The IP address is %s\n", inet_ntoa(saSample.sin_addr));
    }
    else
        printf("inet_ntoa() failed with error code %ld\n", WSAGetLastError());
 
    WSACleanup();
    return 0;
}

int AI_NUMERICHOST_test6(int argc, char **argv)
{
     // Declare and initialize variables
    WSADATA wsaData;
    DWORD dwError;
    int i = 0;
    struct hostent *remoteHost;
    char *host_name;
    struct in_addr addr;
    char **pAlias;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <ipv4address> or <hostname>\n", argv[0]);
        printf("Example: %s 127.0.0.1\n", argv[0]);
        printf("  to return the host\n");
        printf("Example: %s www.palerbutuh.com\n", argv[0]);
        printf("  to return the IP addresses for a host\n");
        return 1;
    }
 
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() looks fine!\n");
 
    host_name = argv[1];
    // If the user input is an alpha name for the host, use gethostbyname()
    // If not, gethostbyaddr()
    // We are assuming the IPv4 here
    if (isalpha(host_name[0]))    // host address is a name instead og IP
    {
        printf("Calling gethostbyname() with %s\n", host_name);
        remoteHost = gethostbyname(host_name);
    }
    else
    {
        // host address is an IP
        printf("Calling gethostbyaddr() with %s\n", host_name);
        addr.s_addr = inet_addr(host_name);
        if (addr.s_addr == INADDR_NONE)
        {
            printf("The IPv4 address entered must be a legal address!\n");
            WSACleanup();
            return 1;
        }
        else
        {
            // gethostbyaddr() is OK
            // Should provide both the Ipv6 and IPv4. Just IPv4 shown here
            remoteHost = gethostbyaddr((char *) &addr, 4, AF_INET);
        }
    }
 
    if (remoteHost == NULL)
    {
        dwError = WSAGetLastError();
        if (dwError != 0)
        {
            if (dwError == WSAHOST_NOT_FOUND)
            {
                        printf("Host not found!\n");
                        WSACleanup();
                        return 1;
            }
            else if (dwError == WSANO_DATA)
            {
                        printf("No data record found!\n");
                        WSACleanup();
                        return 1;
            }
            else
            {
                    printf("Function failed with error code %ld\n", dwError);
                    WSACleanup();
                    return 1;
            }
         }
    }
    else
    {
        printf("Function returned successfully!\n");
        printf("\tOfficial name: %s\n", remoteHost->h_name);
        // Extract all the cname/aliases if any
        for (pAlias = remoteHost->h_aliases; *pAlias != 0; pAlias++)
        {
            printf("\tAlternate name (if any) #%d: %s\n", ++i, *pAlias);
        }
 
        // Check the address family type
        printf("\tAddress type: ");
        switch (remoteHost->h_addrtype)
        {
        case AF_INET:
                    printf("AF_INET family\n");
                    break;
        case AF_INET6:
                    printf("AF_INET6 family\n");
                    break;
        case AF_NETBIOS:
                    printf("AF_NETBIOS - Windows NetBIOS\n");
                    break;
        default:
                    printf(" %d\n", remoteHost->h_addrtype);
                    break;
        }
        // Check the address family type
        printf("\tAddress length: %d\n", remoteHost->h_length);
        // Traverse the hostent address list and print the IP address(s)
        i = 0; // Reset counter
        while (remoteHost->h_addr_list[i] != 0)
        {
           addr.s_addr = *(u_long *)remoteHost->h_addr_list[i++];
           printf("\tIP Address #%d: %s\n", i, inet_ntoa(addr));
        }
    }
    WSACleanup();
    return 0;      
}

int AI_NUMERICHOST_test7(int argc, char **argv)
{
    // Declare and initialize variables
    WSADATA wsaData;
    char node_name[100] = "";
    struct hostent *hostptr;
    struct servent *servptr;
    // http://www.iana.org/assignments/port-numbers
    char services[12][10] = {"http", "ftp", "ssh", "telnet", "imap4", "smtp", "gopher", "finger", "pop3", "tftp", "mysql"};
    int i;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <host_name>\n", argv[0]);
        printf("Example: %s www.yahoo.com\n", argv[0]);
        // Just return with error
        return 1;
    }
 
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() looks fine!\n");
 
    strcpy_s(node_name, sizeof(node_name), argv[1]);
    // Here, name of server computer is expected, better to add
    // the server IP address functionality...
    hostptr = gethostbyname(node_name);

    if (hostptr == NULL)
    {
        printf("gethostbyname() failed with error code %ld\n", WSAGetLastError());
    }
    else
        printf("gethostbyname() is OK.\n Host name is %s\n", hostptr->h_name);

	for(i=0; i <= 10;i++)
	{
		// The proto pointer is NULL, the getservbyname() will returns
		// the first service entry where name matches the s_name member
		// of the servent struct or the s_aliases member of the servent struct.

		/* The getservbyname function retrieves service 
		 * information corresponding to a service name and protocol. */
		servptr = getservbyname(services[i], NULL);
       
		if (servptr == NULL)
			printf ("\ngetservbyname() - %s failed with error code %ld\n", services[i], WSAGetLastError());
		else
		{
			// Print the related info
			printf("\ngetservbyname() is OK - %s\n", services[i]);
			printf(" Service name is %s\n", servptr->s_name);
			// Need to cast to host-byte order
			printf(" Port number is %u\n", ntohs(servptr->s_port));
			printf(" Protocol is %s\n", servptr->s_proto);
		}
	}

	WSACleanup();
    return 0;  
}

int AI_NUMERICHOST_test8(int argc, char **argv)
{ 
    // Declare and initialize variables
    WSADATA wsaData;
    char node_name[100] = "";
    struct hostent *hostptr;
    struct servent *servptr;
    // http://www.iana.org/assignments/port-numbers
    int services[12] = {23, 80, 25, 21, 53, 69, 110, 143, 70, 7, 13};
    int i;
 
    // Validate the parameters
    if (argc != 2)
    {
        printf("Usage: %s <host_name>\n", argv[0]);
        printf("Example: %s www.yahoo.com\n", argv[0]);
        // Just return with error
        return 1;
    }
 
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() looks fine!\n");
 
    strcpy_s(node_name, sizeof(node_name), argv[1]);
    // Here, name of server computer is expected, it is better
    // to add the server IP address functionality...
    hostptr = gethostbyname(node_name);
 
    if (hostptr == NULL)
    {
        printf("gethostbyname() failed with error code %ld\n", WSAGetLastError());
    }
    else
        printf("gethostbyname() is OK.\n Host name is %s\n", hostptr->h_name);
 
    for(i=0; i <= 10;i++)
    {
        // If this is null, getservbyport returns the first service
        // entry for which the port matches the s_port of the servent structure.
		/* The getservbyport function retrieves service 
		 * information corresponding to a port and protocol. */
        servptr = getservbyport(ntohs(services[i]), NULL);

        if (servptr == NULL)
            printf ("\ngetservbyport() - %d failed with error code %ld\n", services[i], WSAGetLastError());
        else
        {
            // Print the related info
            printf("\ngetservbyport() is OK - %d\n", services[i]);
            printf(" Service name is %s\n", servptr->s_name);
            // Need to cast to host-byte order
            printf(" Port number is %u\n", ntohs(servptr->s_port));
            printf(" Protocol is %s\n", servptr->s_proto);
        }
    }
    WSACleanup();
    return 0;  
}

void UniversalServerIP6()
{
	// Declare and initialize variables
    WSADATA            wsaData;
    SOCKET             slisten[16], NewConnection;
    char*              szPort = "5150";
    struct addrinfo    hints, *res=NULL, *ptr=NULL;
    int                count=0, rc;
    char               recvbuff[1024];
    int                ByteReceived, i;
 
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Server: WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("Server: WSAStartup() looks fine!\n");
 
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    rc = getaddrinfo(NULL, szPort, &hints, &res);
    if (rc != 0)
    {
        printf("Server: getaddrinfo() failed with error code %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    else
        printf("Server: getaddrinfo() is OK...\n");

    ptr = res;

    while (ptr)
    {
        printf("\nServer: count value = %d\n", count);
        // Use the res struct info for listening...
        slisten[count] = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (slisten[count] == INVALID_SOCKET)
        {
            printf("Server: socket() failed with error code %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        else
            printf("Server: socket() is OK...\n");

        // The res struct info
        printf("\n The address family: %d\n", res->ai_family);
        printf(" The socket type: %d\n", res->ai_socktype);
        printf(" The protocol: %d\n\n", res->ai_protocol);

        // Then bind
        rc = bind(slisten[count], ptr->ai_addr, ptr->ai_addrlen);

        if (rc == SOCKET_ERROR)
        {
            printf("Server: bind() failed with error code %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        else
            printf("Server: bind() is OK...\n");

        // Next, listen
        rc = listen(slisten[count], 10);

        if (rc == SOCKET_ERROR)
        {
            printf("Server: listen() failed with error code %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        else
        {
            printf("Server: listen() is OK...\n");
            NewConnection = SOCKET_ERROR;
            // While the NewConnection socket equal to SOCKET_ERROR
            // which is always true in this case...
            while(NewConnection == SOCKET_ERROR)
            {
                // Accept connection on the slisten[count] socket and assign
                // it to the NewConnection socket, let the slisten[count]
                // do the listening for more connection
                NewConnection = accept(slisten[count], NULL, NULL);
                printf("Server: accept() is OK...\n");
                printf("Server: New client got connected, ready to receive and send data...\n");

                // Wait for more connections by calling accept again on ListeningSocket (loop)
                // or start sending or receiving data on NewConnection.
                ByteReceived = recv(NewConnection, recvbuff, sizeof(recvbuff), 0);

                // When there is problem
                if ( ByteReceived == SOCKET_ERROR )
                {
                    printf("Server: recv() failed with error code %ld\n", WSAGetLastError());
                    WSACleanup();
                    break;
                }
                else
                {
                    printf("Server: recv() is OK....\n");
                    // Print the received bytes. Take note that this is the total
                    // byte received, it is not the size of the declared buffer
                    printf("Server: Bytes received: %d\n", ByteReceived);
                    // Print what those bytes represent
                    printf("Server: Those bytes are: \"");
                    // Print the string only, discard other
                    // remaining 'rubbish' in the 1024 buffer size
                    for(i=0;i < ByteReceived;i++)
                        printf("%c", recvbuff[i]);
                    printf("\"\n");
                }
            }
        }

        if(res->ai_protocol == 6)
        {
            printf("Doing the TCP shutdown on the receiving part...\n");
            shutdown(slisten[count], SD_RECEIVE);
        }

        closesocket(slisten[count]);
        count++;
        ptr = ptr->ai_next;
    }

    freeaddrinfo(res);
    WSACleanup();
    return 0;
}

void UniversalServerIP61()
{
    // Variable and structure declarations
    WSADATA    wsaData;
    int sd = -1, RetVal, sdconn = -1;
    int rc, on = 1, j = 0, rcdsize=BUFFER_LENGTH;
    char buffer[BUFFER_LENGTH];
    struct sockaddr_in6 serveraddr, clientaddr;
    int addrlen = sizeof(struct sockaddr_storage);
    WCHAR  MyAddrString[256] = L"Some dummy initializer";
    int str = sizeof(struct sockaddr_storage);
    DWORD str2 = sizeof(struct sockaddr_storage);

    // Initialize the use of the Winsock DLL by a process
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        printf("Server: WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("Server: WSAStartup() looks fine!\n");

    // Using a loop for more 'efficient' code
    do
    {
        // The socket() function returns a socket descriptor, which represents
        // an endpoint.  Get a socket for address family AF_INET6 to
        // prepare to accept incoming connections on
        if ((sd = socket(AF_INET6, SOCK_STREAM, 0)) == INVALID_SOCKET)
        {
            printf("Server: socket() failed with code error %ld\n", WSAGetLastError());
            break;
        }
        else
            printf("Server: socket() looks good!\n");

        // The setsockopt() used to allow the local address to be reused when
        // the server is restarted before the required wait time expires
        if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == SOCKET_ERROR)
        {
            printf("Server: setsockopt(SO_REUSEADDR) failed with error code %ld\n", WSAGetLastError());
            break;
        }
        else
            printf("Server: setsockopt(SO_REUSEADDR) found OK!\n");

        // After the socket descriptor is created, a bind() function gets a
        // unique name for the socket.  In this example, the user sets the
        // address to in6addr_any, which (by default) allows connections to
        // be established from any IPv4 or IPv6 client based on the hostname
        // that specifies port 7171.
        // That is, the bind is done to both the IPv4 and IPv6 TCP/IP
        // stacks. However this sample program only accept the IPv4 hostname, then
        // the client must prepare to convert the IPv4 address to the hostname
        // before translating the IP string to network address before making a connection
        // using various Winsock API such as getaddressinfo() etc.
        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin6_family = AF_INET6;
        serveraddr.sin6_port   = htons(SERVER_PORT);

        // Applications use in6addr_any similarly to the way they use
        // INADDR_ANY in IPv4.
        serveraddr.sin6_addr   = in6addr_any;
        // The remaining fields in the sockaddr_in6 are currently not
        // supported and should be set to 0 to ensure upward compatibility
        if (bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == SOCKET_ERROR)
        {
            printf("Server: bind() failed with error code %ld\n", WSAGetLastError());
            break;
        }
        else
            printf("Server: bind() also looks fine!\n");

        // The listen() function allows the server to accept incoming
        // client connections.  In this example, the backlog is set to 7
        // This means that the system will queue 10 incoming connection
        // requests before the system starts rejecting the incoming requests
        if (listen(sd, 7) == SOCKET_ERROR)
        {
            printf("Server: listen() failed with error code %ld\n", WSAGetLastError());
            break;
        }
        printf("Server: listen() is OK and ready for client connect()...\n");

        // The server uses the accept() function to accept an incoming
        // connection request.  The accept() call will block indefinitely
        // waiting for the incoming connection to arrive from an IPv4 or IPv6 client
        if ((sdconn = accept(sd, NULL, NULL)) == INVALID_SOCKET)
        {
            printf("Server: accept() failed with error code %ld\n", WSAGetLastError());
            break;
        }
        else
        {
            printf("Server: accept() return success!\n");
            // Display the client address.  Note that if the client is
            // an IPv4 client, the address will be shown as an IPv4 Mapped IPv6 address
            if(getpeername(sdconn, (struct sockaddr *)&clientaddr, &addrlen) == SOCKET_ERROR)
            {
                printf("Server: getpeername() failed miserably with error code %ld\n", WSAGetLastError());
            }
            else
                printf("Server: getpeername() found OK!\n");

            // converts an IPv4 or IPv6 Internet network address into a string in Internet standard
            // format. The ANSI version of this function is inet_ntop() which is used in UNIX/BSD
            RetVal = WSAAddressToString((LPSOCKADDR)&clientaddr, addrlen, NULL, MyAddrString, &str);
            if(RetVal != 0)
                printf("Server: WSAAddressToString() failed with error code %ld\n", WSAGetLastError());
            else
            {
                printf("Server: Client address is %S\n", MyAddrString);
                printf("Server: Client port is %d\n", ntohs(clientaddr.sin6_port));
            }
        }

        // In this example we know that the client will send 256 bytes of
        // data over.  Knowing this, we can use the SO_RCVTIMEO socket
        // option and specify that we don't want our recv() to wake up
        // until all 256 bytes of data have arrived
        // http://msdn.microsoft.com/en-us/library/ms740476(VS.85).aspx
        // Take note that some parameters are Microsoft specific
        /*if (setsockopt(sdconn, SOL_SOCKET, SO_RCVTIMEO, (char *)&rcdsize, sizeof(rcdsize)) == SOCKET_ERROR)
        {
			printf("Server: setsockopt(SO_RCVTIMEO) failed with error code %ld\n", WSAGetLastError());
			break;
        }
        else
            printf("Server: setsockopt(SO_RCVTIMEO) looks good!\n");*/

        // Receive that 256 bytes of data from the client
        rc = recv(sdconn, buffer, sizeof(buffer), 0);
        if (rc == SOCKET_ERROR)
        {
            printf("Server: recv() failed with error code %ld\n", WSAGetLastError());
            break;
        }
        else
        {
            printf("Server: recv() is OK...\n");
        }

        printf("Server: %d bytes of data were received\n", rc);
        // Just for display!
        printf("Server: Received money is - ");
        for(j=0; buffer[j] == '$';j++)
            printf("%c",buffer[j]);
        printf("\n");

        /*if (rc == 0 || rc < sizeof(buffer))
        {
            printf("Server: The client closed the connection before sending all the data!\n");
            break;
        }
        else
        {*/
            printf("Server: All the data sent by client!\n");
        //}

        // Echo the data back to the client
        printf("Server: Echoing back the sent data to client...\n");
        rc = send(sdconn, buffer, sizeof(buffer), 0);
        if (rc == SOCKET_ERROR)
        {
            printf("Server: send() failed with error code %ld\n", WSAGetLastError());
            break;
        }
        else
            printf("Server: send() is OK...\n");
    } while (FALSE);

    // Program should complete here
    // Do the shutdown() for TCP so receive and send operation will no longer be allowed
    printf("Server: Ready to do the TCP shutdown()...\n");
    if(shutdown(sdconn, SD_BOTH) == SOCKET_ERROR)
       printf("Server: shutdown sdconn socket and the other end failed with error %ld\n", WSAGetLastError());
    else
       printf("Server: shutdown() on both end for sdconn socket looks OK...\n");

    // Close down any open socket descriptors,
    // BSD/UNIX uses close()
    if (closesocket(sd) == 0)
        printf("Server: sd socket was closed...\n");
    if (closesocket(sdconn) == 0)
        printf("Server: sdconn socket was closed...\n");

    // Terminates the use of the Ws2_32.DLL for all threads (if any)
    printf("Server: Doing the WSACleanup()...\n");
    if( WSACleanup() == SOCKET_ERROR)
        printf("Server: WSACleanup() failed with error code %ld\n", WSAGetLastError());
    else
        printf("Server: WSACleanup() confirmed OK!\n");
    return 0;
}

#define DEVICE_LIST_LEN    10

void GetListOfDiscoveredDevices()
{
	int            i;
	SOCKADDR_IRDA  DestSockAddr = { AF_IRDA, 0, 0, 0, 0, "SampleIrDAService" }; 
    unsigned char  DevListBuff[sizeof(DEVICELIST) - sizeof(IRDA_DEVICE_INFO) + (sizeof(IRDA_DEVICE_INFO) * DEVICE_LIST_LEN)];
    int            DevListLen   = sizeof(DevListBuff);
    PDEVICELIST    pDevList     = (PDEVICELIST) &DevListBuff;
    SOCKET         Sock;

    pDevList->numDevice = 0;
 
    // Sock is not in connected state
    if (getsockopt(Sock, SOL_IRLMP, IRLMP_ENUMDEVICES, (char *) pDevList, &DevListLen) == SOCKET_ERROR)
    {
        // WSAGetLastError
    }
 
    if (pDevList->numDevice == 0)
    {
        // no devices discovered or cached
        // not a bad idea to run a couple of times
    }
    else
    {
        // one per discovered device
		for (i = 0; i < (int)pDevList->numDevice; i++)
		{
			// typedef struct _IRDA_DEVICE_INFO
			// {
					//    u_char    irdaDeviceID[4];
					//    char    irdaDeviceName[22];
					//    u_char    irdaDeviceHints1;
					//     u_char    irdaDeviceHints2;
					//    u_char    irdaCharSet;
			// } _IRDA_DEVICE_INFO;
		 
			// pDevList->Device[i]. see _IRDA_DEVICE_INFO for fields
			// display the device names and let the user select one
		}
    }
 
    // assume the user selected the first device [0]
    memcpy(&DestSockAddr.irdaDeviceID[0], &pDevList->Device[0].irdaDeviceID[0], 4);
 
    //if (connect(Sock, (const struct sockaddr *) &DestSockAddr, sizeof(SOCKADDR_IRDA)) == SOCKET_ERROR)
    //{
        // WSAGetLastError()
    //}
}

// Change the service name accordingly
#define IR_SERVICE_NAME                     "MIKEBLUR"
#define MAX_BUFFER                          4096
 
void IrServer()
{
    WSADATA       wsaData;
    SOCKET        sock, sockClient;
    // irdaAddressFamily - Address family. 
	//                     This member is always AF_IRDA.
	// 
    // irdaDeviceID[4]   - Device identifier (ID) of the IrDA device 
	//                     to which the client wants to issue the connect function call. 
	//                     Ignored by server applications.
	//
    // irdaServiceName[25] - Well-known service name associated 
	//                     with a server application. Specified by servers during their bind function call.
	// 
    SOCKADDR_IRDA irAddr  = {AF_IRDA, 0, 0, 0, 0, "\0"}, remoteIrAddr;
    int           iIrSize = sizeof(SOCKADDR_IRDA);
    BOOL          bDone   = FALSE;
    int           ret;
    char          szRecvBuff[MAX_BUFFER];
 
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        printf("IRServer: WSAStartup() failed with error code %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("IRServer: WSAStartup() is OK!\n");
 
    if ((sock = socket(AF_IRDA, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("IRServer: socket() failed with error code %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    else
        printf("IRServer: socket() is OK!\n");
 
    printf("IRServer: Filling up the service name into the SOCKADDR_IRDA struct...\n");
    // Better to verify strcpy_s() returned value, not done here - EINVAL or ERANGE
    strcpy_s(irAddr.irdaServiceName, sizeof(irAddr.irdaServiceName), IR_SERVICE_NAME);
 
    // Binding our socket to the local service name
    printf("IRServer: Binding to the service name: %s\n", irAddr.irdaServiceName);
    if (bind(sock, (struct sockaddr *)&irAddr, sizeof(SOCKADDR_IRDA)) == SOCKET_ERROR)
    {
        printf("IRServer: bind() failed with error code %ld\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    else
        printf("IRServer: bind() looks pretty much OK!\n");
 
    if (listen(sock, 10) == SOCKET_ERROR)
    {
        printf("IRServer: listen() failed with error code %ld\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    else
        printf("IRServer: listen() is OK! I were bound and listening lol...\n");
 
    while (1)
    {
        if ((sockClient = accept(sock, (struct sockaddr *)&remoteIrAddr, &iIrSize)) == SOCKET_ERROR)  
        {
            printf("IRServer: accept failed with error %d\n", WSAGetLastError());
            break;
        }
        else
        {
            printf("IRServer: accept() is fine....\n");
            printf("Remote info: %s\n", remoteIrAddr.irdaDeviceID);
        }
 
        if ((ret = recv(sock, szRecvBuff, MAX_BUFFER, 0)) == SOCKET_ERROR)
        {
            printf("IRServer: recv() failed with error code %ld\n", WSAGetLastError());
            break;
        }
        else if (ret == 0)
        {
            printf("IRServer: connection has been gracefully closed, error code %ld\n", WSAGetLastError());
            break;
        }
        else
            printf("IRServer: Successfully received %d bytes\n", ret);
 
        // Echo data back to client
        printf("IRServer: Echoing the received data back to client...\n");
        if ((ret = send(sock, szRecvBuff, ret, 0)) == SOCKET_ERROR)
        {
            printf("IRServer: send() failed with error code %ld\n", WSAGetLastError());
            break;
        }
        else
        {
            printf("IRServer: send() is working!\n");
            printf("IRServer: Successfully sent %d bytes\n", ret);
        }
    }
 
    printf("IRServer: Closing connection...\n");
    if(closesocket(sock) == SOCKET_ERROR)
        printf("IRServer: Failed to close sock socket, error code %ld!\n", WSAGetLastError());
    else
        printf("IRServer: sock socket successfully closed!\n");

    if(WSACleanup() == SOCKET_ERROR)
        printf("IRServer: WSACleanup() failed with error code %ld\n", WSAGetLastError());
    else
        printf("IRServer: WSACleanup() is OK!\n");

    return 0;
}

void NBCheck(NCB x)
{
    // NRC_GOODRET == 0
    if (NRC_GOODRET != x.ncb_retcode)
    {
        printf("Line %d: Got 0x%x from NetBios()\n", __LINE__, x.ncb_retcode);
    }
}
 
BOOL NBReset (int nLana, int nSessions, int nNames)
{
    NCB ncb;

    memset (&ncb, 0, sizeof (ncb));
    ncb.ncb_command = NCBRESET;
    // If ncb_lsn is not 0x00, all resources associated with ncb_lana_num are to be freed
    // If ncb_lsn is 0x00, all resources associated with ncb_lana_num are to be freed,
    // and new resources are to be allocated. The ncb_callname[0] byte specifies the
    // maximum number of sessions, and the ncb_callname[2] byte specifies the maximum
    // number of names. A nonzero value for the ncb_callname[3] byte requests that the
    // application use NAME_NUMBER_1
    ncb.ncb_lsn = 0;            // So, allocate new lana_num resources
    ncb.ncb_lana_num = nLana;
    ncb.ncb_callname[0] = nSessions;  // Max number of sessions
    ncb.ncb_callname[2] = nNames;  // Max number of names
	/* The Netbios function interprets and executes 
	 * the specified network control block (NCB).
     * The Netbios function is provided primarily 
	 * for applications that were written for the NetBIOS interface 
	 * and need to be ported to Windows. */
    Netbios (&ncb);

    NBCheck (ncb);

    return (NRC_GOODRET == ncb.ncb_retcode);
}
 
BOOL NBAddName (int nLana, LPCWSTR szName)
{
    NCB ncb;

    memset(&ncb, 0, sizeof (ncb));
    ncb.ncb_command = NCBADDNAME;
    ncb.ncb_lana_num = nLana;

    MakeNetbiosName (ncb.ncb_name, szName);
    Netbios (&ncb);
    NBCheck (ncb);

    return (NRC_GOODRET == ncb.ncb_retcode);
}
 
// MakeNetbiosName - Builds a name padded with spaces up to
// the length of a NetBIOS name (NCBNAMSZ) whicis 16 bytes
void MakeNetbiosName (char *achDest, LPCWSTR szSrc)
{
    int cchSrc;

    cchSrc = lstrlen(szSrc);
    if (cchSrc > NCBNAMSZ)
        cchSrc = NCBNAMSZ;

    // Solving the trailing-space in 16-bytes storage if any
    // Firstly set all to ' ' char
    memset(achDest, ' ', NCBNAMSZ);
    // Then copy the string, leaving the ' ' chars
    memcpy(achDest, szSrc, cchSrc);
}
 
BOOL NBListNames (int nLana, LPCWSTR szName)
{
    int cbBuffer;
    ADAPTER_STATUS *pStatus;
    NAME_BUFFER *pNames;
    int i;

    // Allocate the largest buffer we might need
    cbBuffer = sizeof (ADAPTER_STATUS) + 255 * sizeof (NAME_BUFFER);
    pStatus = (ADAPTER_STATUS *) HeapAlloc (GetProcessHeap (), 0, cbBuffer);
    if (NULL == pStatus)
        return FALSE;

    // Verify the allocation
    if (!NBAdapterStatus(nLana, (PVOID) pStatus, cbBuffer, szName))
    {
        HeapFree(GetProcessHeap(), 0, pStatus);
        return FALSE;
    }
    else
        printf("NBAdapterStatus() heap allocation is OK!\n");

    // Then list the names immediately follows the adapter status structure.
    pNames = (NAME_BUFFER *) (pStatus + 1);
    printf("The NetBIOS names are: \n");
    for (i = 0; i < pStatus->name_count; i++)
        // NCBNAMSZ is 16 bytes lol! Minus the last byte (port)
        printf ("\t%.*s\n", NCBNAMSZ-1, pNames[i].name);
    // Deallocate the allocated heap
    HeapFree (GetProcessHeap (), 0, pStatus);
    return TRUE;
}
 
BOOL NBAdapterStatus (int nLana, PVOID pBuffer, int cbBuffer, LPCWSTR szName)
{
    NCB ncb;

    memset(&ncb, 0, sizeof (ncb));
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = nLana;
    ncb.ncb_buffer = (PUCHAR)pBuffer;
    ncb.ncb_length = cbBuffer;

    MakeNetbiosName (ncb.ncb_callname, szName);

    Netbios(&ncb);
    NBCheck(ncb);

    return (NRC_GOODRET == ncb.ncb_retcode);
}

void NBATest()
{
	if (!NBReset (LANANUM, 20, 30))
    {
        printf("NBReset() failed!\n");
        return -1;
    }
    else
        printf("NBReset() OK!\n");
   
    if (!NBAddName (LANANUM, (LPCWSTR)LOCALNAME))
    {
        printf("NBAddName() failed!\n");
        return -1;
    }
    else
        printf("NBAddName() OK!\n");
   
    if (!NBListNames (LANANUM, (LPCWSTR)LOCALNAME))
    {
        printf("NBListNames() failed!\n");
        return -1;
    }
    else
        printf("NBListNames() OK!\n");
   
    printf ("Tasks succeeded...\n");
    return 0;
}

typedef struct _ASTAT_
{
    ADAPTER_STATUS    adapt;
    NAME_BUFFER       NameBuff[30];
}ASTAT, *PASTAT;

void NBATest1()
{
    NCB ncb;
    UCHAR uRetCode;
    ASTAT Adapter;

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBRESET;
    // Provide valid LANA number of your machine. You may want to
    // use LANACFG tool to retrive your system LANA number
    ncb.ncb_lana_num = 3;

    uRetCode = Netbios(&ncb);

    if (uRetCode == 0)
        printf("NCBRESET is OK!\n");
    else
        printf("The NCBRESET return code is: 0x%x\n", uRetCode);

    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBASTAT;
    // Provide valid LANA number of your machine
    ncb.ncb_lana_num = 3;
   
    // Padds up to 16 bytes
    memcpy(&ncb.ncb_callname, "*               ", 16);
    ncb.ncb_buffer = (UCHAR*)&Adapter;
    ncb.ncb_length = sizeof(Adapter);

    uRetCode = Netbios(&ncb);
    if (uRetCode == 0)
        printf("NCBASTAT is OK!\n");
    else
        printf("The NCBASTAT failed with return code 0x%x \n", uRetCode);
 
    if (uRetCode == 0)
    {
        printf("The Ethernet MAC Address is: %02X-%02X-%02X-%02X-%02X-%02X\n",
                Adapter.adapt.adapter_address[0],
                Adapter.adapt.adapter_address[1],
                Adapter.adapt.adapter_address[2],
                Adapter.adapt.adapter_address[3],
                Adapter.adapt.adapter_address[4],
                Adapter.adapt.adapter_address[5]);
    }
}

/* ##########################  NBios server ############################ */

// Description:
//    This sample illustrates creating a Winsock server using
//    the AF_NETBIOS address family.  The server creates a listening
//    socket for each LANA number of the given socket type.
//    For datagrams, each listening socket waits for incoming data.
//For connection-oriented communication, the server waits
//    for client connections at which point a thread is created
//    to service the connection.
//
// Command Line Parameters/Options:  
//    NetbiosServer [-n str] [-p int] [-l int] [-t char] [-c int] [-b int]
//                                                          -n NAME        Our NetBIOS name
//                                                          -p PORT        The 16th byte qualifier of our name
//                                                          -l LANA        Specifies to listen on this LANA only
//                                                                                                 By default listen on all LANAs
//                                                          -t TYPE        Specifies datagram (d) or seqpacket (s)
//                                                          -c COUNT       Number of types to receive per client
//                                                          -b SIZE        Size of buffer to receive
// Link to ws2_32.lib

// Global variables for the name of our server and its 16th byte qualifier
#define       DEFAULT_COUNT 5

char          szServerName[NETBIOS_NAME_LENGTH];     // Our NetBIOS name
int           iPort,                                                                          // Our 16th byte
              iLana,                                                                                 // LANA to listen on
              iSocketType = SOCK_SEQPACKET;          // Socket type
DWORD         dwCount     = DEFAULT_COUNT,           // How many packets
              dwSize      = MAX_BUFFER;                                               // Receive buffer size
BOOL          bOneLana    = FALSE;                   // Listen on one lana only
 
// Function: usage
// Description: Print out usage information.

// Function: usage
// Description: Print out usage information.
int usage()
{
    printf("Usage: NetbiosServer -n [str] -p [int] -l [int] -t [char] -c [int] -b [int]\n");
    printf("       -n NAME        Our NetBIOS name\n");
    printf("       -p PORT        The 16th byte qualifier of our name\n");
    printf("       -l LANA        Specifies to listen on this LANA only\n");
    printf("                      By default listen on all LANAs\n");
    printf("       -t TYPE        Specifies datagram (d) or seqpacket (s)\n");
    printf("       -c COUNT       Number of types to receive per client\n");
    printf("       -b SIZE        Size of buffer to receive (make sure it is big enough!)\n");
    printf("Example: NetbiosServer -n MIKEBLUR -p 7172 -l 3 -t s -c 5 -b 64\n");
    printf("\n");
    return 0;
}

// Function: ValidateArgs
// Description
//    Parse the argument list for our NetBIOS name and whether
//    we want to operate on all LANAs or just one
int ValidateArgs(int argc, char **argv)
{
   int i;
 
   printf("Argument(s) supplied...\n");
   if (argc > 1)
   {
       for (i = 1; i < argc; i++)
       {
           if ( (argv[i][0] == '-') || (argv[i][0] == '/')  )
            {
                switch (tolower(argv[i][1]) )
               {
               case '?':
                   usage();
                   break;
                case 'n':   // use a unique name
                    if (i+1 < argc)
                    {
                         strncpy_s(szServerName, sizeof(szServerName), argv[i+1],NETBIOS_NAME_LENGTH);
                         if (0 != strlen(szServerName))
                                    ++i;
                    }
                   break;
                                                case 'p':   // set the 16th byte
                    if (i+1 < argc)
                   {
                      iPort = atoi(argv[i+1]);
                       ++i;
                    }
                   break;
                                                case 'l':   // listen on one lana only
                   if (i+1 < argc)
                   {
                       bOneLana = TRUE;
                       iLana = atoi(argv[i+1]);
                      ++i;
                   }
                  break;
                                                case 't':   // datagram or stream socket?
                   if (i+1 < argc)
                   {
                       if ('s' == tolower(argv[i+1][0]))
                                                                           iSocketType = SOCK_SEQPACKET;
                       else if ('d' == tolower(argv[i+1][0]))
                           iSocketType = SOCK_DGRAM;
                       else
                           usage();
                      ++i;
                   }
                   break;
                case 'c':   // number of messages to send
                  if (i+1 < argc)
                   {
                       dwCount = atol(argv[i+1]);
                       ++i;
                   }
                   break;
                case 'b':   // size of send buffer
                   if (i+1 < argc)
                   {
                       dwSize = (atol(argv[i+1]) > MAX_BUFFER ? MAX_BUFFER : atol(argv[i+1]));
                       ++i;
                   }
                   break;
                default:
                  usage();
                   break;
                                }
                           }
                        }
   }
   return 0;  
}

// Function: ClientThread
// Description:
//    This thread is spawned for each incoming client connection to
//    handle the echo server responsibilities
DWORD WINAPI ClientThread(LPVOID lpParam)
{
    SOCKET        sock = (SOCKET)lpParam;
    char          szRecvBuff[MAX_BUFFER];
    DWORD         dwRet, dwErr;
    unsigned long iOptVal = 1L;
 
    // Make the client connection non-blocking  
	/* The ioctlsocket function controls the I/O mode of a socket. 
	 * 
	 */
    if(ioctlsocket(sock, FIONBIO, &iOptVal) == SOCKET_ERROR)
    {
       printf("ioctlsocket(FIONBIO) failed with error code %d\n", WSAGetLastError());
       return 1;
    }
    else
       printf("ioctlsocket(FIONBIO) is OK!\n");
 
    // In a loop read and write the data from and to the client
    while (1)
    {
       dwRet = recv(sock, szRecvBuff, MAX_BUFFER, 0);

       if (dwRet == 0)
       {
           printf("recv() with graceful close...\n");
           return 0;
       }
       else if (dwRet == SOCKET_ERROR)
       {
            // If we get a WSAEWOULDBLOCK just keep going
            if ((dwErr = WSAGetLastError()) == WSAEWOULDBLOCK)
               continue;
            else if (dwErr == WSAECONNRESET)
            {
               printf("Client aborted the connection...\n");
               return 0;
            }
            else
            {
			   printf("recv() failed with error code %d\n", dwErr);
			   return 1;
            }
       }

       szRecvBuff[dwRet] = 0;
       printf("Read %d bytes...\n", dwRet);
       printf("Received money: %s\n", szRecvBuff);
      
       while (1)
       {
           dwRet = send(sock, szRecvBuff, dwRet, 0);
           if (dwRet == SOCKET_ERROR)
           {
               if ((dwErr = WSAGetLastError()) == WSAEWOULDBLOCK)
                   // Just continue
                   continue;
               else if (dwErr == WSAECONNRESET)
                   // Exit loop
                   break;
               printf("send() failed with error code %d\n", dwErr);
               return 1;
           }
           break;
       }
   }
   closesocket(sock);
   return 0;
}

extern int iSocketType;

// Function: FindProtocol
// Description:
//    Search through the available network service providers for
//    AF_NETBIOS compatible protocols. The number of providers
//    returned will be equal to 2 times the number of LANAs we
//    would have in NetBIOS. This is because there is two providers
//    for each LANA: one datagram and one session oriented provider.
BOOL FindProtocol(WSAPROTOCOL_INFO **wsapi, DWORD *dwCount)
{
    WSAPROTOCOL_INFO *lpProtocolBuf=NULL;
    DWORD            dwErr, dwRet, dwBufLen=0;
    DWORD              i;
 
    *dwCount = 0;
    if (SOCKET_ERROR != WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen))
    {
        // This should never happen as there is a NULL buffer
        printf("WSAEnumProtocols() failed!\n");
        return FALSE;
    }
    else if (WSAENOBUFS != (dwErr = WSAGetLastError()))
    {
         // We failed for some reason not relating to buffer size - also odd
         printf("WSAEnumProtocols() failed with error code %d\n", dwErr);
         return FALSE;
    }
    else
        printf("WSAEnumProtocols() is OK!\n");
 
    // Allocate the correct buffer size for WSAEnumProtocols as well as the buffer to return
    lpProtocolBuf = (WSAPROTOCOL_INFO *)GlobalAlloc(GMEM_FIXED, dwBufLen);
    *wsapi = (WSAPROTOCOL_INFO *)GlobalAlloc(GMEM_FIXED, dwBufLen);
 
    if ((lpProtocolBuf == NULL) || (*wsapi == NULL))
    {
           printf("GlobalAlloc() failed with error code %d\n", GetLastError());
           return FALSE;
    }
    printf("GlobalAlloc() is working...\n");
 
    dwRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
    if (dwRet == SOCKET_ERROR)
    {
           printf("WSAEnumProtocols() failed with error code %d\n", WSAGetLastError());
           GlobalFree(lpProtocolBuf);
           return FALSE;
    }
    printf("WSAEnumProtocols() is OK\n");
 
    // Loop through the returned protocol information looking for those
    // that are in the AF_NETBIOS address family.
    for (i=0; i<dwRet;i++)
    {
           if (lpProtocolBuf[i].iAddressFamily == AF_NETBIOS)
           {
            if (iSocketType == SOCK_SEQPACKET)
            {
                if ( (lpProtocolBuf[i].dwServiceFlags1 & XP1_GUARANTEED_DELIVERY) &&
                    (lpProtocolBuf[i].dwServiceFlags1 & XP1_GUARANTEED_ORDER) &&
                    ((lpProtocolBuf[i].dwServiceFlags1 & XP1_CONNECTIONLESS) == 0) &&
                    (lpProtocolBuf[i].iSocketType == iSocketType))
                {
                    (*wsapi)[(*dwCount)++] = lpProtocolBuf[i];
                }
            }
            else if (iSocketType == SOCK_DGRAM)
            {
                if ( !(lpProtocolBuf[i].dwServiceFlags1 & XP1_GUARANTEED_DELIVERY) &&
                    !(lpProtocolBuf[i].dwServiceFlags1 & XP1_GUARANTEED_ORDER) &&
                    ((lpProtocolBuf[i].dwServiceFlags1 & XP1_CONNECTIONLESS) != 0) &&
                    (lpProtocolBuf[i].iSocketType == iSocketType))
                {
                    (*wsapi)[(*dwCount)++] = lpProtocolBuf[i];
                }
            }
           }
    }
    GlobalFree(lpProtocolBuf);
    return TRUE;
}


// Function: main
// Description:
//    Parse the command line parameters, load the Winsock library,
//    enumerate the LANAs, and listen according to what is specified
//    by the command line. By default, a listen will be posted on
//    every available LANA. Once a client connection is made, spawn
//    a thread to handle that connection
int NetBiosMain(int argc, char **argv)
{
    WSADATA           wsd;
    HANDLE            hThread;
    DWORD             dwThreadId, dwNumProtocols, dwIndex, dwErr, i;
    int               iLastByte, addrlen = sizeof(SOCKADDR_NB), iEvents;
    WSAPROTOCOL_INFO *wsapi=NULL;
    SOCKET           *sockListen=NULL,     // array of listening sockets
                                sockClient;
    WSAEVENT         *hEvents=NULL;      // one event for each LANA
    SOCKADDR_NB       nbaddr,               // our NetBIOS address
                                          nbclient;              // client's NetBIOS address
    BOOL              bDone;
 
    // Parse command line and load Winsock library
    //if(argc < 2)
    //{
    //    usage();
    //    return 1;
    //}
    //else
    //{
        printf("Validating argument(s)...\n");
        ValidateArgs(argc, argv);
       
        // These all just another verification for debugging purpose!
        printf("iLana = %d\n", iLana);
        printf("szServerName = %s\n", szServerName);
        printf("iPort = %d\n", iPort);
        printf("iSocketType = %d\n", iSocketType);
        printf("dwCount = %d\n", dwCount);
        printf("dwSize = %d\n", dwSize);
        printf("\n");      
	//}

    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("Unable to load Winsock! Error code is %ld\n", WSAGetLastError());
        return 1;
    }
    else
        printf("WSAStartup() is OK, Winsock lib loaded!\n");
 
    // If we are listening on all LANAs enumerate them  
    //if (!bOneLana)
    //{
       if (FindProtocol(&wsapi, &dwNumProtocols) != TRUE)
       {
           printf("FindProtocol(): Unable to find correct protocol!\n");
           // Just exit
           return 1;
       }
       else
           printf("FindProtocol(): Correct protocol found!\n");

       if (dwNumProtocols == 0)
       {
           printf("FindProtocol(): No NetBIOS capable providers found!\n");
           // Just exit
           return 1;
       }
       else
           printf("FindProtocol(): NetBIOS capable providers found!\n");
   //}
   //else
   //{
   //    dwNumProtocols = 1;
   //}
   memset((&nbaddr)->snb_name, 0, NETBIOS_NAME_LENGTH);

   SET_NETBIOS_SOCKADDR(&nbaddr, NETBIOS_UNIQUE_NAME, szServerName, iPort);
 
   // Allocate a SOCKET handle for each LANA we are going to listen on
   sockListen = (SOCKET *)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, sizeof(SOCKET) * dwNumProtocols);
   if (sockListen == NULL)
   {
       printf("Out of memory!\n");
       return 1;
   }
 
   // Allocate an event handle for each LANA we are going to listen on
   hEvents = (WSAEVENT *) GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,sizeof(WSAEVENT) * dwNumProtocols);
   if (hEvents == NULL)
   {
       printf("Out of memory!\n");
       return 1;
   }
 
   // For each service provider create a ServerThread except for
   // the last provider which we'll start in this main thread
   for (i=0; i < dwNumProtocols ;i++)
   {
       if (!bOneLana)
           printf("Transport: '%S'\n", wsapi[i].szProtocol);
       else
           printf("Transport: LANA # is %d\n", iLana);

       hEvents[i] = WSACreateEvent();
       if (hEvents[i] == NULL)
       {
           printf("WSACreateEvent() failed with error code %d\n", WSAGetLastError());
           continue;
       }
 
       // Create socket(s)
       //if (!bOneLana)
           sockListen[i] = WSASocket(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO,
           FROM_PROTOCOL_INFO, &wsapi[i], 0, WSA_FLAG_OVERLAPPED);
       //else
       //    sockListen[i] = WSASocket(AF_NETBIOS, SOCK_SEQPACKET, -iLana, NULL, 0, WSA_FLAG_OVERLAPPED);
       if (sockListen[i] == SOCKET_ERROR)
       {
           printf("WSASocket() failed with error code %d\n", WSAGetLastError());
           continue;
       }
 
       // Bind and listen each socket to our server name
       if (bind(sockListen[i], (SOCKADDR *)&nbaddr, sizeof(nbaddr)) == SOCKET_ERROR)
       {
           printf("bind() failed: %d\n", WSAGetLastError());
           return 1;
       }
       else
           printf("bind() looks OK!\n");

       listen(sockListen[i], 10);
       printf("listen() also seems fine, so I\'m listening...\n");

       // We're only interested in FD_ACCEPT events for our server sockets
       if (iSocketType == SOCK_SEQPACKET)
		   iEvents = FD_ACCEPT;
	   else
           iEvents = FD_READ;

	   /* The WSAEventSelect function specifies an event object 
	    * to be associated with the specified set of FD_XXX network events. 
		* 
		* s [in]
        *   A descriptor identifying the socket.
		*
        * hEventObject [in]
        *   A handle identifying the event object to be associated 
		*   with the specified set of FD_XXX network events.
		*
        * lNetworkEvents [in] 
        *   A bitmask that specifies the combination of FD_XXX 
		*   network events in which the application has interest.
		*/
       if (WSAEventSelect(sockListen[i], hEvents[i], iEvents) == SOCKET_ERROR)
       {
           printf("WSAEventSelect() failed with error code %d\n", WSAGetLastError());
           return 1;
       }
       else
           printf("WSAEventSelect() pretty damn OK!\n");
   }
 
   bDone = FALSE;
   while (!bDone)
   {
       // Wait until a client connection is pending
	   printf("WSAWaitForMultipleEvents\n");
       dwIndex = WSAWaitForMultipleEvents(dwNumProtocols, hEvents, FALSE,WSA_INFINITE, FALSE);
       if (dwIndex == WSA_WAIT_FAILED)
       {
		   printf("WSAWaitForEvents() failed with error code %d\n", WSAGetLastError());
		   return 1;
       }
       else
           printf("WSAWaitForEvents() is OK dude!\n");
 
       addrlen = sizeof(nbclient);
       if (iSocketType == SOCK_SEQPACKET)
       {
           sockClient = accept(sockListen[dwIndex], (SOCKADDR *)&nbclient, &addrlen);
           if (sockClient == INVALID_SOCKET)
           {
               if ((dwErr = WSAGetLastError()) != WSAEWOULDBLOCK)
               {
                   printf("accept() failed with error code %d\n", dwErr);
                   return 1;
               }
               else
               {
                   continue;
               }
           }
           printf("accept() seems working...\n");

           // Print out the client name who connected
           // However this retard code failed miserabily!!!:-(
           iLastByte = nbclient.snb_name[NETBIOS_NAME_LENGTH-1];
           nbclient.snb_name[NETBIOS_NAME_LENGTH-1];
           printf("Client '%s (%02d)' got connected...\n", nbclient.snb_name, iLastByte);

           // Create a thread to handle the connection, jump to CreateThread()
           hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)sockClient,0, &dwThreadId);
           if (hThread == NULL)
           {
               printf("CreateThread() failed with error code %d\n", GetLastError());
               return 1;
           }
           else
               printf("CreateThread() is OK!\n");

           CloseHandle(hThread);
           sockClient = INVALID_SOCKET;
       }
       // iSocketType == SOCK_DGRAM
       else
       {
           char    recvBuff[MAX_BUFFER];
           DWORD   dwRet;
          
           dwRet = recvfrom(sockListen[dwIndex], recvBuff, MAX_BUFFER, 0,(SOCKADDR *)&nbclient, &addrlen);
           if (dwRet == SOCKET_ERROR)
           {
               if ((dwErr = WSAGetLastError()) == WSAEWOULDBLOCK)
                           continue;
               printf("recvfrom() failed with error code %d\n", dwErr);
               return 1;
           }
           else
               printf("recvfrom() is OK!\n");

           printf("Read %d bytes...\n", dwRet);
           printf("Received money: %s\n", recvBuff);

           iLastByte = nbclient.snb_name[NETBIOS_NAME_LENGTH-1];
           nbclient.snb_name[NETBIOS_NAME_LENGTH-1];

           printf("Read %d bytes from '%s'(%02d)\n", dwRet, nbclient.snb_name, iLastByte);
          
           dwRet = sendto(sockListen[dwIndex], recvBuff, dwRet, 0, (SOCKADDR *)&nbclient, sizeof(nbclient));
           if (dwRet == SOCKET_ERROR)
           {
               if ((dwErr = WSAGetLastError()) == WSAEWOULDBLOCK)
                    continue;
               printf("sendto() failed with error code %d\n", dwErr);
               return 1;
           }
           else
               printf("sendto() is OK...\n");

           printf("Wrote %d bytes successfully...\n", dwRet);
       }
       WSAResetEvent(hEvents[dwIndex]);
   }
   // Clean up things
   printf("Cleaning-up and de-allocating all the rubbish...\n");
   for (i=0; i < dwNumProtocols ;i++)
   {
       closesocket(sockListen[i]);
       WSACloseEvent(hEvents[i]);
   }
 
   GlobalFree(wsapi);
   GlobalFree(hEvents);
   GlobalFree(sockListen);
   WSACleanup();
   return 0;
 }

 #define DEFAULT_ZONE       "*"
 #define DEFAULT_TYPE       "Windows Sockets"
 #define DEFAULT_OBJECT     "AppleTalk-Server_Bodo"
 char szZone[MAX_ENTITY], szType[MAX_ENTITY], szObject[MAX_ENTITY];
 BOOL bFindName = FALSE, bListZones = FALSE, bListMyZone = FALSE;

void usage_appletalk()
{
    printf("Usage: appletalkzone [options]\n");
    printf("         Name Lookup:\n");
    printf("           -z:ZONE-NAME\n");
    printf("           -t:TYPE-NAME\n");
    printf("           -o:OBJECT-NAME\n");
    printf("         List All Zones:\n");
    printf("           -lz\n");
    printf("         List My Zone:\n");
    printf("           -lm\n");
    ExitProcess(1);
}

void ValidateArgs_appletalk(int argc, char **argv)
{
            int i;
            strcpy_s(szZone, sizeof(szZone),DEFAULT_ZONE);
            strcpy_s(szType, sizeof(szType),DEFAULT_TYPE);
            strcpy_s(szObject, sizeof(szObject),DEFAULT_OBJECT);
           
            for(i = 1; i < argc; i++)
            {
                        if (strlen(argv[i]) < 2)
                                    continue;
                        if ((argv[i][0]=='-')||(argv[i][0]=='/'))
                        {
                                    switch (tolower(argv[i][1]))
                                    {
                                    case 'z':        // Specify a zone name
                                                if (strlen(argv[i]) > 3)
                                                            strncpy_s(szZone, sizeof(szZone),&argv[i][3], MAX_ENTITY);
                                                bFindName = TRUE;
                                                break;
                                    case 't':        // Specify a type name
                                                if (strlen(argv[i]) > 3)
                                                            strncpy_s(szType, sizeof(szType),&argv[i][3], MAX_ENTITY);
                                                bFindName = TRUE;
                                                break;
                                    case 'o':        // Specify an object name
                                                if (strlen(argv[i]) > 3)
                                                            strncpy_s(szObject, sizeof(szObject),&argv[i][3], MAX_ENTITY);
                                                bFindName = TRUE;
                                                break;
                                    case 'l':        // List zones information
                                                if (strlen(argv[i]) == 3)
                                                            // List all zones
                                                            if (tolower(argv[i][2]) == 'z')
                                                                        bListZones = TRUE;
                                                            // List my zone
                                                            else if (tolower(argv[i][2]) == 'm')
                                                                        bListMyZone = TRUE;
                                                break;
                                    default:
                                                usage();
            }
        }
    }
}

void AppleTalkLookupZones(int argc, char **argv)
{
    WSADATA              wsd;
    char                 cLookupBuffer[16000], *pTupleBuffer = NULL;
    PWSH_NBP_TUPLE       pTuples = NULL;
    PWSH_LOOKUP_NAME     atlookup;
    PWSH_LOOKUP_ZONES    zonelookup;
    SOCKET               s;
    DWORD                dwSize = sizeof(cLookupBuffer);
    SOCKADDR_AT          ataddr;
    int                  i;
 
    // Load the Winsock library
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
    {
        printf("Unable to load Winsock library!\n");
        return 1;
    }
    printf("WSAStartup() is OK! Library loaded...\n");
 
    ValidateArgs_appletalk(argc, argv);
 
    atlookup = (PWSH_LOOKUP_NAME)cLookupBuffer;
    zonelookup = (PWSH_LOOKUP_ZONES)cLookupBuffer;
    if (bFindName)
    {
        // Fill in the name to look up
        strcpy_s(atlookup->LookupTuple.NbpName.ObjectName, sizeof(atlookup->LookupTuple.NbpName.ObjectName), szObject);
        atlookup->LookupTuple.NbpName.ObjectNameLen = strlen(szObject);
        strcpy_s(atlookup->LookupTuple.NbpName.TypeName, sizeof(atlookup->LookupTuple.NbpName.TypeName),szType);
        atlookup->LookupTuple.NbpName.TypeNameLen = strlen(szType);
        strcpy_s(atlookup->LookupTuple.NbpName.ZoneName, sizeof(atlookup->LookupTuple.NbpName.ZoneName),szZone);
        atlookup->LookupTuple.NbpName.ZoneNameLen = strlen(szZone);
    }
 
    // Create the AppleTalk socket
    s = socket(AF_APPLETALK, SOCK_STREAM, ATPROTO_ADSP);
    if (s == INVALID_SOCKET)
    {
        printf("socket() failed with error code %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("socket() is OK for AppleTalk!\n");
 
    // We need to bind in order to create an endpoint on the
    // AppleTalk network to make our query from
    ZeroMemory(&ataddr, sizeof(ataddr));
    ataddr.sat_family = AF_APPLETALK;
    ataddr.sat_socket = 0;
    if (bind(s, (SOCKADDR *)&ataddr, sizeof(ataddr)) == INVALID_SOCKET)
    {
        printf("bind() failed with error code %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("bind() is OK!\n");
 
    if (bFindName)
    {
        printf("Looking up: %s:%s@%s\n", szObject, szType, szZone);
		/*
		 * SO_LOOKUP_NAME
		 * 
		 *    Looks up a specified NBP name 
		 *    and returns the matching tuples 
		 *    of name and NBP information. 
		 *
		 *    The optlen parameter must be 
		 *    at least one byte larger than the size of the WSH_LOOKUP_NAME structure.
		 */
        if (getsockopt(s, SOL_APPLETALK, SO_LOOKUP_NAME, (char *)atlookup, &dwSize) == INVALID_SOCKET)
        {
            printf("getsockopt(SO_LOOKUP_NAME) failed with error code %d\n", WSAGetLastError());
            return 1;
        }
        else
            printf("getsockopt(SO_LOOKUP_NAME) is OK!\n");
 
        printf("Lookup returned: %d entries\n", atlookup->NoTuples);
 
        // Our character buffer now contains an array of
        // WSH_NBP_TUPLE structures after our WSH_LOOKUP_NAME structure
        pTupleBuffer = (char *)cLookupBuffer + sizeof(WSH_LOOKUP_NAME);
        pTuples = (PWSH_NBP_TUPLE) pTupleBuffer;
 
        for(i = 0; i<(int)atlookup->NoTuples; i++)
        {
            ataddr.sat_family = AF_APPLETALK;
            ataddr.sat_net    = pTuples[i].Address.Network;
            ataddr.sat_node   = pTuples[i].Address.Node;
            ataddr.sat_socket = pTuples[i].Address.Socket;
            printf("server address = %lx.%lx.%lx.\n",
                ataddr.sat_net,
                ataddr.sat_node,
                ataddr.sat_socket);
        }
    }
    else if (bListZones)
    {
        // It is very important to pass a sufficiently big buffer
        // for this option. Windows NT 4 SP3 blue screens if it is too small.
		/* Same as SO_GETZONELIST.
		 * Returns zone names from the Internet zone list. 
		 * The optlen parameter must be at least 
		 * one byte larger than the size 
		 * of the WSH_LOOKUP_ZONES structure.
		 */
        if (getsockopt(s, SOL_APPLETALK, SO_LOOKUP_ZONES, (char *)atlookup, &dwSize) == INVALID_SOCKET)
        {
            printf("getsockopt(SO_LOOKUP_NAME) failed with error code %d\n", WSAGetLastError());
            return 1;
        }
        else
            printf("getsockopt(SO_LOOKUP_NAME) looks fine!\n");
 
        printf("Lookup returned: %d zones\n", zonelookup->NoZones);
        // The character buffer contains a list of null-separated
        //  strings after the WSH_LOOKUP_ZONES structure
        pTupleBuffer = (char *)cLookupBuffer + sizeof(WSH_LOOKUP_ZONES);
        for(i = 0; i<(int)zonelookup->NoZones; i++)
        {
            printf("%3d: '%s'\n", i+1, pTupleBuffer);
            while (*pTupleBuffer++);
        }
    }
    else if (bListMyZone)
    {
        // This option returns a simple string
		/* Same as SO_GETMYZONE 
		 * Returns the default zone on the network.
		 */
        if (getsockopt(s, SOL_APPLETALK, SO_LOOKUP_MYZONE, (char *)cLookupBuffer, &dwSize) == INVALID_SOCKET)
        {
            printf("getsockopt(SO_LOOKUP_NAME) failed with error code %d\n", WSAGetLastError());
            return 1;
        }
        else
            printf("getsockopt(SO_LOOKUP_NAME) is OK lor!\n");
 
        printf("My Zone: '%s'\n", cLookupBuffer);
    }
    else
	    usage_appletalk();
 
    WSACleanup();

    return 0;   
}

