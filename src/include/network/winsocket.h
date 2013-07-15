
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <Nb30.h>

#ifndef WINSOCKET_H
#define WINSOCKET_H

void StartMain();
void BindSocket();
int TcpServer();
void UniversalServerIP6();
void UniversalServerIP61();
void IrServer();

// LANANUM and LOCALNAME should be set as in your system
// For LANA num you may want to use LANACFG.EXE tool
#define LANANUM     3
// Padded up to 16 bytes
#define LOCALNAME  "TERGEDIK        "
 
void NBCheck(NCB x);
void MakeNetbiosName (char *achDest, LPCWSTR szSrc);
BOOL NBAddName (int nLana, LPCWSTR szName);
BOOL NBReset (int nLana, int nSessions, int nNames);
BOOL NBListNames (int nLana, LPCWSTR szName);
BOOL NBAdapterStatus (int nLana, PVOID pBuffer, int cbBuffer, LPCWSTR szName);

#endif