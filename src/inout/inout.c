/***
* Purpose:
*       This file include the function declarations 
*       of base operations for IO, console buffer write or read
*
*******************************************************************************/

#include "inout.h"

/***
*void write_console(char *message) - writes the message into console output
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
int write_console(char *message)
{
	HANDLE  consoleHandler;

	BOOL    consoleModeResult;
	DWORD   consoleMode;
    
	BOOL    writeFileResult;
	INT     messageLength = NELEMS(message);
	LPDWORD bytesWritten;

	/* Retrieve a standard std console handler */
	consoleHandler = GetStdHandle(STD_INPUT_HANDLE); 
    
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
        bytesWritten,
        NULL);

	/* If unable to write to the std buffer, quit */
    if (writeFileResult == WRITE_FILE_ERROR)
	{
		return WRITE_FILE_ERROR;
	}

	/* Return success */
	return 0;
}
