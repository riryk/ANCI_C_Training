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
		StrCopyNum((char*)buffer, readMessages[i], bufferSize);

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

/***
* Routine Description:
* Writes a message to a file
*******************************************************************************/
void WriteToFile(char* pathToFile, char* message)
{
	BOOL bErrorFlag = FALSE;
	DWORD dwBytesToWrite = (DWORD)strlen(message);
	DWORD dwBytesWritten = 0;
	/* Get a handler to this file */
	HANDLE fileHandler = CreateFile(
		               pathToFile,                         // name of the write
                       GENERIC_WRITE,                      // open for writing
                       0,                                  // do not share
                       NULL,                               // default security
                       CREATE_NEW | OPEN_EXISTING,         // create new file only
                       FILE_ATTRIBUTE_NORMAL,              // normal file
                       NULL);   
    /* When unable to get a handle to the file. 
	 * Retrieve the error information and print it to console output. */
	if (fileHandler == INVALID_HANDLE_VALUE)
	{
		DisplayFileError(TEXT("WriteToFile"));
		printf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), pathToFile);
		return;
	}
	/* Try to write the message into the file */
	printf(TEXT("Writing %d bytes to %s.\n"), dwBytesToWrite, pathToFile);
	/* Try to write the message */
	bErrorFlag = WriteFile( 
                    fileHandler,           // open file handle
                    message,               // start of data to write
                    dwBytesToWrite,        // number of bytes to write
                    &dwBytesWritten,       // number of bytes that were written
                    NULL);                 // no overlapped structure
	/* When unable to write bytes into text file */
	if (bErrorFlag == FALSE)
	{
		DisplayFileError(TEXT("WriteToFile"));
		printf(TEXT("Terminal failure: Unable to write file.\n"));
	}
	else
	{
		if (dwBytesWritten != dwBytesToWrite)
		{
			/* This is an error because a synchronous write that results in
             * success (WriteFile returns TRUE) should write all data as
             * requested. This would not necessarily be the case for
             * asynchronous writes.
			 */
            printf("Error: dwBytesWritten != dwBytesToWrite\n");
		}
		else
		{
			/* Bytes have been successfully written */
			printf(TEXT("Wrote %d bytes to %s.\n"), dwBytesToWrite, pathToFile);
		}
	}
	/* Close a file handler and free memory */
	CloseHandle(fileHandler);
}

/***
* Routine Description:
* Reads bytes to a buffer from a file
*******************************************************************************/
char* ReadFromFile(char* pathToFile)
{
	char buffer[BUFFER_READ_SIZE];
	BOOL bErrorFlag = FALSE;
	DWORD dwBytesRead = 0;
	/* Get a handler to this file */
    HANDLE fileHandler = CreateFile(
		               pathToFile,                         // file to open
                       GENERIC_READ,                       // open for reading
                       FILE_SHARE_READ,                    // share for reading
                       NULL,                               // default security
                       OPEN_EXISTING,                      // existing file only
                       FILE_ATTRIBUTE_NORMAL,              // normal file
                       NULL);                              // no attr. template
	/* When unable to get a handle to the file. 
	 * Retrieve the error information and print it to console output. */
	if (fileHandler == INVALID_HANDLE_VALUE)
	{
		DisplayFileError(TEXT("ReadFromFile"));
		printf(TEXT("Terminal failure: Unable to open file \"%s\" for write.\n"), pathToFile);
		return NULL;
	}
	/* Try to read a buffer from the file */
	printf(TEXT("Reading a buffer %d bytes from %s.\n"), BUFFER_READ_SIZE, pathToFile);
	/* Try to read the message */
	bErrorFlag = ReadFile(
		           fileHandler, 
				   buffer, 
				   BUFFER_READ_SIZE - 1, 
				   &dwBytesRead, 
				   NULL);
	/* If could not read a file content */
	if (bErrorFlag = FALSE)
	{
		DisplayFileError(TEXT("ReadFromFile"));
		printf(TEXT("Terminal failure: Unable to read from file \"%s\".\n"), pathToFile);
		CloseHandle(fileHandler);
		return NULL;
	}
	/* This is the section of code that assumes the file is ANSI text. 
     * Modify this block for other data types if needed.
	 */
	if (dwBytesRead > 0 && dwBytesRead <= BUFFER_READ_SIZE - 1)
	{
		/* Set the symbols in the end to '\0' */
		buffer[dwBytesRead] = '\0';
		/* Successful reading. Write the buffer to console output in test mode */
		printf(TEXT("Data read from %s (%d bytes): \n"), pathToFile, dwBytesRead);
		printf("%s\n", buffer);
	}
	else if (dwBytesRead == 0)
	{
		/* We have not read any bytes from the file. It can be empty */
		printf(TEXT("No data read from %s: \n"), pathToFile);
	}
	else
	{
		/* It can be an unpredictable error */
		printf("\n ** Unexpected value for dwBytesRead ** \n");
	}

	return buffer;
}

/***
* Routine Description:
* Retrieve and output the system error message for the last-error code
*******************************************************************************/
void DisplayFileError(LPTSTR lpszFunction) 
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
	SIZE_T errorBufSize;
	/* First of all get the latest error code from windows */
    DWORD dw = GetLastError(); 
	/* Read a resource message for the last error from Windows resource files */
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, 
        NULL);
	/* Print the error message into console output */
	printf("%s failed with error code %d as follows:\n%s", lpszFunction, dw, lpMsgBuf);
	/* When we call FormatMessage windows function, windows allocates memory
	 * for LPVOID lpMsgBuf variable. If we call this function several times we will
	 * gain memory leak. So we need to free memory after writing it to console buffer
	 * But this memory has been allocated in a Windows heap, which is different from
	 * our local heap, which malloc uses. If we try to free memory:
	 *  free(lpMsgBuf)
	 * we will receive the error that it is not possible to free memory from 
	 * an another heap. For this purpose we use a native Windows function for free.
	 */
    LocalFree(lpMsgBuf);
}

