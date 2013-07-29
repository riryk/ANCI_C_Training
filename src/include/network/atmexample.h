
#ifndef ATM_EXAMPLE_H
#define ATM_EXAMPLE_H

int GetNumATMInterfaces_atm(SOCKET s);
BOOL GetATMAddress_atm(SOCKET s,int device, ATM_ADDRESS *atmaddr);
int usage_atm(char *progname);
WSAPROTOCOL_INFO* FindProtocol_atm();
void AtoH_atm(CHAR *szDest, CHAR *szSource, INT iCount);
//UCHAR BtoH_atm1(CHAR ch);
void EnumerateATMAddresses_atm(WSAPROTOCOL_INFO *lpProtocol);
int main_atm(int argc, char **argv);
void ValidateArgs_atm(int argc, char **argv);

#endif