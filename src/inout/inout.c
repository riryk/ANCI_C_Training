/***
* Purpose:
*       This file include the function declarations 
*       of base operations for IO, console buffer write or read
*
*******************************************************************************/

#include "inout.h"
#include "str.h"
#include "mthread.h"
#include "memory.h"

CRITICAL_SECTION section;
int wasSectionInitialized = 1;

/***
*void InOutCritSectInit() - initializes all critical sections in inout.c 
*                           and must be called in the beginning of main.
*Purpose:
*       This function is simple wrapper for InitializeCriticalSection native Win API function
*
*******************************************************************************/
void InOutCritSectInit()
{
    InitializeCriticalSection(&section);
    wasSectionInitialized = 0;
}

/***
*char* ReadConsole_NoLock() - reads the console input without any locks
*
*Purpose:
*       Reads message from console input.
*       Function uses buffer reading but puts buffer into single memory
*
*Entry:
*       DWORD   bufferSize       - the size of read buffer
*
*Exit:
*       NOT NULL                           success
*       NULL                               failure
*
*Exceptions:
*
*******************************************************************************/
char** ReadConsole_NoLock(DWORD bufferSize)
{
	char* readMessages[BUFFER_ARRAY_SIZE];
    HANDLE  consoleHandler;
    BOOL readResult;

	LPVOID buffer;
    DWORD readBytesNum;
	DWORD lastError;
	int i = 0;

	/* Retrieve a standard std console handler */
	consoleHandler = GetStdHandle(STD_INPUT_HANDLE); 

	/* If windows return error, we return null */
	if (consoleHandler == INVALID_HANDLE_VALUE)
	{
	  	return NULL;
	}

	/* Loop while ReadFile function returns FALSE or cannot read any bytes */
	while (1)
	{
		/* Try to read buffer */
		readResult = ReadFile(
			consoleHandler,
			buffer,
			bufferSize,
			&readBytesNum,
			NULL);

		/* When we get error or have not read no bytes, we return null */
		if (readResult == FALSE || readBytesNum == 0)
		{
			lastError = GetLastError();
			return readMessages;
		}
        
		/* Allocate new memory for the buffer we have alredy read using ReadFile function */
		readMessages[i] = (char*)MemoryAlloc(bufferSize * sizeof(char));

		/* Copy chars from temporary buffer to the memory we have just allocated */
		StrCopyNum(buffer, readMessages[i], bufferSize);

		/* Increase the buffer pointer */
		i++;

		/* We have reached the end of readMessages array */
		if (i == BUFFER_ARRAY_SIZE)
		{
           return readMessages;
		}
	}

	/* Return read messages */
	return readMessages;
}

/***
*void WriteToConsole_NoLock(char *message) - writes the message into console output without any locks
*
*Purpose:
*       Take user message and write it into console output.
*       The function does not use message partitioning - 
*       message separation on small buffers for decreasing memory usage.
*
*Entry:
*       char*       message       - the pointer to the message
*
*Exit:
*       0                           success
*       1 INVALID_STD_HANDLE        could not get a handler to console buffer
*       2 INVALID_CONSOLE_MODE      could not get the console mode
*
*Exceptions:
*
*******************************************************************************/
int WriteToConsole_NoLock(char *message)
{
	HANDLE  consoleHandler;

	BOOL    consoleModeResult;
	DWORD   consoleMode;
    
	BOOL    writeFileResult;
	INT     messageLength = LengthOfStr(message);
	DWORD   symbolsWritten;

	/* Retrieve a standard std console handler */
	consoleHandler = GetStdHandle(STD_OUTPUT_HANDLE); 
    
	/* If windows return error, we break function */
	if (consoleHandler == INVALID_HANDLE_VALUE)
	{
	  	return INVALID_STD_HANDLE;
	}
    
	/* Retrieve std mode */
	consoleModeResult = GetConsoleMode(
		consoleHandler, 
		&consoleMode);

    /* If unable to get console mode, break the function */
	if (consoleModeResult == 0)
	{
		return INVALID_CONSOLE_MODE;
	}

	/* Writing message to the std buffer */
    writeFileResult = WriteFile(
        consoleHandler,
		message,
        messageLength,
        &symbolsWritten,
        NULL);

	/* If unable to write to the std buffer, quit */
    if (writeFileResult == WRITE_FILE_ERROR)
	{
		return WRITE_FILE_ERROR;
	}

	/* Return success */
	return 0;
}

/***
*void WriteToConsole(char *message) - writes the message into console output
* and synchronizes threads 
*
*******************************************************************************/
int WriteToConsole(char *message)
{
   int WriteResult;

   __try
   {
	   /* Check if it has been called function InOutCritSectInit() */
	   if (wasSectionInitialized == 1)
	   {
		   return NOT_INITIALIZED_CRIT_SECTION;
	   }

	   /*Acquire lock*/
       EnterCriticalSection(&section); 

	   /*Write to console without lock */
	   WriteResult = WriteToConsole_NoLock(message);
   }
   __finally
   {
	   /*Release lock*/
       LeaveCriticalSection(&section);
   }

   /* Return result */
   return WriteResult;
}

/***
* This is a wrapper for WriteToConsole function for using it in threads
*
*******************************************************************************/
DWORD WINAPI Test_WriteToConsole_Wrap(LPVOID lpParam) 
{
	/* Call WriteToConsole */
    WriteToConsole((char*)lpParam);
}

/***
* This is a wrapper for WriteToConsole_NoLock function for using it in threads
* 
*******************************************************************************/
DWORD WINAPI Test_WriteToConsole_NoLock_Wrap(LPVOID lpParam) 
{
    WriteToConsole_NoLock((char*)lpParam);
}

/***
* This is a test asyncronous method for WriteToConsole
* 
*******************************************************************************/
int Test_WriteToConsole(char* testMessage, int lockFlag)
{
	int index;
    HANDLE threads[THREAD_COUNT];
    LPTHREAD_START_ROUTINE threadFunc;
    
	/* Initialize a critical section */
    InOutCritSectInit();

	/* Determine what lockable or not lockable WriteConsole to choose 
	 * Otherwise return error */
    if (lockFlag == TEST_WITH_LOCK)
	{
        threadFunc = Test_WriteToConsole_Wrap;
	}
	else if (lockFlag == TEST_WITHOUT_LOCK)
	{
        threadFunc = Test_WriteToConsole_NoLock_Wrap;
	}
	else
	{
		return TEST_ERROR_LOCK;
	};

	/* Create THREAD_COUNT number of threads */
	for (index = 0; index < THREAD_COUNT; index++)
	{
        threads[index] = CreateMThread(threadFunc, testMessage);

		if (threads[index] == NULL)
		{
			return TEST_COULD_NOT_CREATE_THREAD;
		}
	}

	/* Wait for those threads to complete */
	WaitFor(threads, THREAD_COUNT);

	/* Close threads handlers */
    for (index = 0; index < THREAD_COUNT; index++)
	{
		CloseHandle(threads[index]);
	}

	/* Return success */
	return 0;
}