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
*
*Exceptions:
*
*******************************************************************************/
void write_console(char *message)
{
	HANDLE consoleHandler;

	/* Retrieve a standard  */
	consoleHandler = GetStdHandle(STD_INPUT_HANDLE); 

	if (consoleHandler == INVALID_HANDLE_VALUE)
	{}
}
