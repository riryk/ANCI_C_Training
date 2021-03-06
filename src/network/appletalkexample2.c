// Copyright (c) 1994-1997  Microsoft Corporation
//
// Abstract: This Sample Windows Socket Application demonstrates use of
//          the AppleTalk PAP Protocol. Both a server and a client
//          application are included in this source file. The client
//          app reads a 'file' and sends the contents to the server. The
//          server writes out the received bytes to another file
//
// http://msdn.microsoft.com/en-us/library/ms737629(VS.85).aspx
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
 
#include <windows.h>
// Link to ws2_32.lib
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
// Link to          Mswsock.lib, for WSARecvEx()
#include <Mswsock.h>
 
// the following header contains appletalk specific definitions.
// should be included for AppleTalk Windows Socket Applications
#include <atalkwsh.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock.lib")


#define SOCKET_COUNT            32
#define DEF_SERVER_NAME         "Windows Adsp"
#define DEF_SERVER_TYPE         "Windows Sockets"
#define DEF_SERVER_ZONE         "*"
#define BLOCKSIZE               4096
#define DEF_QUEUE_LEN           5
#define ENTITY_LEN              32
 
CHAR    gServerName[ENTITY_LEN]         = DEF_SERVER_NAME;
CHAR    gServerType[ENTITY_LEN]         = DEF_SERVER_TYPE;
CHAR    gServerZone[ENTITY_LEN]         = DEF_SERVER_ZONE;
CHAR    gProtocolName[10]               = "ADSP";
INT     NumTimes                        = 5;
CHAR    gFileName[256];
 
LPSTR   pServerName   = gServerName;
LPSTR   pServerType   = gServerType;
LPSTR   pServerZone   = gServerZone;
LPSTR   pProtocolName = gProtocolName;
LPSTR   pFileName     = gFileName;
 
HANDLE  gFileHandle;
WSADATA WsaData;
 
SYSTEMTIME    SystemStartTime;
SYSTEMTIME    SystemEndTime;
FILETIME      FileTime;
 
BOOL fFileSpecified = FALSE;
BOOL fVerify = FALSE;
BOOL fRandom = FALSE;
BOOL fRDM    = FALSE;
 
int loop = 1, looptot;
double Throughput, AvgThroughPut, MinThroughPut, MaxThroughPut;
 
char *DataString = {"SenDing All my Love to YouUuuuuuuu! Gedik...tergedik-gedik"};
 
#define NANOPOWER    1000000000
 
//#define DEBUG
 
typedef struct Sock_Info {
   int     sock_typ;
   int     prototyp;
   SOCKET  nsock;
} *PTEST_INFO, test_info;
 
// Prototypes
BOOL DoServer_apple2();
BOOL DoClient_apple2();
BOOL OpenSocketRegisterAndListen_apple2();
unsigned long PerfThread_apple2(LPVOID lpvTest);
BOOL CheckForRecv_apple2(SOCKET s);
BOOL CheckForSend_apple2(SOCKET s);
void usage_apple2();
 
int main_apple2(int argc, char **argv)
{
    char p;
    //char *ptr;
    int i, r;
    BOOL fClient = FALSE;
    BOOL fServer = FALSE;
 
    for(i = 1; i < argc; i++)
    {
        p = argv[i][0];
        if (p == '/' || p == '-')    // option string
        {
            p = argv[i][1];
            switch(tolower(p))
            {
            case 's':
				fServer = TRUE;
				break;
            case 'c':
                fClient = TRUE;
                break;
            case 'n':
                if(i+1 < argc)
                {
                    strcpy_s(pServerName,sizeof(gServerName),argv[i+1]);
                    ++i;
                }
                break;
            case 't':
                if(i+1 < argc)
                {
                    strcpy_s(pServerType,sizeof(gServerType),argv[i+1]);
                    ++i;
                }
                break;
            case 'z':
                if(i+1 < argc)
                {
                    strcpy_s(pServerZone,sizeof(gServerZone),argv[i+1]);
                    ++i;
                }
                break;
            case 'p':
                if(i+1 < argc)
                {
                    if(strlen(argv[i+1]) >= 3)
                    {
                        if(_stricmp(argv[i+1],"ADSP") == 0)
                        {
							pProtocolName = argv[i+1];
							//iSocketType = SOCK_STREAM;
                        }
                        else if (_stricmp(argv[i+1],"PAP") == 0)
                        {
                            pProtocolName = argv[i+1];
                            //iSocketType = SOCK_RDM;
                        }
                    }
                    ++i;
                }
                break;
            case 'b':
                NumTimes = atoi(argv[i+1]);
                i++;
                break;
            case 'f':
                strcpy_s(pFileName,sizeof(gFileName),argv[i+1]);
                fFileSpecified = TRUE;
                i++;
                break;
            case 'l':
                loop = atoi(argv[i+1]);
                i++;
                break;
            case 'v':
                fVerify = TRUE;
                break;
            case 'r':
                fRandom = TRUE;
                break;
            case 'y':
                fRDM = TRUE;
                break;
            case '?':
            default:
                usage_apple2();
            }
        }
        else
        {
            usage_apple2();
        }
    }
   
    printf("Default/given Server Name \"%s\"\n", pServerName);
    printf("Default/given Server Type \"%s\"\n", pServerType);
    printf("Default/given Server Zone \"%s\"\n", pServerZone);
    printf("===The %s throughput measurement===\n", pProtocolName);
   
    if(_stricmp(pProtocolName, "ADSP") && _stricmp(pProtocolName, "PAP"))
    {
        printf("Invalid Protocol Specified!\n");
        exit(1);
    }

    if(!(fClient || fServer))
        usage_apple2();

    r = WSAStartup(0x0101, &WsaData);
    if (r == SOCKET_ERROR)
    {
        printf("Startup failed!\n");
        WSACleanup();
        return(0);
    }
           
    if(fClient)
    {
        BOOL DClnt;
       
        looptot = 0;
        MaxThroughPut = 0;
        AvgThroughPut = 0;

        if(fFileSpecified)
			printf("Ignoring -f option for client.\n");
        while (looptot < loop)
        {
            DClnt = DoClient_apple2();
            if(DClnt)
            {
                if ((looptot == 0) || (MinThroughPut > Throughput))
                    MinThroughPut = Throughput;
                if (MaxThroughPut < Throughput)
                    MaxThroughPut = Throughput;

                AvgThroughPut = ((AvgThroughPut * looptot) + Throughput) / (looptot+1);

                printf("==Loop %2d/%d Throughput Cur/Min/Max/Avg==\n", ++looptot, loop);
                printf("Cur Throughput: %6.2f\n", Throughput);
                printf("Min throughput: %6.2f\n", MinThroughPut);
                printf("Max throughput: %6.2f\n", MaxThroughPut);
                printf("Avg throughput: %6.2f\n", AvgThroughPut);
                // Reset
                Throughput = 0.0;
            }
            else
                break;
        }
    }
           
    if(fServer)
        DoServer_apple2();
    WSACleanup();
    return(0);
}
 
// Function - DoServer()
// Calling Function - main();
// Comments - This section of code calls the Server side of the application.
BOOL DoServer_apple2()
{
    BOOL   fRet = FALSE;
    do
    {
        // register the name specified on the command line  or use the defaults.
        fRet = OpenSocketRegisterAndListen_apple2();
    }while(FALSE);
    return(fRet);
}
 
// Function - OpenSocketRegisterAndListen()
// Calling Function - DoServer();
// Comments - This section of code controls the Server side of the application.
BOOL OpenSocketRegisterAndListen_apple2()
{
    int                  r = 0;         // return from socket calls
    int                  pass_no = 1;
    BOOL                 fRet = TRUE;
    SOCKET               sock, newsock; // SOCKET descriptor
    SOCKADDR_AT          atalkaddr;     // See atalkwsh.h
    WSH_REGISTER_NAME    nbpname;       // structure for registering NBP name, see atalkwsh.h
    fd_set               readfds;       // fd_set structure for select call
    int                  addrlen;
    PTEST_INFO           test;
    int                  sockettype = SOCK_STREAM;
    int                  protocoltype = ATPROTO_ADSP;
    int                  NumBytesRecvd = 0;
    unsigned long        ThreadId;
           
    // open a appletalk socket. The protocol family should be AF_APPLETALK,
    // the socket type can be SOCK_STREAM or SOCK_RDM, and the ProtocolType
    // for PAP socket must be ATPROTO_PAP. Note that opening a socket does
    // not yet create an endpoint on the AppleTalk Protocol. A bind must
    // happen before this socket can be used with AppleTalk;
    do
    {
        if(!_stricmp(pProtocolName, "PAP"))
        {
			sockettype = SOCK_RDM;
			protocoltype = ATPROTO_PAP;
        }
        else if (fRDM)
            sockettype = SOCK_RDM;
                       
        sock = socket(AF_APPLETALK, sockettype, protocoltype);
        if(sock == INVALID_SOCKET)
        {
            printf("Open Socket: Error = %ld\n", WSAGetLastError());
            r = -1;
            break;
        }
       
        atalkaddr.sat_socket = 0;
        atalkaddr.sat_family = AF_APPLETALK;
       
        r = bind(sock, (struct sockaddr *)&atalkaddr, sizeof(SOCKADDR_AT));
        if(r < 0)
        {
			printf("Bind:Error = %ld\n", WSAGetLastError());
			break;
        }
                       
        // end point has now been created on the AppleTalk Protocol
        // now register the name that the client will look up.
        strcpy_s(nbpname.ObjectName, sizeof(nbpname.ObjectName),pServerName);
        nbpname.ObjectNameLen = strlen(pServerName);
        strcpy_s(nbpname.TypeName, sizeof(nbpname.TypeName),pServerType);
        nbpname.TypeNameLen = strlen(pServerType);
        strcpy_s(nbpname.ZoneName, sizeof(nbpname.ZoneName),"*");
        nbpname.ZoneNameLen = strlen("*");
        printf("Registering %s:%s@%s\n", nbpname.ObjectName, nbpname.TypeName,nbpname.ZoneName);
       
        r = setsockopt(sock,               // socket descriptor
                    SOL_APPLETALK,         // socket level - always SOL_APPLETALK
                    SO_REGISTER_NAME,      // socket option
                    (char *)&nbpname,      // nbp name structure
                    sizeof(WSH_NBP_NAME)); // size of nbp name structure
                       
        if(r < 0)
        {
			printf("Register Name: Error = %d\n", WSAGetLastError());
			break;
        }
        else
            printf("Name registered!\n");
 
    }while(FALSE);
           
    if(r == SOCKET_ERROR)
    {
        closesocket(sock);
        return(FALSE);
    }

    // Post a listen on this socket. The default queue length is 5
    r =  listen(sock, DEF_QUEUE_LEN);
    if(r < 0)
    {
        printf("listen():Error = %d\n", WSAGetLastError());
        return FALSE;
    }
    else
        printf("I'm listening...\n");
 
    // listen is successful. select the socket for reading
    do
    {
        FD_ZERO(&readfds);            // clear set
        FD_SET(sock, &readfds);       // add sock to set
               
        // this is a blocking select. Select will complete when a client connects.
        r = select(0, &readfds, NULL, NULL, NULL);
        if(r == SOCKET_ERROR)
        {
            printf("select(): Error = %d\n", WSAGetLastError());
            fRet = FALSE;
            break;
        }
        if ( r == 0)
        {
            printf("select(): no sockets available\n");
            fRet = FALSE;
            break;
        }

        printf("select() should be OK!\n");
       
        // Accept an incoming request.
        addrlen = sizeof(SOCKADDR_AT);
        newsock = accept(sock, (struct sockaddr *)&atalkaddr, &addrlen);
        if(newsock == INVALID_SOCKET)
        {
            printf("accept(): Socket Error = %d\n", WSAGetLastError());
            fRet = FALSE;
            break;
        }
        printf("accept() Succeeded!\n");

		if(r == SOCKET_ERROR)
		{
			fRet = FALSE;
			break;
		}

        test = (PTEST_INFO) LocalAlloc(LPTR, sizeof(test_info));
        test->sock_typ = sockettype;
        test->prototyp = protocoltype;
        test->nsock = newsock;

        if( ! CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)PerfThread_apple2,(PTEST_INFO) test, 0, & ThreadId ) )
        {
            fprintf(stderr,"CreateThread NULL 0 PerfThread %d 0 %d 0 %lx",
                test->sock_typ, test->prototyp, (unsigned long) & ThreadId);
            exit(1);
        }

        if (fRet == FALSE)
            break;
    }while (TRUE);
 
    closesocket(sock);
    // send all done
    return(TRUE);
}
 
// Function - PerfThread()
// Calling Function - OpenSocketRegisterAndListen();
// Comments - This section of code controls the Server side Recieve and Verification
// for the application
unsigned long PerfThread_apple2( PTEST_INFO lpvTest )
{
    int     r;                          // return from socket calls
    int     recvflags = 0;              // see WSARecvEx call
    CHAR    recvbuf[4096];              // recv buffer
    long    blockmode = 1;              // see ioctlsocket call
    BOOL    fRet = TRUE;
    int     NumBytesRecvd = 0, pass_no = 1;
    SOCKET  newsock = lpvTest->nsock;   // SOCKET descriptor
    int     sockettype = lpvTest->sock_typ;
    int     protocoltype = lpvTest->prototyp;
 
    // make socket non blocking. As far as possible, use non
    // blocking sockets to improve performance of app
    r = ioctlsocket(newsock, FIONBIO, (ULONG *)&blockmode);
    if( r == SOCKET_ERROR)
    {
        printf("ioctlsocket(FIONBIO): error = %d\n", WSAGetLastError());
        fRet = FALSE;
        return(FALSE);
    }
    else
        printf("ioctlsocket(FIONBIO) is fine!\n");
 
    do
    {
        // Prime a Read here. This will enable the receive to complete
        // This is PAP specific. For other AppleTalk protocols - ADSP
        // and DDP a recv can be posted directly
        if(!_stricmp(pProtocolName, "PAP"))
        {
			/* This call primes a read on a PAP connection 
			 * so the sender can start sending the data */
            r = setsockopt(newsock,SOL_APPLETALK,SO_PAP_PRIME_READ,recvbuf,sizeof(recvbuf));
            if( r < 0)
            {
                printf("setsockopt(SO_PAP_PRIME_READ): error = %d\n", WSAGetLastError());
                fRet = FALSE;
                break;
            }
            else
                printf("setsockopt(SO_PAP_PRIME_READ) is OK!\n");
        }
 
        fRet = CheckForRecv_apple2(newsock);
        if(fRet == FALSE)
            break;
 
        r = WSARecvEx(newsock,recvbuf,sizeof(recvbuf), &recvflags);
        if ((sockettype == SOCK_RDM) && (r < sizeof(recvbuf)))
        {
			printf("WSARecvEx() - Did not receive entire message!\n");
			fRet = FALSE;
			break;
        }
        if( r == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAEWOULDBLOCK)
                continue;
            else
            {
                if(WSAGetLastError() == WSAENOTCONN || WSAGetLastError() == WSAEFAULT)
                            break;
                printf("WSARecvEx(): error = %d\n", WSAGetLastError());
                fRet = FALSE;
                break;
            }
        }
        printf("WSARecvEx() should be fine!\n");
        printf("Received %d bytes\n", r);
                       
        if (fVerify)
        {
            int rr, pos = 0;

            do
            {
                fRet = CheckForSend_apple2(newsock);
                if (fRet == FALSE)
                {
					printf("send select error\n");
					break;
                }

                rr = send(newsock, &recvbuf[pos], r-pos, 0);
                if ( rr < 0 )
                {
                    if(WSAGetLastError() == WSAEWOULDBLOCK)
                    {
                        printf("Send: error = WSAEWOULDBLOCK\n");
                        continue;
                    }
                    else
                    {
                        printf("send: error = %d\n", WSAGetLastError());
                        break;
                    }
                }
                printf("Bytes Sent %d\n", rr);

                pos += rr;
                if (pos >= r)
                    break;
            } while(TRUE);
        }  // endif Verify

        NumBytesRecvd += r;

        // reset the recv flags for the next WSARecvEx
        recvflags = 0;
    }
	while(TRUE);

    printf("Total Number of Bytes Received = %d \n",NumBytesRecvd);
    closesocket(newsock);
    LocalFree(lpvTest);
    return(TRUE);
}
 
// Function - DoClient()
// Calling Function - main();
// Comments - This section of code controls the Client side of the application.
BOOL DoClient_apple2()
{
    int                  r = 0;             // return code
    BOOL                 fRet = FALSE;      //
    int                  charcnt;           // count of bytes read from file
    CHAR                 LookupBuffer[512]; // Lookup results return buffer
    PCHAR                ptupleBuffer;
    PWSH_LOOKUP_NAME     pLookup;
    PWSH_NBP_TUPLE       pTempTuple;
    SOCKADDR_AT          ataddress;
    DWORD                WrittenSize;
    SOCKET               clntsock;
    long                 blockmode = 1;          // for ioctlsocket
    int                  BytesTransferred = 0;
    HLOCAL               memhandle;
    LPSTR                DataPtr, DataStartPtr, DataEndPtr;
    DWORD                hourdiff;          //, minutediff, seconddiff;
    double               StartSecond, EndSecond,ElapsedSeconds;
    int                  sockettype = SOCK_STREAM;
    int                  prototype = ATPROTO_ADSP;
    int                  i, rndbuf;
 
    if(!_stricmp(pProtocolName, "PAP"))
    {
		sockettype = SOCK_RDM;
		prototype = ATPROTO_PAP;
    }
    else if (fRDM)
        sockettype = SOCK_RDM;
    if (fRandom)
        rndbuf = rand() % strlen(DataString) + 1;
    else
        rndbuf = strlen(DataString);
 
    do
    {
        // See socket call in OpenSocketRegisterAndListen for desc
        clntsock = socket(AF_APPLETALK, sockettype, prototype);

        if(clntsock == INVALID_SOCKET)
        {
            printf("Open Socket: Error = %ld\n", WSAGetLastError());
            r = -1;
            break;
        }
        else
            printf("socket() is OK!\n");
 
        ataddress.sat_socket = 0;
        ataddress.sat_family = AF_APPLETALK;
 
        r = bind(clntsock, (struct sockaddr *)&ataddress, sizeof(SOCKADDR_AT));
        if(r < 0)
        {
			printf("bind():Error = %d\n", WSAGetLastError());
			break;
        }
        else
            printf("bind() is OK!\n");
 
        // end point has now been created on the AppleTalk Protocol.
        // lookup the server name.
        pLookup = (PWSH_LOOKUP_NAME)LookupBuffer;
        strcpy_s(pLookup->LookupTuple.NbpName.ObjectName, sizeof(pLookup->LookupTuple.NbpName.ObjectName),pServerName);
        pLookup->LookupTuple.NbpName.ObjectNameLen = strlen(pServerName);

        strcpy_s(pLookup->LookupTuple.NbpName.TypeName,sizeof(pLookup->LookupTuple.NbpName.TypeName),pServerType);
        pLookup->LookupTuple.NbpName.TypeNameLen = strlen(pServerType);

        strcpy_s(pLookup->LookupTuple.NbpName.ZoneName,sizeof(pLookup->LookupTuple.NbpName.ZoneName), pServerZone);
        pLookup->LookupTuple.NbpName.ZoneNameLen = strlen(pServerZone);
 
        printf("Looking up %s:%s@%s\n",
			pLookup->LookupTuple.NbpName.ObjectName,
            pLookup->LookupTuple.NbpName.TypeName,
            pLookup->LookupTuple.NbpName.ZoneName);

        WrittenSize = sizeof(LookupBuffer);
        r = getsockopt(clntsock, SOL_APPLETALK, SO_LOOKUP_NAME,(char*)LookupBuffer, (int *)&WrittenSize);

        if(r != NO_ERROR)
        {
			printf("getsockopt():error = %d\n", WSAGetLastError());
			break;
        }
        else
            printf("getsockopt() is OK!\n");
 
        if (pLookup->NoTuples <= 0)
        {
			printf("LookupName failed - no tuples found\n");
			r=-1;
			break;
        }
 
        ptupleBuffer = (char *)pLookup+sizeof(WSH_LOOKUP_NAME);
        pTempTuple = (PWSH_NBP_TUPLE)ptupleBuffer;
        ataddress.sat_family = AF_APPLETALK;
        ataddress.sat_net = pTempTuple[0].Address.Network;
        ataddress.sat_node = pTempTuple[0].Address.Node;
        ataddress.sat_socket = pTempTuple[0].Address.Socket;
 
        printf("server address = %lx.%lx.%lx.\n", 
			ataddress.sat_net,
            ataddress.sat_node,
            ataddress.sat_socket);
 
        // lookup succeeded. Use the address in ataddress to connect to the server
        r =  connect(clntsock, (struct sockaddr *)&ataddress, sizeof(SOCKADDR_AT));
        if(r < 0)
        {
			printf("connect: error = %d\n", WSAGetLastError());
			break;
        }
        printf("Connect Succeeded\n");
    }
	while(FALSE);
 
    if(r < 0)
    {
		closesocket(clntsock);
		return(FALSE);
    }
 
    //  Set Socket to non blocking mode
    r = ioctlsocket(clntsock, FIONBIO, (ULONG *)&blockmode);

    if( r == SOCKET_ERROR)
    {
        printf("ioctlsocket(): error = %d\n", WSAGetLastError());
        return FALSE;
    }
    else
        printf("ioctlsocket() Succeeded!\n");

    // Fill Up Send Buffer with Data
    memhandle = LocalAlloc(LPTR, BLOCKSIZE);
    if(memhandle == NULL)
    {
		printf("LocalAlloc() failed %d\n", GetLastError());
		return(FALSE);
    }
    else
        printf("LocalAlloc() is OK!\n");

    DataPtr = (LPSTR)LocalLock(memhandle);
    DataStartPtr = DataPtr;
    DataEndPtr = DataPtr + LocalSize(memhandle);
 
    // fill the buffer with Data
    while((DataStartPtr + rndbuf) <= DataEndPtr)
    {
        memcpy(DataStartPtr, DataString,rndbuf);
        DataStartPtr+= rndbuf;
    }
 
    if(DataStartPtr != DataEndPtr)
        memcpy(DataStartPtr, DataString, DataEndPtr-DataStartPtr-1);
    *DataEndPtr = '\0';
 
    fRet = CheckForSend_apple2(clntsock);
    if(fRet == FALSE)
        return FALSE;

    GetLocalTime(&SystemStartTime);

    printf("\nData Size = %d\n", lstrlen((LPCWSTR)DataPtr));
    printf("Base Data sent: %s\n",DataStartPtr);
    printf("Start Time:%d:%2d:%2d:%2d\t", 
		     SystemStartTime.wHour,
             SystemStartTime.wMinute,
             SystemStartTime.wSecond,
             SystemStartTime.wMilliseconds);
 
    for( i= 0; i < NumTimes; i++)
    {
		// Can I send - CheckForSend calls select to find if we can send without blocking
		DataStartPtr = DataPtr;

        do
        {
			fRet = CheckForSend_apple2(clntsock);
            if(fRet == FALSE)
            {
				printf("CheckForSend() error!\n");
				break;
            }
            else
                printf("CheckForSend() is OK!\n");

            charcnt = DataEndPtr - DataStartPtr;
            if(charcnt == 0)
                break;
            r = send(clntsock, DataStartPtr, charcnt, 0);
            if ( r < 0 )
            {
                if(WSAGetLastError() == WSAEWOULDBLOCK)
                {
                    printf("send(): wouldblock\n");
                    continue;
                }
                else
                {
                    printf("send(): error = %d\n", WSAGetLastError());
                    fRet = FALSE;
                    break;
                }
            }
            else
            {
                printf("send() is OK!\n");
                printf("Sent %d bytes\n",r);
                if ((fVerify) && (DataStartPtr + r == DataEndPtr))
                {
					CHAR buffer[4096];
					int recvflag = 0;
					int rr, pos = 0;
 
                    // Prime a Read here. This will enable the receive to complete
                    // This is PAP specific. For other AppleTalk protocols -
                    // ADSP and DDP a recv can be posted directly
                    if(!_stricmp(pProtocolName, "PAP"))
                    {
                        rr = setsockopt(clntsock,
                                    SOL_APPLETALK,
                                    SO_PAP_PRIME_READ,
                                    buffer,
                                    sizeof(buffer));
                        if( rr < 0)
                        {
							printf("setsockopt(SO_PAP_PRIME_READ): error = %d\n", WSAGetLastError());
							fRet = FALSE;
							break;
                        }
                    }

                    do
                    {
                        fRet = CheckForRecv_apple2(clntsock);
                        if(fRet == FALSE)
                            break;

                        rr = WSARecvEx(clntsock, &buffer[pos], sizeof(buffer)-pos, &recvflag);
                        if( rr == SOCKET_ERROR)
                        {
                            if (WSAGetLastError() == WSAEWOULDBLOCK)
								continue;
                            else
                            {
								printf("WSARecvEx(): error = %d\n", WSAGetLastError());
								fRet = FALSE;
								break;
                            }
                        }
                        else
                            printf("WSARecvEx() is OK!\n");
                        pos += rr;

                        printf("Recv'd %d bytes...\n",rr);
                        if ((recvflag == 0) || (sockettype == SOCK_RDM) || (pos == sizeof(buffer)))
                           break;
                        recvflag = 0;
                    } 
					while(TRUE);
 
					if ((memcmp(DataPtr, buffer, 4096) != 0) && (fRet != FALSE))
					{
						printf("Strings do not compare\n");
						printf("Data sent: \n%s\n",DataPtr);
						printf("Data recv'd: \n%s\n",buffer);
					}
                } // IF (fVerify)
 
                BytesTransferred += r;
                DataStartPtr += r;

                if(DataStartPtr == DataEndPtr)
                   break;
            } // IF send() PASSED
       }
	   while(TRUE);
 
       if(fRet == FALSE)
          break;
	} // FOR
	 
	GetLocalTime(&SystemEndTime);
	 
	printf("End Time:%d:%2d:%2d:%2d\t", 
			  SystemEndTime.wHour,
			  SystemEndTime.wMinute,
			  SystemEndTime.wSecond,
			  SystemEndTime.wMilliseconds);
	 
	// calculate the difference
	hourdiff = SystemEndTime.wHour - SystemStartTime.wHour;
	StartSecond = (SystemStartTime.wHour * 3600) +
	(SystemStartTime.wMinute * 60) +
	SystemStartTime.wSecond +
	(SystemStartTime.wMilliseconds * 0.001);
	 
	EndSecond = 0;
	if(SystemEndTime.wMonth != SystemStartTime.wMonth)
	{
		EndSecond = (SystemEndTime.wDay * 24) * 3600;
		switch (SystemStartTime.wMonth)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			EndSecond = ((31 - SystemStartTime.wDay) * 24) + EndSecond;
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			EndSecond = ((30 - SystemStartTime.wDay) * 24) + EndSecond;
			break;
		case 2:
			if((SystemStartTime.wYear % 400 == 0) ||
			   ((SystemStartTime.wYear % 4 == 0) &&
				(SystemStartTime.wYear % 100 != 0)))
				  EndSecond = ((29 - SystemStartTime.wDay) *24) + EndSecond;
			else
				  EndSecond = ((28 - SystemStartTime.wDay) * 24) + EndSecond;
			break;
		}
	}
	else if(SystemEndTime.wDay > SystemStartTime.wDay)
	{
		EndSecond = ((SystemEndTime.wDay - SystemStartTime.wDay) * 24) * 3600;
	}
	 
	EndSecond = EndSecond 
	+ (SystemEndTime.wHour * 3600)
	+ (SystemEndTime.wMinute * 60) 
	+  SystemEndTime.wSecond 
	+ (SystemEndTime.wMilliseconds * 0.001);
	 
	ElapsedSeconds = EndSecond-StartSecond;
	 
	printf("Elapsed Time (secs) = %6.3f\n", ElapsedSeconds);
	printf("Bytes Transferred = %ld\n", BytesTransferred);
	printf("Send - All Done\n");
	 
	if(ElapsedSeconds !=0)
	{
		Throughput = (BytesTransferred)/(ElapsedSeconds) ;
		printf("Throughput (bytes/sec) = %6.2f\n", Throughput);
	}
	printf("\n");
	closesocket(clntsock);
	LocalFree(memhandle);
	return(TRUE);
}
 
BOOL CheckForSend_apple2(SOCKET s)
{
    fd_set writefds;
    int r;

    printf("\nIn CheckForSend()...\n");
    FD_ZERO(&writefds);
    FD_SET(s, &writefds);

    // indefinite wait select
    r = select(0, NULL, &writefds, NULL, NULL);
    if( r != 1)
    {
		printf("select():error = %d\n", WSAGetLastError());
		return(FALSE);
    }
    else
        printf("select() is OK!\n");

    return(TRUE);
}
 
BOOL CheckForRecv_apple2(SOCKET s)
{
    fd_set readfds;
    int r;

    FD_ZERO(&readfds);
    FD_SET(s, &readfds);

    printf("\nIn CheckForRecv()...\n");

    r =  select(0, &readfds, NULL, NULL, NULL);
    if( r != 1)
    {
        printf("select():error = %d\n", WSAGetLastError());
        return(FALSE);
    }
    else
        printf("select() is OK!\n");
    return(TRUE);
}
 
void usage_apple2()
{
    printf("Appletalkbodo -s/c <options>\n");
    printf("\t-s|-c\t\t\tServer/Client\n");
    printf("\t-n <server_name>\tServer Name\n");
    printf("\t-t <server_type>\tServer Type \n");
    printf("\t-z <server_zone>\tServer Zone \n");
    printf("\t-p ADSP|PAP\t\tProtocol Name (ADSP/PAP)\n");
    printf("\t-f <file_name>\t\tFile Name for data to be saved (server only) \n");
    printf("\t-b <int>\t\tNumber of times to send a 4k buffer (Default = 5) \n");
    printf("\t-l <int>\t\tNumber of times to re-start Appletalkbodo\n");
    printf("\t\t\t\t(Default = 1) (Client only)\n");
    printf("\t-v\t\t\tVerify Integraty of data transmission. \n");
    printf("\t-r\t\t\tRandomize the size of the base data string. \n");
    printf("\t-y\t\t\tChange ADSP to use SOCK_RDM instead of SOCK_STREAM. \n");
    printf("Otherwise default values will be used...\n");
    printf("Server example: Appletalkbodo -s -n mikeblur -t gedik -z * -p PAP -f fck.txt\n");
    printf("Client example: Appletalkbodo -c -n mikeblur -t gedik -z * -p PAP\n");
    printf("\n");
    exit(1);
}