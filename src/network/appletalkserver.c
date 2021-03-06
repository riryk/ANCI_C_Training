// Description:
//    This sample illustrates how to use the AppleTalk PAP and ADSP
//    protocol. This sample either acts as a sender or receiver.
//    If acting as the receiver use the -z -t and -n options to
//    specify the zone, type, and object name of your receiver
//    (server).  If acting as the sender specify the -s options
//    and the -z -t and -n options refer to the name of the
//    AppleTalk endpoint to connect to (i.e. the receiver)
//
// Command Line Parameters/Options:
//    Usage: AppleTalkExample [-s] [-z Zone] [-t Type] [n Object] [-c x] [-b x] [-p str]
//       -s         Act as a sender\n");
//       -z Zone    AppleTalk zone\n");
//       -t Type    AppleTalk type name\n");
//       -n Object  AppleTalk object name\n");
//       -c x       Number of packets to send\n");
//       -b x       Buffer size to send\n");
//       -p str     Specifies protocol (PAP or ADSP)
//          Receiver example: AppleTalkExample -z * -t WindowsSockets -n AppletalkApp
 
// http://msdn.microsoft.com/en-us/library/ms737629(VS.85).aspx
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
 
#include <windows.h>
// Link to ws2_32.lib
#include <winsock2.h>
#include <atalkwsh.h>
#include <stdio.h>
#include <stdlib.h>
// Link to Mswsock.lib
// For WSARecvEx()
#include <Mswsock.h>

#pragma comment(lib, "Mswsock.lib")
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_ZONE        "*"
#define DEFAULT_TYPE        "WindowsSockets"
#define DEFAULT_OBJECT      "AppletalkApp"
 
// The maximum size of a PAP message is 4096 bytes
#define MAX_BUFFER          4096
#define DEFAULT_COUNT       20
#define MAX_PROTO           32
 
BOOL bSender=FALSE;
 
char szZone[MAX_ENTITY] = "";                 // Zone to lookup
char szType[MAX_ENTITY] = "";                 // Type the object belongs to
char szObject[MAX_ENTITY] = "";               // Appletalk object (name) to
 
DWORD dwSendSize=MAX_BUFFER, // Number of bytes to send
      dwCount_apple=DEFAULT_COUNT; // Number of packets to send
 
int   iProto=ATPROTO_PAP,            // Protocol to use (PAP or ADSP)
      iSocketType_apple=SOCK_RDM;          // Socket type of the given protocol
                                     //   PAP  == SOCK_RDM while
                                     //   ADSP == SOCK_STREAM


// Function: usage
// Description: Print usage information and exit.
int usage_apple()
{
    printf("Usage: AppleTalkExample [-s] [-z Zone] [-t Type] [-n Object] [-c x] [-b x]\n");
    printf("       -s            Act as a sender\n");
    printf("       -z Zone       AppleTalk zone\n");
    printf("       -t Type       AppleTalk type name\n");
    printf("       -n Object     AppleTalk object name\n");
    printf("       -c x          Number of packets to send\n");
    printf("       -b x          Buffer size to send\n");
    printf("       -p [adsp|pap] Protocol to use\n");
    return 0;
}

// Function: ValidateArgs
// Description:
//    This function parses the command line arguments and sets
//    several global variables based upon the supplied commands
int ValidateArgs_apple(int argc, char **argv)
{
    int    i;
 
    if (argc > 1)
    {
        for (i = 1; i < argc; i++)
        {
           if ( (argv[i][0] == '-') || (argv[i][0] == '/')  )
            {
               switch (tolower(argv[i][1]) )
                                       {
               case '?':
                   usage_apple();
                   break;
                case 's':   // act as a sender
                   bSender = TRUE;
                    break;
                 case 'z':   // zone name
                   if(i+1 < argc)
                   {
                       strcpy_s(szZone,sizeof(szZone),argv[i+1]);
                       ++i;
                    }
                   break;
                  case 't':   // type name
                   if(i+1 < argc)
                   {
                       strcpy_s(szType,sizeof(szType),argv[i+1]);
                       ++i;
                   }
                   break;
                  case 'n':   // object name
                   if(i+1 < argc)
                   {
                       strcpy_s(szObject,sizeof(szObject),argv[i+1]);
                       ++i;
                   }
                   break;
                  case 'c':   // number of packets to send
                   if(i+1 < argc)
                   {
                      dwCount_apple = atoi(argv[i+1]);
                      ++i;
                   }
                   break;
                 case 'b':   // size of packet to send
                   if(i+1 < argc)
                   {
                       dwSendSize = atoi(argv[i+1]);
                       if(dwSendSize > MAX_BUFFER)
                              usage_apple();
                              ++i;
                   }
                   break;
                  case 'p':   // protocol to use
                   if(i+1 < argc)
                  {
                       if(strlen(argv[i+1]) >= 3)
                       {
                           if(0 == _stricmp(argv[i+1],"adsp"))
                           {
                              iProto = ATPROTO_ADSP;
                              iSocketType_apple = SOCK_STREAM;
                           }
                           else if (0 == _stricmp(argv[i+1],"pap"))
                           {
                                iProto = ATPROTO_PAP;
                                iSocketType_apple = SOCK_RDM;
                            }
                       }
                      ++i;
                  }
                  break;
               default:
                  usage_apple();
                   break;
               }
           }
      }
    }
   return 0;
}

// Function: RegisterName
// Description: Register an AppleTalk name on the network.  
int RegisterName_apple(SOCKET s, char * zone, char *type, char *obj)
{
    WSH_REGISTER_NAME    nbpname;
    int             ret;

    // Fill out the WSH_NBP_NAME structure
    // memset(&nbpname,0,sizeof(nbpname)); or
    // ZeroMemory(&nbpname, sizeof(nbpname)); or
    SecureZeroMemory(&nbpname, sizeof(nbpname)); // more secure
    strcpy_s(nbpname.ZoneName, sizeof(nbpname.ZoneName),zone);
   
    nbpname.ZoneNameLen = strlen(zone);
    strcpy_s(nbpname.TypeName, sizeof(nbpname.TypeName), type);
   
    nbpname.TypeNameLen = strlen(type);
    strcpy_s(nbpname.ObjectName, sizeof(nbpname.ObjectName),obj);
    nbpname.ObjectNameLen = strlen(obj);

    /*
           printf("Registering %s:%s@%s\n", nbpname.ObjectName,
                                nbpname.TypeName,
                                nbpname.ZoneName);

    */
   
    printf("Registering: %s:%s@%s\n", obj, type, zone);
    /* Registers the given name on the AppleTalk network */ 
    ret = setsockopt(s, SOL_APPLETALK, SO_REGISTER_NAME, (char *)&nbpname, sizeof(nbpname));
    if (ret == SOCKET_ERROR)
    {
        printf("setsockopt(SO_REGISTER_NAME) failed with error code %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("setsockopt(SO_REGISTER_NAME) is OK!\n");
   
    return 0;
}

// Function: FindName
// Description:
//    Lookup a given name on the AppleTalk network so that we can
//    get the network, node, and socket number associated with it
//    in order to make a connection
int FindName_apple(SOCKET s, char *zone, char *type, char *obj, SOCKADDR_AT *addr)
{
    char              szLookup[MAX_BUFFER], *lpLookup;
    PWSH_LOOKUP_NAME  lpLookupName=NULL;
    int               size = MAX_BUFFER,  ret;
    PWSH_NBP_TUPLE    lpTuple;

    lpLookupName = (PWSH_LOOKUP_NAME)szLookup;
   
    // Fill in our WSH_LOOKUP_NAME structure with the name to find
    strcpy_s(lpLookupName->LookupTuple.NbpName.ObjectName, sizeof(lpLookupName->LookupTuple.NbpName.ObjectName),obj);
    lpLookupName->LookupTuple.NbpName.ObjectNameLen = strlen(obj);
    strcpy_s(lpLookupName->LookupTuple.NbpName.TypeName, sizeof(lpLookupName->LookupTuple.NbpName.TypeName), type);
    lpLookupName->LookupTuple.NbpName.TypeNameLen = strlen(type);
    strcpy_s(lpLookupName->LookupTuple.NbpName.ZoneName, sizeof(lpLookupName->LookupTuple.NbpName.ZoneName),zone);
    lpLookupName->LookupTuple.NbpName.ZoneNameLen = strlen(zone);
   
    ret = getsockopt(s, SOL_APPLETALK, SO_LOOKUP_NAME, (char *)szLookup, &size);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(SO_LOOKUP_NAME) failed with error code %d\n", WSAGetLastError());
        return 1;
    }
    else
        printf("getsockopt(SO_LOOKUP_NAME) is OK!\n");
   
    lpLookup = (char *)szLookup + sizeof(WSH_LOOKUP_NAME);
    lpTuple  = (PWSH_NBP_TUPLE)lpLookup;
   
    printf("Found %d names!\n", lpLookupName->NoTuples);
    // Note that we only return the first name returned
    addr->sat_family = AF_APPLETALK;
    addr->sat_net = lpTuple[0].Address.Network;
    addr->sat_node = lpTuple[0].Address.Node;
    addr->sat_socket = lpTuple[0].Address.Socket;
   
    printf("addr->sat_family = AF_APPLETALK\n");
    printf("addr->sat_net    = %d\n", addr->sat_net);
    printf("addr->sat_node   = %d\n", addr->sat_node);
    printf("addr->sat_socket = %d\n", addr->sat_socket);
   
    return 0;
}

// Function: atalkrecv  
// Description:  
//    On a socket read as much data as there is pending. For the PAP
//    protocol this means reading until WSARecvEx() returns without
//    the MSG_PARTIAL flag set. For ADSP we simply request to read
//    the maximum buffer size. If an error occurs we return the
//    Winsock error; otherwise a zero is returned. For ADSP we are
//    assuming that the message from can be read on the first
//    attempt.
int atalkrecv(SOCKET s, char *recvbuff, int *nRead)
{
    int        iFlags=0, ret;
    DWORD      dwErr;
    BOOL       bDone=FALSE;
    struct fd_set fdread; *nRead = 0;
    dwErr = 0;
 
    if (iProto == ATPROTO_PAP)  
    {
        // For PAP we have to do a prime read first
		/* This call primes a read on a PAP connection 
		 * so the sender can start sending the data */
        ret = setsockopt(s, SOL_APPLETALK, SO_PAP_PRIME_READ, recvbuff, MAX_BUFFER);
        if (ret == SOCKET_ERROR)
        {
            printf("setsockopt(SO_PAP_PRIME_READ) failed with error code %d\n", (dwErr = WSAGetLastError()));
            return dwErr;
        }
        else
            printf("setsockopt(SO_PAP_PRIME_READ) pretty fine!\n");
      }
 
    FD_ZERO(&fdread);
    FD_SET(s, &fdread);
    select(0, &fdread, NULL, NULL, NULL);
 
    while (!bDone)
    {
        // Make sure we receive the whole message (PAP only)
        //  For the ADSP protocol, WSARecvEx() will never
        //  return the MSG_PARTIAL flag.
        //if((ret = WSARecv(s,xxxx,MAX_BUFFER,1,&recvbuff[(*nRead)],&iFlags,NULL, NULL== SOCKET_ERROR)
		/* 
		    The WSARecvEx function receives data from 
			a connected socket or a bound connectionless socket. 
			The WSARecvEx function is similar to the recv function, 
			except that the flags parameter is used only to return information. 
			When a partial message is received while using datagram protocol, 
			the MSG_PARTIAL bit is set in the flags parameter on return from the function.
		 */
        if ((ret = WSARecvEx(s, &recvbuff[(*nRead)], MAX_BUFFER, &iFlags)) == SOCKET_ERROR)
        {
            if ((dwErr = WSAGetLastError()) == WSAEWOULDBLOCK)
                break;
            else if (dwErr == WSAENOTCONN)
                break;
            printf("WSARecvEx() failed with error code %d\n", dwErr);
            return dwErr;
        }
        if (ret == 0)
        {
            printf("WSARecvEx() found a graceful close...\n");
            break;
        }
        if ((iFlags & MSG_PARTIAL) == 0)
            bDone = TRUE;
        (*nRead) += ret;
    }
    return dwErr;
}

// Function: atalksend
// Description:
//    Send the specified number of bytes from the buffer on the
//    given socket. Nothing special needs to be done for either
//    ADSP or PAP. For PAP, the message should only require one
//    send. For ADSP, it could require several send() calls as
//    it is stream oriented.
int atalksend(SOCKET s, char *sendbuf, int nSend)
{
	 DWORD        dwErr;
	 int          ret,nPos;
	 struct fd_set fdwrite;

	 dwErr = 0;
	 FD_ZERO(&fdwrite);
	 FD_SET(s, &fdwrite);
	 select(0, NULL, &fdwrite, NULL, NULL);

	 nPos = 0;
	 while (nSend > 0)
	 {
		 if ((ret = send(s, &sendbuf[nPos], nSend, 0)) == SOCKET_ERROR)
		 {
			 if ((dwErr = WSAGetLastError()) != WSAEWOULDBLOCK)
			 {
				 printf("send() failed with error code %d\n", dwErr);
			 }
			 return dwErr;
		 }
		 else
			   printf("send() is working!\n");

		 nSend -= ret;
		 nPos  += ret;
	 }
	 return dwErr;
}

// Function: ClientThread
// Description: A desc goes here
DWORD WINAPI ClientThread_apple(LPVOID lpParam)
{
     SOCKET        s = (SOCKET)lpParam;
     DWORD         i;
     char          recvbuff[MAX_BUFFER];
     int           nRead=0;
     u_long        optval=1;
     int           ret;
 
     // Make the socket non-blocking, error notification is easier this way
     if (ioctlsocket(s, FIONBIO, &optval) == SOCKET_ERROR)
     {
         printf("ioctlsocket(FIONBIO) failed with error code %d\n", WSAGetLastError());
         return 1;
     }
     else
         printf("ioctlsocket(FIONBIO)  is pretty damn OK!\n");
 
     // For PAP, receive and send the requested number of packets
     // For ADSP, receive and send until the client closed the connection
     i = 0;
     do {
         if ((ret = atalkrecv(s, recvbuff, &nRead)) != 0)
         {
             if (ret == WSAEWOULDBLOCK)
             {
                 printf("atalkrecv() returns WSAEWOULDBLOCK!\n");
                 continue;
             }
             else if (ret == WSAENOTCONN)
             {
                 printf("atalkrecv() returns WSAENOTCONN!\n");
                 break;
             }
             else
             {
                 printf("atalkrecv() failed with error code %d\n", ret);
                 break;
             }
         }
         i++;
 
         if (nRead > 0)
             printf("read %d bytes...\n", nRead);
 
         if ((ret = atalksend(s, recvbuff, nRead)) != 0)
         {
             if (ret == WSAEWOULDBLOCK)
             {
                 printf("atalksend() returns WSAEWOULDBLOCK!\n");
                 continue;
             }
             else
             {
                 printf("send() failed with error code %d\n", ret);
                 break;
             }
         }
         else
             printf("atalksend() is OK!\n");
 
         printf("wrote %d bytes...\n", nRead);
     } while (((i <= dwCount_apple) && (iProto == ATPROTO_PAP)) || (iProto == ATPROTO_ADSP));
    return 0;
}

// Function: main
// Description:
//    Load the Winsock library, parse the command line arguments,
//    create a socket of the requested type, and either send or
//    receive data. For the PAP protocol make sure we receive
//    an entire message by checking for the MSG_PARTIAL flag on
//    return from WSARecvEx().  For ADSP, the receiver will
//    continually loop until the connection is closed (as it is
//    a stream protocol).
int main_apple(int argc, char **argv)
{
     WSADATA       wsd;
     SOCKET        s;
     SOCKADDR_AT   ataddr;
     int           ret, nRead, ataddrsz=sizeof(ataddr);
 
     // Get things started by validating command line args,
     //  loading the Winsock lib, and creating a socket of the requested type.
     if(argc == 1)
     {
         usage_apple();
         return 1;
     }
     else
         ValidateArgs_apple(argc, argv);
 
     if (WSAStartup(MAKEWORD(1,1), &wsd) != 0)
     {
         printf("WSAStartup() failed!\n");
         return 1;
     }
     printf("WSAStartup() is OK!\n");

     printf("Copying the supplied arguments...\n");
     if (strcpy_s(szZone, sizeof(szZone), DEFAULT_ZONE) == 0)
         printf("Zone copied successfully!\n");
     else
         printf("Zone copy failed!\n");

     if(strcpy_s(szType, sizeof(szType), DEFAULT_TYPE) == 0)
         printf("Type copied successfully!\n");
     else
         printf("Type copy failed!\n");
 
     if(strcpy_s(szObject, sizeof(szObject), DEFAULT_OBJECT) == 0)
         printf("object copied successfully!\n");
     else
         printf("object copy failed!\n");
 
     s = socket(AF_APPLETALK, iSocketType_apple, iProto);
     if (s == INVALID_SOCKET)
     {
         printf("socket() failed with error code %d\n", WSAGetLastError());
         return 1;
     }
     else
         printf("socket() is OK!\n");
 
     // Open a port on the AppleTalk network
     ZeroMemory(&ataddr, sizeof(ataddr));
     ataddr.sat_family = AF_APPLETALK;
     ataddr.sat_socket = 0;
 
     if (bind(s, (SOCKADDR *)&ataddr, sizeof(ataddr)) == INVALID_SOCKET)
     {
         printf("bind() failed with error code %d\n", WSAGetLastError());
         return 1;
     }
     else
         printf("bind() is OK lol!\n");
 
     if (bSender)                       // we are sending packets
     {
         SOCKADDR_AT   ataddr;
         char          sendbuff[MAX_BUFFER];
         DWORD         i;
         u_long        optval=1;
 
         // Find the given name on the AppleTalk network so we can
         //  connect and send data to it!
         if (FindName_apple(s, szZone, szType, szObject, &ataddr) != 0)
         {
             printf("Unable to find a receiver!\n");
             return 1;
         }
         else
             printf("Receiver found!\n");
 
         // If we found a name, try to connect to it
         printf("Connecting to: %s:%s@%s\n", szObject, szType, szZone);
 
         if (connect(s, (SOCKADDR *)&ataddr, sizeof(ataddr)) == SOCKET_ERROR)
         {
             printf("connect() failed with error code %d\n", WSAGetLastError());
             return 1;
         }
         else
             printf("connect() is OK!\n");
 
         if (ioctlsocket(s, FIONBIO, &optval) == SOCKET_ERROR)
         {
             printf("ioctlsocket(FIONBIO) failed with error code %d\n", WSAGetLastError());  
             return 1;
         }
         else
            printf("ioctlsocket(FIONBIO) is OK\n");
 
         // Once we've connected, start sending data. Send the
         //  requested number of packets of the given size
         memset(sendbuff, '$', dwSendSize);
         i=0;
         do {
             if ((ret = atalksend(s, sendbuff, dwSendSize)) != 0)
             {
                 if (ret == WSAEWOULDBLOCK)
                 {
                     printf("Client would block send() lol!\n");
                     continue;
                 }
                 else
                 {
                     printf("send() failed with error code %d\n", ret);
                     break;
                 }
             }
             else
                     printf("atalksend() is working...\n");
             i++;
 
             printf("Wrote %d bytes...\n", dwSendSize);
 
             if ((ret = atalkrecv(s, sendbuff, &nRead)) != 0)
             {
                 if (ret == WSAEWOULDBLOCK)
                 {
                     printf("Client would block atalkrecv()...\n");
                     continue;
                 }
                 else
                 {
                     printf("atalkrecv() failed with error code %d\n", ret);
                     break;
                 }
             }
             printf("Read %d bytes...\n", nRead);
         } while (i < dwCount_apple);
     }
     else // we're receiving data
     {
         SOCKET        scli;
         BOOL          bDone=FALSE;
         HANDLE        hThread;
         DWORD         dwThreadId;
         struct fd_set fdread;
 
         // Register our name so others can connect to it
         if (RegisterName_apple(s, szZone, szType, szObject) != 0)
             return 1;
         else
             printf("RegisterName() looks fine!\n");
 
         listen(s, 8);
         printf("listen() should be fine!\n");
         printf("I'm listening you retard client...\n");
 
         while (!bDone)
         {
             FD_ZERO(&fdread);
             FD_SET(s, &fdread);
             select(0, &fdread, NULL, NULL, NULL);
 
             // Once we receive a connection accept it
             scli = accept(s, (SOCKADDR *)&ataddr, &ataddrsz);
             if (scli == INVALID_SOCKET)
             {
                 printf("accept() failed with error code %d\n", WSAGetLastError());
                 bDone = TRUE;
             }
             else
                  printf("accept() is OK!\n");
 
             hThread = CreateThread(NULL, 0, ClientThread_apple, (LPVOID)scli,0, &dwThreadId);
             if (hThread == NULL)
             {
                 printf("CreateThread() failed with error code %d\n", GetLastError());
                 bDone = TRUE;
             }
             else
                 printf("CreateThread() is OK!\n");
 
             CloseHandle(hThread);
         }
     }
     closesocket(s);
     WSACleanup();
 
     return 0;
}