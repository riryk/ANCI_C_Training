
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

#define MAX_GUID_SZ        256


// Description:
//    This is a simple program that calls WSAEnumNameSpaceProviders
//    to obtain a list of the name spaces currently available on
//    the machine. This app simply lists them.
//
// No command line argument
//    WSAEnumNameSpaceProviders
//
// Link to ws2_32.lib


// Function: main
// Description:
//    Load Winsock and enumerate the name spaces. First we call
//    WSAEnumNameSpaceProviders with a NULL buffer to find out
//    the required buffer size. We then allocate the buffer
//    and make the call again. Lastly, we print the structures out
int main_WSAEnumNameSpaceProviders(int argc, char **argv)
{
    WSADATA            wsd;
    char              *buff=NULL;
    WCHAR              szGuid[MAX_GUID_SZ];
    DWORD              dwSize;
    WSANAMESPACE_INFO *ns=NULL;
    int                ret, i;   
 
    // Load Winsock
    if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
    {
        printf("WSAStartup() failed with error code %d\n", WSAGetLastError());
        return -1;
    }
    else
        printf("WSAStartup() is OK!\n");
 
    // Call the API with a NULL buffer to find out the buffer size we really need
    dwSize = 0;
    ret = WSAEnumNameSpaceProviders(&dwSize, NULL);
    if (ret != SOCKET_ERROR)
    {
        printf("Shouldn't be here!\n");
        return -1;
    }
    else
        printf("WSAEnumNameSpaceProviders() should be fine!\n");
 
    // Allocate the given size
    buff = (char *)LocalAlloc(LPTR, dwSize);
    if (!buff)
    {
        printf("Out of memory!\n");
        return -1;
    }
    else
        printf("LocalAlloc() for buffer sounds good!\n");
 
    // Make the call for real now
    ns = (WSANAMESPACE_INFO *)buff;
    ret = WSAEnumNameSpaceProviders(&dwSize, ns);
    if (ret == SOCKET_ERROR)
    {
        printf("WSAEnumNameSpaceProviders() failed with error code %d\n", WSAGetLastError());
        LocalFree(buff);
        return -1;
    }
    else
        printf("WSAEnumNameSpaceProviders() is working...\n");
 
    // Print the info out
    for(i=0; i < ret ;i++)
    {
        printf("\nName Space: %S\n", ns[i].lpszIdentifier);
        printf("        ID: ");
        switch (ns[i].dwNameSpace)
        {
            case NS_ALL:
                printf("NS_ALL\n");
                break;
            case NS_SAP:
                printf("NS_SAP\n");
                break;
            case NS_NDS:
                printf("NS_NDS\n");
                break;
            case NS_PEER_BROWSE:
                printf("NS_PEER_BROWSE\n");
                break;
            case NS_TCPIP_LOCAL:
                printf("NS_TCPIP_LOCAL\n");
                break;
            case NS_TCPIP_HOSTS:
                printf("NS_TCPIP_HOSTS\n");
                break;
            case NS_DNS:
                printf("NS_DNS\n");
                break;
            case NS_NETBT:
                printf("NS_NETBT\n");
                break;
            case NS_WINS:
                printf("NS_WINS\n");
                break;
            case NS_NBP:
                printf("NS_NBP\n");
                break;
            case NS_MS:
                printf("NS_MS\n");
                break;
            case NS_STDA:
                printf("NS_STDA\n");
                break;
            case NS_NTDS:
                printf("NS_NTDS\n");
                break;
            case NS_X500:
                printf("NS_X500\n");
                break;
            case NS_NIS:
                printf("NS_NIS\n");
                break;
            case NS_NISPLUS:
                printf("NS_NISPLUS\n");
                break;
            case NS_WRQ:
                printf("NS_WRQ\n");
                break;
            default:
                printf("%d\n", ns[i].dwNameSpace);
        }
 
        StringFromGUID2(&ns[i].NSProviderId, szGuid, MAX_GUID_SZ);
        printf("      GUID: %S\n", szGuid);
        printf("    Active: %s\n", ((ns[i].fActive) ? "YES" : "NO"));
        printf("   Version: %d\n", ns[i].dwVersion);
    }
 
    // Cleanup and exit
    if(LocalFree(buff) != NULL)
        printf("LocalFree() failed!\n");
    else
        printf("LocalFree() is OK!\n");
 
    if(WSACleanup() == 0)
        printf("WSACleanup() is OK!\n");
    else
        printf("WSACleanup() failed with error code %d\n", WSAGetLastError());
    return 0;
}