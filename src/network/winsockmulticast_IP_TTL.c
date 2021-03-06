
#include "winsocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <rpc.h>
#include <af_irda.h>
#include <stdio.h>
#include <wsnetbs.h>
#include <winsock.h>
#include <atalkwsh.h>
#include <ws2atm.h>
#include <Mstcpip.h>
#include "winsockmulticast_IP_TTL.h"




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


// Description:
//    This is a simple multicast application which illustrates
//    the use of the IP_TTL option to modify the time-to-live
//    field of the IP header similar to the SIO_MULTIPOINT_SCOPE
//    ioctl does except this applies to any kind of IP traffic.
//
// Command line arguments:
//    ip_multicast_ttl [s|r] ttl
//      s        Sender
//      r        Receiver
//      ttl      Integer TTL value
//
// Vista issue: http://connect.microsoft.com/VisualStudio/feedback/ViewFeedback.aspx?FeedbackID=329162
//
// A program that includes Winsock.h should only link with Wsock32.lib
//

#define MAX_BUF             64
 
// Change the IP accordingly for testing
// Multicast address range 224.0.0.0 - 239.255.255.255
// http://www.iana.org/assignments/multicast-addresses/
#define MULTICAST_IP        "127.0.0.1"//"192.168.1.1"
#define MULTICAST_PORT      24000


// Function: usage
// Description: Prints usage information
int usage_multicast(char *progname)
{
    printf("Usage: %s s|r ttl\n", progname);
    printf("          s = sender\n");
    printf("          r = receiver\n");
    printf("        ttl = multicast TTL value\n");
    printf("Example: %s s 100\n", progname);
    printf("Example: %s r 100\n", progname);
    return 0;
}

// Function: main
// Description:
//    Load Winsock, parse the arguments and start either the
//    multicast sender or receiver. 
//    Before sending data set the IP_TTL to the specified value.
void main_multicast(BOOL bReceive/*int argc, char **argv*/)
{
    WSADATA        wsd;
    SOCKET         s;
    // struct ipv6_mreq for IPv6
    struct ip_mreq mcast; // IPv4
    // or better one, use struct ip_mreq_source/IP_MREQ_SOURCE/*PIP_MREQ_SOURCE
    // http://msdn.microsoft.com/en-us/library/ms738704(VS.85).aspx
    // Vista or later use GROUP_REQ and the GROUP_SOURCE_REQ structures which for both IPv6 and IPv4
    SOCKADDR_IN    local, from;
    int            ttl, ttlsz, fromsz, ret;
    char           databuf[MAX_BUF];
    // BOOL           bReceive=FALSE;
 
    // Parse the command line
    /*if (argc < 2)
    {
        usage_multicast(argv[0]);
        exit(1);
    }*/

	/*if (tolower(argv[1][0]) == 's')
        bReceive = FALSE;
    else if (tolower(argv[1][0]) == 'r')
        bReceive = TRUE;
    else
        usage_multicast(argv[0]); */
	
    ttl = 12; //atoi(argv[2]);

	// Load winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd))
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s == INVALID_SOCKET)
    {
        printf("socket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("socket() is OK!\n");

    if (bReceive)
    {
        local.sin_family = AF_INET;
        local.sin_port = htons(MULTICAST_PORT);
        local.sin_addr.s_addr = INADDR_ANY;
 
        if (bind(s, (SOCKADDR *)&local, sizeof(local)) == SOCKET_ERROR)
        {
            printf("bind() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
            printf("bind() is OK!\n");
    }

	// Join the multicast group
    // For XP sp2 and Windows 2003 not needed in order to send,
    // only the receivers need to join a multicast group in order to receive
    if(bReceive)
    {
        mcast.imr_multiaddr.s_addr = inet_addr(MULTICAST_IP);
        mcast.imr_interface.s_addr = INADDR_ANY;

        if (setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mcast, sizeof(mcast)) == SOCKET_ERROR)
        {
            printf("setsockopt(IP_ADD_MEMBERSHIP) failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
            printf("setsockopt(IP_ADD_MEMBERSHIP) is OK!\n");
	}

    // Set the TTL to our value
    ttlsz = sizeof(ttl);
    if (setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, sizeof(ttl)) == SOCKET_ERROR)
    {
        printf("setsockopt(IP_MULTICAST_TTL) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("setsockopt(IP_MULTICAST_TTL) is OK!\n");

    // Verify
    if (getsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, &ttlsz) == SOCKET_ERROR)
    {
        printf("getsockopt(IP_MULTICAST_TTL) failed: %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("getsockopt(IP_MULTICAST_TTL) is OK!\n");
 
    printf("Multicast TTL is set to: %d\n", ttl); 

    if (bReceive)
    {
        // Receive some data
        fromsz = sizeof(from);
        ret = recvfrom(s, databuf, MAX_BUF, 0, (SOCKADDR *)&from, &fromsz);
        if (ret == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
            printf("recvfrom() is OK!\n");
 
        databuf[ret] = 0;
        printf("read: [%s] from [%s]\n", databuf, inet_ntoa(from.sin_addr));
    }
    else
    {
        // Send some data
        SOCKADDR_IN to;
 
        memset(databuf, '$', MAX_BUF);
 
        to.sin_family = AF_INET;
        to.sin_port = htons(MULTICAST_PORT);
        to.sin_addr.s_addr = inet_addr(MULTICAST_IP);
 
        ret = sendto(s, databuf, MAX_BUF-1, 0, (SOCKADDR *)&to, sizeof(to));
 
        if (ret == SOCKET_ERROR)
        {
            printf("sendto() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
            printf("sendto() is OK!\n");
        printf("Sent %d bytes to %s\n", ret, inet_ntoa(to.sin_addr));
    }

    // Cleanup
    if(closesocket(s) == 0)
        printf("closesocket() should be fine!\n");
    else
        printf("closesocket() failed with error code %d\n", WSAGetLastError());
 
    if(WSACleanup() == 0)
        printf("WSACleanup() is OK!\n");
    else
        printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
}

void main_multicast_receive()
{
    main_multicast(TRUE);
}

void main_multicast_send()
{
    main_multicast(FALSE);
}

int main_address_list()
{
    WSADATA       wsd;
    SOCKET        s;
    DWORD         dwBytesRet=0;
    char          buf[1024];
    int           ret, i;
    SOCKET_ADDRESS_LIST *slist;
    SOCKADDR_IN  *addr;
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is OK!\n");
 
    // Get the address list
    ret = WSAIoctl(s, SIO_ADDRESS_LIST_QUERY, NULL, 0, buf, 1024, &dwBytesRet, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoctl(SIO_ADDRESS_LIST_QUERY) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAIoctl(SIO_ADDRESS_LIST_QUERY) is OK!\n");
 
    printf("Bytes returned: %d bytes\n", dwBytesRet);
    // Print the results
    slist = (SOCKET_ADDRESS_LIST *)buf;
    printf("      Addr Count: %d\n", slist->iAddressCount);
 
    for(i=0; i < slist->iAddressCount ;i++)
    {
        addr = (SOCKADDR_IN *) slist->Address[i].lpSockaddr;
        printf("       Addr [%02d]: %s\n", i, inet_ntoa(addr->sin_addr));
        printf("Addr Length [%02d]: %d bytes\n", i, slist->Address[i].iSockaddrLength);
    }
    // Cleanup
    if(closesocket(s) == 0)
        printf("closesocket() should be fine!\n");
    else
        printf("closesocket() failed with error code %d\n", WSAGetLastError());
    if(WSACleanup() == 0)
        printf("WSACleanup() is OK!\n");
    else
        printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}

// Description:
//    This sample sets the Type-of-Service bit and sends
//    some datagrams. To really see the results of this
//    option you need to capture the packets with Microsoft
//    Network Monitor (netmon) to view the actual packets.
//
// Command line arguments
//    Ip_tos ip
//                      ip         IP Address
//
// Link to ws2_32.lib
//
// Function: main
// Description:
//    Load Winsock, create a datagram socket, set the IP_TOS
//    option and send some packets.

int main_ip_tos(int argc, char **argv)
{ 
    WSADATA       wsd;
    SOCKET        s;
    SOCKADDR_IN   remote;
    char         *str="This is a test string from sender!";
    int           ret, iTos;
 
    if (argc < 2)
    {
        printf("Usage: %s <IP_Address_to_send>\n", argv[0]);
        printf("Example %s 74.125.45.100\n", argv[0]);
        exit(1);
    }
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() is pretty fine!\n");  

    // Create a socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0,WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is OK!\n");

	// Set the TOS bit
    iTos = 1;
    ret = setsockopt(s, IPPROTO_IP, IP_TOS, (char *)&iTos, sizeof(iTos));
    if (ret == SOCKET_ERROR)
    {
        printf("setsockopt(IP_TOS) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("setsockopt(IP_TOS) should be OK!\n");

    // Perform a sendto()
    printf("Sending all my love....\n");
    remote.sin_family = AF_INET;
    remote.sin_addr.s_addr = inet_addr(argv[1]);
    remote.sin_port = htons(5150);
 
    ret = sendto(s, str, strlen(str), 0, (SOCKADDR *)&remote, sizeof(remote));
    if (ret == SOCKET_ERROR)
    {
        printf("sendto() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("sendto() is OK!\n");
 
    printf("Wrote %d bytes...\n", ret);
    // Cleanup
    if(closesocket(s) == 0)
         printf("closesocket() should be fine!\n");
    else
         printf("closesocket() failed with error code %d\n", WSAGetLastError());
 
    if(WSACleanup() == 0)
          printf("WSACleanup() is OK!\n");
    else
          printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}

// Description:
//    This sample shows how to use the SIO_ADDRESS_LIST_QUERY
//    ioctl. This is a Windows 2000 command which lists the
//    local interfaces availability.
//
// Link to ws2_32.lib
int main_addr_list_query(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    DWORD         dwBytesRet=0;
    char          buf[1024];
    int           ret, i;
    SOCKET_ADDRESS_LIST *slist;
    SOCKADDR_IN  *addr;
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is OK!\n");
 
    // Get the address list
    ret = WSAIoctl(s, SIO_ADDRESS_LIST_QUERY, NULL, 0, buf, 1024, &dwBytesRet, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoctl(SIO_ADDRESS_LIST_QUERY) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAIoctl(SIO_ADDRESS_LIST_QUERY) is OK!\n");
 
    printf("Bytes returned: %d bytes\n", dwBytesRet);
    // Print the results
    slist = (SOCKET_ADDRESS_LIST *)buf;
    printf("      Addr Count: %d\n", slist->iAddressCount);
 
    for(i=0; i < slist->iAddressCount ;i++)
    {
        addr = (SOCKADDR_IN *) slist->Address[i].lpSockaddr;
        printf("       Addr [%02d]: %s\n", i, inet_ntoa(addr->sin_addr));
        printf("Addr Length [%02d]: %d bytes\n", i, slist->Address[i].iSockaddrLength);
    }
    // Cleanup
    if(closesocket(s) == 0)
        printf("closesocket() should be fine!\n");
    else
        printf("closesocket() failed with error code %d\n", WSAGetLastError());
    if(WSACleanup() == 0)
        printf("WSACleanup() is OK!\n");
    else
        printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;     
}

UCHAR BtoH( CHAR ch );
 
int  GetNumATMInterfaces(SOCKET s)
{
    DWORD  dwNum, nbytes = sizeof(DWORD);
 
    if (WSAIoctl(s, SIO_GET_NUMBER_OF_ATM_DEVICES, NULL, 0,
        (LPVOID)&dwNum, sizeof(DWORD), &nbytes, NULL, NULL) == SOCKET_ERROR)
    {
        return -1;
    }
    return dwNum;
}
 
BOOL GetATMAddress(SOCKET s, int device, ATM_ADDRESS *atmaddr)
{
    DWORD bytes;
 
    if (WSAIoctl(s, SIO_GET_ATM_ADDRESS, (LPVOID)&device, sizeof(DWORD),
        (LPVOID)atmaddr, sizeof(ATM_ADDRESS), &bytes, NULL, NULL) == SOCKET_ERROR)
    {
        return FALSE;
    }
    return TRUE;
}
 
BOOL FindProtocol1(WSAPROTOCOL_INFO *lpProto)
{
    WSAPROTOCOL_INFO *lpProtocolBuf=NULL;
    DWORD             dwErr, dwRet, dwBufLen=0, i;
 
    if (WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen) != SOCKET_ERROR)
    {
        // This should never happen as there is a NULL buffer
        printf("WSAEnumProtocols() failed!\n");
        return FALSE;
     }
    else if ((dwErr = WSAGetLastError()) != WSAENOBUFS)
    {
         // We failed for some reason not relating to buffer size - also odd
         printf("WSAEnumProtocols() failed with error code %d\n", dwErr);
         return FALSE;
    }
    else
         printf("WSAEnumProtocols() should be fine!\n");
 
    // Allocate the correct buffer size for WSAEnumProtocols as well as the buffer to return
    lpProtocolBuf = (WSAPROTOCOL_INFO *)GlobalAlloc(GMEM_FIXED, dwBufLen);
 
    if (lpProtocolBuf == NULL)
    {
          printf("GlobalAlloc() for protocol buffer failed with error code %d\n", GetLastError());
          return FALSE;
    }
    else
          printf("GlobalAlloc() is OK for protocol buffer...\n");
 
    dwRet = WSAEnumProtocols(NULL, lpProtocolBuf, &dwBufLen);
    if (dwRet == SOCKET_ERROR)
    {
          printf("WSAEnumProtocols() failed wit herror code %d\n", WSAGetLastError());
          GlobalFree(lpProtocolBuf);
          return FALSE;
    }
    else
         printf("WSAEnumProtocols() is OK!\n");
 
    // Loop through the returned protocol information 
	// looking for those that are in the AF_ATM address family.
    for (i=0; i < dwRet ;i++)
    {
          printf("Protocol: %S\n", lpProtocolBuf[i].szProtocol);
          if ( (lpProtocolBuf[i].iAddressFamily == AF_ATM) && (lpProtocolBuf[i].iSocketType == SOCK_RAW) &&
             (lpProtocolBuf[i].iProtocol == ATMPROTO_AAL5) )
        {
            memcpy(lpProto, &lpProtocolBuf[i], sizeof(WSAPROTOCOL_INFO));
            GlobalFree(lpProtocolBuf);
            return TRUE;
        }
    }
    GlobalFree(lpProtocolBuf);
    return FALSE;
}
 
// AtoH () coverts the ATM address specified in the string (ascii) format to the binary(hexadecimal) format.
void AtoH( CHAR *szDest, CHAR *szSource, INT iCount )
{
    while (iCount--)
    {
        *szDest++ = ( BtoH ( *szSource++ ) << 4 ) + BtoH ( *szSource++ );
    }
    return;
}
 
// BtoH () returns the equivalent binary value for an individual character specified in the ascii format.
UCHAR BtoH( CHAR ch )
{
    if ( ch >= '0' && ch <= '9' )
    {
        return ( ch - '0' );
    }
 
    if ( ch >= 'A' && ch <= 'F' )
    {
        return ( ch - 'A' + 0xA );
    }
 
    if (ch >= 'a' && ch <= 'f' )
    {
        return ( ch - 'a' + 0xA );
    }
    // Illegal values in the address will not be excepted
    return -1;
}

int main_get_atm_address(int argc, char **argv)
{
    WSADATA      wsd;
    SOCKET       s;
    DWORD        deviceID, bytes, dwAddrLen = 256;
    ATM_ADDRESS  addr;
    SOCKADDR_ATM atm_addr;
    char         szAddress[256];
    WSAPROTOCOL_INFO pProtocolInfo;
 
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
    if (FindProtocol1(&pProtocolInfo) == FALSE)
    {
        printf("FindProtocol() returned NULL for ATM. Check the ATM adapter/NIC!!\n");
        WSACleanup();
        return -1;
    }
    else
        printf("FindProtocol() returned something for ATM!\n");
 
    s = WSASocket(FROM_PROTOCOL_INFO, FROM_PROTOCOL_INFO, 
		FROM_PROTOCOL_INFO, &pProtocolInfo, 
		0, WSA_FLAG_OVERLAPPED);

    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    else
       printf("WSASocket() is OK!\n");
 
    deviceID = 0;
    if (WSAIoctl(s, SIO_GET_ATM_ADDRESS, (LPVOID) &deviceID,
        sizeof(DWORD), (LPVOID) &addr, sizeof(ATM_ADDRESS),
        &bytes, NULL, NULL) == SOCKET_ERROR)
    {
        printf("WSAIoctl() error %d\n", WSAGetLastError());
        WSACleanup();
        return -1;
    }
    else
        printf("WSAIoctl() is OK!\n");
 
    ZeroMemory((PVOID)&atm_addr, sizeof(atm_addr));
    atm_addr.satm_family                 = AF_ATM;
    atm_addr.satm_number.AddressType     = ATM_NSAP;
    atm_addr.satm_number.NumofDigits     = ATM_ADDR_SIZE;
    atm_addr.satm_blli.Layer2Protocol    = SAP_FIELD_ANY;
    atm_addr.satm_blli.Layer3Protocol    = SAP_FIELD_ABSENT;
    atm_addr.satm_bhli.HighLayerInfoType = SAP_FIELD_ABSENT;
 
    memcpy(atm_addr.satm_number.Addr, &addr.Addr, ATM_ADDR_SIZE);
    ZeroMemory((PVOID)szAddress, sizeof(szAddress));
 
    if (WSAAddressToString((LPSOCKADDR)&atm_addr, sizeof(atm_addr), 
		&pProtocolInfo, (LPWSTR)szAddress, &dwAddrLen))
    {
        printf("WSAAddressToString() error %d\n", WSAGetLastError());
        return(FALSE);
    }
    else
         printf("WSAAddressToString() is fine!\n");
     printf("The atm address (if any): <%S>\n", szAddress);
 
    // Cleanup
    if(closesocket(s) == 0)
           printf("closesocket() should be fine!\n");
    else
           printf("closesocket() failed with error code %d\n", WSAGetLastError());
    if(WSACleanup() == 0)
           printf("WSACleanup() is OK!\n");
    else
           printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}

// Function: main
// Description: Load Winsock, create a socket and find the broadcast address
int main_sio_get_broadcast_address(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    SOCKADDR_IN   bcast;
    int           ret;
    DWORD         dwBytesRet;
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() should be fine!\n");
 
    // Create a socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is OK!\n");
 
    // Call the ioctl
    ret = WSAIoctl(s, SIO_GET_BROADCAST_ADDRESS, NULL, 0,&bcast, sizeof(bcast), &dwBytesRet, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoclt(SIO_GET_BROADCAST_ADDRESS) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAIoclt(SIO_GET_BROADCAST_ADDRESS) should be fine!\n");
 
    printf("Broadcast address is: '%s'\n", inet_ntoa(bcast.sin_addr));
 
    // Cleanup
    if(closesocket(s) == 0)
         printf("closesocket() should be fine!\n");
    else
         printf("closesocket() failed with error code %d\n", WSAGetLastError());
 
     if(WSACleanup() == 0)
         printf("WSACleanup() is OK!\n");
     else
         printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}

// Description:
//    This sample shows you how to set the SO_KEEPALIVE_VALS
//    on a socket. This option allows you to set the keepalive
//    interval on a per socket basis as opposed to the rather
//    useless SO_KEEPALIVE option.
//
// No command line arguments
//
// Link to ws2_32.lib
int main_sio_keepalive_vals(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    DWORD         dwBytesRet=0;
    struct tcp_keepalive   alive;
 
    // Load Winsock lib
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() should be fine!\n");
 
    // Create a socket
    s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is fine!\n");
 
    // Set the keepalive values
    alive.onoff = TRUE;
    alive.keepalivetime = 7200000;
    alive.keepaliveinterval = 6000;
 
    if (WSAIoctl(s, SIO_KEEPALIVE_VALS, &alive, sizeof(alive), NULL, 0, &dwBytesRet, NULL, NULL) == SOCKET_ERROR)
    {
        printf("WSAIotcl(SIO_KEEPALIVE_VALS) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAIotcl(SIO_KEEPALIVE_VALS) is working!\n");
 
    printf("SIO_KEEPALIVE_VALS set:\n");
    printf("   Keepalive Time     = %lu\n", alive.keepalivetime);
    printf("   Keepalive Interval = %lu\n", alive.keepaliveinterval);
 
    // Cleanup
    if(closesocket(s) == 0)
          printf("closesocket() should be fine!\n");
    else
          printf("closesocket() failed with error code %d\n", WSAGetLastError());
 
    if(WSACleanup() == 0)
          printf("WSACleanup() is OK!\n");
    else
          printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}

// Description:
//    This sample illustrates how to use the SIO_ROUTING_INTERFACE_QUERY
//    option. This takes a destination IP address and port and calls
//    the interface query to determine which local IP interface should
//    be used to transmit the data on. This is a Windows 2000 specific option.
//
// Command Line Arguments/Parameters
//    Sio_routing_interface_query Remote-IP Remote-Port
//      Remote-IP      Address of remote machine interested in
//      Remote-Port    Port of remote machine interested in
//
// Link to ws2_32.lib
// Function: main
// Description:
//    Load Winsock and parse the command line. Then create a
//    socket and perform the routing query.  Print out the route that should be taken.
int main_sio_routing_interface_query(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    SOCKADDR_IN   input, *lpIf=NULL;
    DWORD         dwBytesRet=0, nRet, i;
    char          buf[1024];
    int           ret;
 
    // Parse the command line and load Winsock
    /*if (argc != 3)
    {
        printf("Usage: %s remote-ip remote-port\n", argv[0]);
        printf("Example: %s 209.131.36.158 80\n", argv[0]);
        return -1;
    }*/
 
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
    // Creat a socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is fine!\n");
 
    // Perform the query on the given IP address
    input.sin_family = AF_INET;
    input.sin_addr.s_addr = inet_addr("127.0.0.1"/*argv[1]*/);
    input.sin_port = htons((short)atoi("80"/*argv[2]*/));
 
    ret = WSAIoctl(s, SIO_ROUTING_INTERFACE_QUERY, &input, sizeof(input), buf, 1024, &dwBytesRet, NULL, NULL);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAIoctl(SIO_ROUTING_INTERFACE_QUERY) failed: %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAIoctl(SIO_ROUTING_INTERFACE_QUERY) is fine!\n");
 
    printf("bytes returned: %d\n", dwBytesRet);
 
    nRet = dwBytesRet / sizeof(SOCKADDR_IN);
    lpIf = (SOCKADDR_IN *)buf;
 
    // Print out the interface information
    for(i=0; i < nRet ;i++)
    {
        printf("Interface:\n   sin_family = %d\n   sin_addr   = %s\n   sin_port   = %d\n", lpIf[i].sin_family, inet_ntoa(lpIf[i].sin_addr), lpIf[i].sin_port);
    }
 
    if(closesocket(s) == 0)
        printf("closesocket() should be fine!\n");
    else
        printf("closesocket() failed with error code %d\n", WSAGetLastError());
 
     if(WSACleanup() == 0)
        printf("WSACleanup() is OK!\n");
     else
        printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}

// Function: main
// Description: Loads Winsock, creates a listening socket and get the state of SO_ACCEPTCONN.
int main_so_acceptconn(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    SOCKADDR_IN   local;
    int           ret, iSize;
    BOOL          bOpt;
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
    // Creat a socket, bind to it, and listen
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is OK!\n");
 
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons(5150);
 
    iSize = sizeof(bOpt);
    ret = getsockopt(s, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bOpt, &iSize);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(SO_ACCEPTCONN) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("getsockopt(SO_ACCEPTCONN) is OK!\n");
    if (bOpt == TRUE)
        printf("getsockopt(SO_ACCEPTCONN) returned TRUE!\n");
    else
        printf("getsockopt(SO_ACCEPTCONN) returned FALSE!\n");
    if (bind(s, (SOCKADDR *)&local, sizeof(local)) == SOCKET_ERROR)
    {
        printf("bind() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("bind() is OK!\n");
 
    listen(s, 7);
 
    // Get the option value
    iSize = sizeof(bOpt);
    ret = getsockopt(s, SOL_SOCKET, SO_ACCEPTCONN, (char *)&bOpt, &iSize);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(SO_ACCEPTCONN) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
 
    if (bOpt == TRUE)
        printf("getsockopt(SO_ACCEPTCONN) returned TRUE!\n");
    else
        printf("getsockopt(SO_ACCEPTCONN) returned FALSE!\n");
 
    if(closesocket(s) == 0)
         printf("closesocket() should be fine!\n");
    else
         printf("closesocket() failed with error code %d\n", WSAGetLastError());
    if(WSACleanup() == 0)
         printf("WSACleanup() is OK!\n");
    else
         printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}

// Description:
//    This sample illustrates how to send and receive
//    broadcast data. The SO_BROADCASt option only needs
//    to be set on the sending socket; however in reality,
//    the Microsoft provider's don't check to see if the
//    option is set. In other words you could send data
//    to INADDR_BROADCAST without the option set and no error would occur.
//
// Command line arguments
//    SO_BROADCAST s|r
//      s      Send broadcast data
//      r      Receive data
//
// Link to ws2_32.lib
// Function: main
// Description:
//    The main function does everything: loads the Winsock library,
//    parses the arguments, sets the broadcast option, and either
//    sends or receives data depending on the command line options.
int main_so_broadcast(int argc, char **argv, BOOL bSender)
{
    WSADATA       wsd;
    SOCKET        s;
    SOCKADDR_IN   to;
    int           ret, fromsz;
    BOOL          bOpt/*bSender*/;
    char         *msg="This is a test string from sender", rcvbuf[1024];
 
    // Check for arguments
    /*if (argc != 2)
    {
        printf("Usage: %s s|r\n", argv[0]);
        printf("    s  =  send\n");
        printf("    r  =  receive\n");
        printf("Example: %s s\n", argv[0]);
        return -1;
    }*/
 
    /*if (tolower(argv[1][0]) == 's')
        bSender = TRUE;
    else
        bSender = FALSE;*/
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() should be fine!\n");
 
    // Create a UDP socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is fine!\n");
    // Set the broadcast option (really only necessary for the sender
    bOpt = TRUE;
    ret = setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *)&bOpt, sizeof(bOpt));
    if (ret == SOCKET_ERROR)
    {
        printf("setsockopt(SO_BROADCAST) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("setsockopt(SO_BROADCAST) should be OK!\n");
 
    if (bSender)
    {
        // Send some data
        to.sin_family = AF_INET;
        to.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        to.sin_port = htons(5150);
 
        ret = sendto(s, msg, strlen(msg), 0, (SOCKADDR *)&to, sizeof(to));
        if (ret == SOCKET_ERROR)
        {
            printf("sendto() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
            printf("sendto() is OK!\n");
    }
    else
    {
        // Receive data so we must bind first
        to.sin_family = AF_INET;
        to.sin_addr.s_addr = htonl(INADDR_ANY);
        to.sin_port = htons(5150);
 
        ret = bind(s, (SOCKADDR *)&to, sizeof(to));
        if (ret == SOCKET_ERROR)
        {
            printf("bind() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
            printf("bind() is OK!\n");
        // Now we can read data
        fromsz = sizeof(to);
        ret = recvfrom(s, rcvbuf, 1024, 0, (SOCKADDR *)&to, &fromsz);
        if (ret == SOCKET_ERROR)
        {
            printf("recvfrom() failed with error code %d\n", WSAGetLastError());
            return -1;
        }
        else
            printf("recvfrom() is pretty fine!\n");
        rcvbuf[ret] = 0;
        printf("Read: '%s' from %s\n", rcvbuf, inet_ntoa(to.sin_addr));
    }
 
    if(closesocket(s) == 0)
         printf("closesocket() should be fine!\n");
    else
         printf("closesocket() failed with error code %d\n", WSAGetLastError());
 
    if(WSACleanup() == 0)
         printf("WSACleanup() is OK!\n");
    else
         printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;       
}

// Description:
//    This short sample illustrates how to call the SO_PROTOCOL_INFO
//    option to retrieve the Winsock catalog entry corresponding
//    to the socket. This function only shows how to call the option
//    and does not print out the whole WSAPROTOCOL_INFO structure.
//
// No command line arguments
//
// Link to ws2_32.lib
// Function: main
// Description:
//    Load Winsock, create a socket, and call SO_PROTOCOL_INFO to
//    retrieve the catalog entry corresponding to our socket.
int main_protocol_info(int argc, char **argv)
{
    WSADATA          wsd;
    SOCKET           s;
    WSAPROTOCOL_INFOA pinf;
    int              iSize,ret;
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() should be OK!\n");
 
    // Create our TCP socket
    s = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is OK!\n");
 
    // Get the protocol information. You can either use WSA_PROTOCOL_INFO
    // or SO_PROTOCOL_INFOA since we didn't #define UNICODE.
    iSize = sizeof(pinf);
    ZeroMemory(&pinf, iSize);
    ret = getsockopt(s, SOL_SOCKET, SO_PROTOCOL_INFOA, (char *)&pinf, &iSize);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(SO_PROTOCOL_INFO) failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("getsockopt(SO_PROTOCOL_INFO) should be OK!\n");
 
    // Print a couple fields out
    printf("\nProtocol: [%s]\n", pinf.szProtocol);
    printf("Flags:    0x%x\n\n", pinf.dwServiceFlags1);
 
    if(closesocket(s) == 0)
         printf("closesocket() should be fine!\n");
    else
         printf("closesocket() failed with error code %d\n", WSAGetLastError());
    if(WSACleanup() == 0)
         printf("WSACleanup() is OK!\n");
    else
         printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}

// Description:
//    This is a trivial sample that sets the receive timeout
//    option and then attempts to receive a datagram which will fail with a timeout message.
//
// No command line arguments
//
// Link to ws2_32.lib
// Function: main
// Description:
//    Load Winsock, create a UDP socket, set the timeout value,
//    and then call recvfrom() will will fail with a timeout since no data is being sent.
int main_rcvtimeo(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKADDR_IN   from;
    SOCKET        s;
    int           ret, iVal=0, fromsz;
    unsigned int  sz = sizeof(iVal);
    char          rcvbuf[1024];
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
     }
     else
        printf("WSAStartup() should be OK!\n");
 
    // Create a datagram socket
    s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (s == INVALID_SOCKET)
    {
        printf("socket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("socket() is OK!\n");
 
    // Set the timeout value
    iVal = 1000;
    ret = setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&iVal, sz);
    if (ret == SOCKET_ERROR)
    {
        printf("setsockopt() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("setsockopt() is OK!\n");
 
    printf("Timeout was set...\n");
 
    // Retrieve the value just to be sure
    printf("Do a verification...\n");
    ret = getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *)&iVal, &sz);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("getsockopt() is fine!\n");
 
    printf("Timeout value == %d\n", iVal);
 
    // To test the receive function we need to bind first
    from.sin_family = AF_INET;
    from.sin_addr.s_addr = htonl(INADDR_ANY);
    from.sin_port = htons(5150);
 
    ret = bind(s, (SOCKADDR *)&from, sizeof(from));
    if (ret == SOCKET_ERROR)
    {
        printf("bind() failed: %d\n", WSAGetLastError());
        return -1;
    }
 
    // Call receive which will fail with WSAETIMEDOUT
    fromsz = sizeof(from);
    ret = recvfrom(s, rcvbuf, 1024, 0, (SOCKADDR *)&from, &fromsz);
    if (ret == SOCKET_ERROR)
    {
        if (WSAGetLastError() == WSAETIMEDOUT)
            printf("recvfrom() failed with error code 10060 (WSAETIMEDOUT)\n");
        else
            printf("recvfrom() failed with error code %d\n", WSAGetLastError());
    }
    else
        printf("recvfrom() looks fine!\n");
    if(closesocket(s) == 0)
        printf("closesocket() should be fine!\n");
    else
        printf("closesocket() failed with error code %d\n", WSAGetLastError());
     if(WSACleanup() == 0)
         printf("WSACleanup() is OK!\n");
     else
         printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}

// Description:
//    This sample creates a socket and then calls the SO_TYPE option
//    to retrieve the socket type (SOCK_STREAM, SOCK_DGRAM, etc) for that socket.
//
// Command line arguments
//
// Link to ws2_32.lib
// Function: main
// Description: Load Winsock, create a socket, and then call the SO_TYPE option and print the results.
int main_so_type(int argc, char **argv)
{
    WSADATA       wsd;
    SOCKET        s;
    int           ret, iVal, iSize;
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() should be fine!\n");
 
    // In this case create a UDP SOCK_DGRAM socket
    s = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (s == INVALID_SOCKET)
    {
        printf("WSASocket() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSASocket() is OK!\n");
 
    // Retrieve the type and print its value
    iSize = sizeof(iVal);
    ret = getsockopt(s, SOL_SOCKET, SO_TYPE, (char *)&iVal, &iSize);
    if (ret == SOCKET_ERROR)
    {
        printf("getsockopt(SO_TYPE) failed with error code %d\n", iVal);
        return -1;
    }
    else
        printf("getsockopt(SO_TYPE) is pretty fine!\n");
 
    printf("Protocol type: ");
 
    if (iVal == SOCK_STREAM)
        printf("SOCK_STREAM\n");
    else if (iVal == SOCK_DGRAM)
        printf("SOCK_DGRAM\n");
    else if (iVal == SOCK_RDM)
        printf("SOCK_RDM\n");
    else if (iVal == SOCK_SEQPACKET)
        printf("SOCK_SEQPKACKET\n");
    else if (iVal == SOCK_RAW)
        printf("SOCK_RAW\n");
    else
        printf("Unknown\n");
 
    // Clean-up
    if(closesocket(s) == 0)
         printf("closesocket() should be fine!\n");
    else
         printf("closesocket() failed with error code %d\n", WSAGetLastError());
    if(WSACleanup() == 0)
         printf("WSACleanup() is OK!\n");
    else
         printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}