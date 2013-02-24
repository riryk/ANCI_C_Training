/***
* Purpose:
*       This file include the function declarations 
*       of base operations for IO, console buffer write or read
*
*******************************************************************************/

#include <windows.h>



#define THREAD_COUNT                 10
#define BUFFER_SIZE                  8
#define BUFFER_READ_SIZE             81
#define BUFFER_ARRAY_SIZE            10

#define INVALID_STD_HANDLE           1
#define INVALID_CONSOLE_MODE         2
#define WRITE_FILE_ERROR             3
#define NOT_INITIALIZED_CRIT_SECTION 4

#define TEST_WITH_LOCK               1
#define TEST_WITHOUT_LOCK            2
#define TEST_ERROR_LOCK              10
#define TEST_COULD_NOT_CREATE_THREAD 11

void InOutCritSectInit();

char** ReadConsole_NoLock(DWORD bufferSize);
int WriteToConsole(char *message);

int Test_WriteToConsole(char* testMessage, int lockFlag);

void WriteToFile(char* pathToFile, char* message);
char* ReadFromFile(char* pathToFile);
void DisplayFileError(LPTSTR lpszFunction);
