#include "str.h"

/* ANCI C reading 38 */
int main(int argc, char *argv[])
{ 
	//int c = getchar();
	//FILE* _file = fopen("D:\\Run.cmd", "r");

	
	HANDLE _fileHandler = CreateFile(
		               "D:\\Run.cmd",                      // name of the write
                       GENERIC_WRITE,                      // open for writing
                       0,                                  // do not share
                       NULL,                               // default security
                       CREATE_NEW | OPEN_EXISTING,         // create new file only
                       FILE_ATTRIBUTE_NORMAL,              // normal file
                       NULL);   

	if (_fileHandler == INVALID_HANDLE_VALUE)
	{

	}

	//memset(str, 0, 4*sizeof(char));
}

void DisplayError()
{
	LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 
    

}

