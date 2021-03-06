
#include "winsocketclient.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <wsnetbs.h>


#pragma comment(lib, "Ws2_32.lib")

#define MAX_BUFFER 20
#define DEFAULT_COUNT 20

char    szServerName[NETBIOS_NAME_LENGTH];   // Our NetBIOS name
int     iPort,                               // Our 16th byte
        iLana,                               // LANA to connect on
        iSocketType = SOCK_SEQPACKET;        // Datagram or stream?
DWORD   dwCount=DEFAULT_COUNT,               // How many packets
        dwSize=MAX_BUFFER;                   // Size of buffer to send
BOOL    bOneLana=FALSE;                      // Connect on one LANA

void TcpClient()
{
   WSADATA      wsaData;
   SOCKET       SendingSocket;
   // Server/receiver address
   SOCKADDR_IN  ServerAddr, ThisSenderInfo;
   int          Port = 5150;
   // Be careful with the array bound, provide some checking mechanism...
   char         sendbuf[1024] = "This is a test string from sender";
   int          nBytes = 2048;
   int          nLeft, nlen;
   int          idx;
   int          ret;
   int          RetCode;
   int          BytesSent;

   // Initialize Winsock version 2.2
   WSAStartup(MAKEWORD(2,2), &wsaData);
   printf("Client: Winsock DLL status is %s.\n", wsaData.szSystemStatus);

   // Create a new socket to make a client connection.
   // AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
   SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (SendingSocket == INVALID_SOCKET)
   {
       printf("Client: socket() failed! Error code: %ld\n", WSAGetLastError());
       // Do the clean up
       WSACleanup();
       // Exit with error
       return -1;
   }
   else
       printf("Client: socket() is OK!\n");

   // Set up a SOCKADDR_IN structure that will be used to connect
   // to a listening server on port 5150. For demonstration
   // purposes, let's assume our server's IP address is 127.0.0.1 or localhost
 
   // IPv4
   ServerAddr.sin_family = AF_INET;
   // Port no.
   ServerAddr.sin_port = htons(Port);
   // The IP address
   ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

   // Make a connection to the server with socket SendingSocket.
   RetCode = connect(SendingSocket, (SOCKADDR *) &ServerAddr, sizeof(ServerAddr));
   if (RetCode != 0)
   {
       printf("Client: connect() failed! Error code: %ld\n", WSAGetLastError());
       // Close the socket
       closesocket(SendingSocket);
       // Do the clean up
       WSACleanup();
       // Exit with error
       return -1;
   }
   else
   {
       printf("Client: connect() is OK, got connected...\n");
       printf("Client: Ready for sending and receiving data...\n");
   }

   // At this point you can start sending or receiving data on
   // the socket SendingSocket. We will describe sending and receiving data
   // later in the chapter.

   getsockname(SendingSocket, (SOCKADDR *)&ServerAddr, (int *)sizeof(ServerAddr));
   printf("Client: Receiver IP(s) used: %s\n", inet_ntoa(ServerAddr.sin_addr));
   printf("Client: Receiver port used: %d\n", htons(ServerAddr.sin_port));
 
   // Sends some data to server/receiver...
   BytesSent = send(SendingSocket, sendbuf, strlen(sendbuf), 0);
   if (BytesSent == SOCKET_ERROR)
       printf("Client: send() error %ld.\n", WSAGetLastError());
   else
   {
       printf("Client: send() is OK - bytes sent: %ld\n", BytesSent);
       // Some info on this sender side...
       // Allocate the required resources
       memset(&ThisSenderInfo, 0, sizeof(ThisSenderInfo));
       nlen = sizeof(ThisSenderInfo);

       getsockname(SendingSocket, (SOCKADDR *)&ThisSenderInfo, &nlen);
       printf("Client: Sender IP(s) used: %s\n", inet_ntoa(ThisSenderInfo.sin_addr));
       printf("Client: Sender port used: %d\n", htons(ThisSenderInfo.sin_port));
       printf("Client: Those bytes represent: \"%s\"\n", sendbuf);
   }
 
   if (shutdown(SendingSocket, SD_SEND) != 0)
       printf("Client: Well, there is something wrong with the shutdown(). The error code: %ld\n", WSAGetLastError());
   else
       printf("Client: shutdown() looks OK...\n");

   // When you are finished sending and receiving data on socket SendingSocket,
   // you should close the socket using the closesocket API. We will
   // describe socket closure later in the chapter.
   if(closesocket(SendingSocket) != 0)
       printf("Client: Cannot close \"SendingSocket\" socket. Error code: %ld\n", WSAGetLastError());
   else
       printf("Client: Closing \"SendingSocket\" socket...\n");

   // When your application is finished handling the connection, call WSACleanup.
   if(WSACleanup() != 0)
       printf("Client: WSACleanup() failed!...\n");
   else
       printf("Client: WSACleanup() is OK...\n");
   return 0;
}

void UdpClient()
{
   WSADATA         wsaData;
   SOCKET          SendingSocket;
   SOCKADDR_IN     ReceiverAddr;
   int             Port = 5150;
   char            SendBuf[1024] = "This is a test string from sender";
   int             BufLength = 1024; 
   int             BytesSent;

   // Initialize Winsock version 2.2
   WSAStartup(MAKEWORD(2,2), &wsaData);
   printf("Client: Winsock DLL status is %s.\n", wsaData.szSystemStatus);

   // Create a new socket to make a client connection.
   // AF_INET = 2, The Internet Protocol version 4 (IPv4) address family, TCP protocol
   SendingSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  
   // Set up a SOCKADDR_IN structure that will be used to connect
   // to a listening server on port 5150. For demonstration
   // purposes, let's assume our server's IP address is 127.0.0.1 or localhost
 
   // IPv4
   ReceiverAddr.sin_family = AF_INET;
   // Port no.
   ReceiverAddr.sin_port = htons(Port);
   // The IP address
   
   ReceiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  

   while (1)
   {
       BytesSent = sendto(SendingSocket, SendBuf, BufLength, 0, (SOCKADDR*)&ReceiverAddr, sizeof(ReceiverAddr));
   }

   if (BytesSent == SOCKET_ERROR)
   {
       printf("Client: send() error %ld.\n", WSAGetLastError());
	   return;
   }

   if(closesocket(SendingSocket) != 0)
       printf("Client: Cannot close \"SendingSocket\" socket. Error code: %ld\n", WSAGetLastError());
   else
       printf("Client: Closing \"SendingSocket\" socket...\n");

   // When your application is finished handling the connection, call WSACleanup.
   if(WSACleanup() != 0)
       printf("Client: WSACleanup() failed!...\n");
   else
       printf("Client: WSACleanup() is OK...\n");
   return 0;
}

// Function: usage
// Description: Print out usage information.
int usage()
{
    printf("Usage: NetbiosClient -n str -p int -l int -t char\n");
    printf("       -n NAME        Our NetBIOS name\n");
    printf("       -p PORT        The 16th byte qualifier of our name\n");
    printf("       -l LANA        Specifies to connect on this LANA only (this is client LANA lor!)\n");
    printf("                       By default attempt connection on all LANAs\n");
    printf("       -t TYPE        Specifies datagram (d) or seqpacket (s)\n");
    printf("       -c COUNT       Number of types to send the message\n");
    printf("       -b SIZE        Size of buffer to send\n");
            printf("Example: NetbiosClient -n MIKEBLUR -p 7172 -l 3 -t s -c 5 -b 64\n");
    return 0;
}

// Function: ValidateArgs
// Description
//    Parse the argument list for our NetBIOS name and whether
//    we want to operate on all LANAs or just one
int ValidateArgs(int argc, char **argv)
{
     int        i;
 
     if (argc > 1)
     {
     for (i = 1; i < argc; i++)
     {
          if((argv[i][0] == '-') || (argv[i][0] == '/'))
          {
              switch (tolower(argv[i][1]))
              {
                  case '?':
                       usage();
                       break;
                   case 'n':   // use a unique name
                        if (i+1 < argc)
                     {
                         strncpy_s(szServerName,sizeof(szServerName),argv[i+1],NETBIOS_NAME_LENGTH);
                         if (strlen(szServerName) != 0)
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
                   case 'l':   // connect on one lana only
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
                      case 'c':   // num types to send message
                         if (i+1 < argc)
                         {
                             dwCount = atol(argv[i+1]);
                             ++i;
                        }   
                        break;   
                    case 'b':   // size of send buffer
                        if (i+1 < argc)
                        {
                            dwSize = (atol(argv[i+1]) > MAX_BUFFER ?MAX_BUFFER : atol(argv[i+1]));
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
//    This function parses the command line arguments, loads the
//    Winsock library, and connects to a server. By default, a
//    connection is attempted on all available LANA numbers. The
//    first one to succeed is used and the others are cancelled
void NetBiosClient(int argc, char **argv)
{
     WSADATA           wsd;
     SOCKET           *socks=NULL;           // array of socket handles
     WSAEVENT         *hEvents=NULL;         // events for each socket
     SOCKADDR_NB       nbaddr;               // NetBIOS addr of server
     WSAPROTOCOL_INFO *wsapi=NULL;
     DWORD             dwRet,dwNumProtocols,dwIndex,dwErr,i;
     char              szMessage[MAX_BUFFER];   // Data buffer
     unsigned long     iOptVal;
     struct fd_set     fdread;
     int j;
 
     // Validate arguments
     //if(argc < 2)
     //{
     //   usage();
     //   return 1;
     //}
     //else
        ValidateArgs(argc, argv);

     strcpy_s(szServerName, sizeof(szServerName), "MIKEBLUR");
     iPort = 7172;

     // These all just another verification!
     printf("iLana = %d\n", iLana);
     printf("szServerName = %s\n", szServerName);
     printf("iPort = %d\n", iPort);
     printf("iSocketType = %d\n", iSocketType);
     printf("dwCount = %d\n", dwCount);
     printf("dwSize = %d\n", dwSize);
     printf("\n");
 
     if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
     {
        printf("Unable to load Winsock!\n");
        return 1;
     }
     else
        printf("WSAStartup() is OK!\n");

     //  If we're connecting on all LANAs enumerate all AF_NETBIOS
     //  protocols; if not we can just specify the LANA we want
     //  when creating the socket
     if (bOneLana == FALSE)
     {
         // This function will return an array of WSAPROTOCOL_INFO
         //  structures that match our socket type
        if (FindProtocol(&wsapi, &dwNumProtocols) != TRUE)
        {
           printf("FindProtocol(): Unable to find correct protocol!\n");
           return 1;
        }
        else
           printf("FindProtocol(): Correct protocol found!\n");
 
        if (dwNumProtocols == 0)
        {
               printf("FindProtocol(): No NetBIOS capable providers found!\n");
               return 1;
        }
        else
               printf("FindProtocol(): NetBIOS capable providers found!\n");
     }
     else
     {
        dwNumProtocols = 1;
     }
     // Setup the NetBIOS address name
     SET_NETBIOS_SOCKADDR(&nbaddr, NETBIOS_UNIQUE_NAME, szServerName,iPort);
 
     // Allocate some global structures
     //  socks : array of SOCKET handles for each transport we connect on
     //  hEvents : array of WSAEVENT handles for event notification
     socks = (SOCKET *)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,sizeof(SOCKET) * dwNumProtocols);
     if (socks == NULL)
     {
        printf("Out of memory\n");
        return 1;
     }
     hEvents = (WSAEVENT *)GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT,sizeof(WSAEVENT) * dwNumProtocols);
     if (hEvents == NULL)
     {
        printf("Out of memory\n");
        return 1;
     }
     else
        printf("GlobalAlloc() looks fine!\n");
 
     // For each LANA, create a WSAEVENT, create the SOCKET, and
     // register it for what events we want to receive
     for (i=0; i < dwNumProtocols; i++)
     {
         if (bOneLana)
            printf("Transport LANA #: %d\n", iLana);
         else
            printf("Transport: '%S'\n", wsapi[i].szProtocol);
 
         hEvents[i] = WSACreateEvent();
         if (hEvents[i] == NULL)
         {
            printf("WSACreateEvent() failed with error code %d\n", WSAGetLastError());
            return 1;
         }
         else
            printf("WSACreateEvent() is OK!\n");
 
         if (bOneLana == FALSE)
            socks[i] = WSASocket(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO,
                                 FROM_PROTOCOL_INFO, &wsapi[i], 0, WSA_FLAG_OVERLAPPED);
         else
            socks[i] = WSASocket(AF_NETBIOS, SOCK_SEQPACKET, -iLana, NULL,0, WSA_FLAG_OVERLAPPED);
 
         if (socks[i] == INVALID_SOCKET)                        
		 {
            printf("socket() failed with error code %d\n", WSAGetLastError());
            //return 1;
			continue;
         }
         else
            printf("socket() is OK!\n");
 
         if (WSAConnect(socks[i], (SOCKADDR *)&nbaddr, sizeof(nbaddr), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
         {
            printf("WSAConnect() failed with error code %d\n", WSAGetLastError());
            continue;
         }

         if (WSAEventSelect(socks[i], hEvents[i], FD_CONNECT) == SOCKET_ERROR)
         {
            printf("WSAEventSelect() failed with error code %d\n", WSAGetLastError());
            return 1;
         }
         else
            printf("WSAEventSelect() is OK!\n");
     }
 
     // Wait for one of the connects to succeed
     dwIndex = WSAWaitForMultipleEvents(dwNumProtocols, hEvents, FALSE,WSA_INFINITE, FALSE);
     if (dwIndex == WSA_WAIT_FAILED)
     {
         printf("WSAWaitForMultipleEvents() failed with error code %d\n", WSAGetLastError());
         return 1;
     }
     else
         printf("WSAWaitForMultipleEvents() is OK!\n");
 
     // Close the sockets of all other pending connections other than the one that completed first
     dwIndex -= WAIT_OBJECT_0;
     for (i=0; i < dwNumProtocols ;i++)
     {
         if (i != dwIndex)
            closesocket(socks[i]);
     }
 
     // Put the socket in non-blocking mode
     iOptVal = 1L;
     if (ioctlsocket(socks[dwIndex], FIONBIO, &iOptVal) == SOCKET_ERROR)
     {
         printf("ioctlsocket(FIONBIO) failed with error code %d\n", WSAGetLastError());
     }
     else
         printf("ioctlsocket(FIONBIO) is OK!\n");
 
     memset(szMessage, '$', dwSize);
     for (i=0; i < dwCount ;i++)
     {
        // Send the message to the server
        dwRet = send(socks[dwIndex], szMessage, dwSize, 0);
        if (dwRet == 0)
        {
            printf("send() failed coz of graceful close...\n");
            closesocket(socks[dwIndex]);
            return 0;
        }
        else if (dwRet == SOCKET_ERROR)
        {
            if ((dwErr = WSAGetLastError()) == WSAEWOULDBLOCK)
            {
                i--;
                continue;
            }
            else
            {
                printf("send() failed with error code %d\n", dwErr);
                return 1;
            }
        }
        else
             printf("send() looks fine!\n");
 
        printf("Wrote %d bytes...\n", dwRet);
        // Wait until the server echoes the data back. This really doesn't
        //  matter when using SOCK_SEQPACKET, but if we're using SOCK_DGRAM
        //  then our recv() would fail with WSAEWOULDBLOCK and we'd skip
        //  the returned data (as the server might not have sent it yet)
        FD_ZERO(&fdread);
        FD_SET(socks[dwIndex], &fdread);
        select(0, &fdread, NULL, NULL, NULL);
        // Read the message back
        dwRet = recv(socks[dwIndex], szMessage, MAX_BUFFER, 0);
        if (dwRet == 0)
        {
            printf("recv() failed coz of graceful close!\n");
            closesocket(socks[dwIndex]);
            return 0;
        }
        else if (dwRet == SOCKET_ERROR)
        {
            if ((dwErr = WSAGetLastError()) == WSAEWOULDBLOCK)
            {
                i--;
                continue;
            }
            else
            {
                printf("recv() failed with error code %d\n", dwErr);
                return 1;
            }
        }
        else
            printf("recv() is OK!\n");
 
        printf("Received money: ");
        for(j=0;szMessage[j] == '$';j++)
            printf("%c", szMessage[j]);
 
        printf("\nRead %d bytes...\n", dwRet);
    }
 
    printf("Cleaning up and de-allocation...\n");
    closesocket(socks[dwIndex]);
    for (i=0; i < dwNumProtocols ;i++)
         WSACloseEvent(hEvents[i]);

    GlobalFree(wsapi);
    GlobalFree(socks);
    GlobalFree(hEvents);
    WSACleanup();
    return 0;      
}