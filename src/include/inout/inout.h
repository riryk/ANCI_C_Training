/***
* Purpose:
*       This file include the function declarations 
*       of base operations for IO, console buffer write or read
*
*******************************************************************************/

#include <windows.h>

#define INVALID_STD_HANDLE         1
#define INVALID_CONSOLE_MODE       2
#define WRITE_FILE_ERROR           3

#define NELEMS(x)  (sizeof(x) / sizeof(x[0]))

int write_console(char *message);

