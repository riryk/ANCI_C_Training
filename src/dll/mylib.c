
#include <windows.h>
#include "mylib.h"

int g_nResult;

int Add(int nLeft, int nRight)
{
    g_nResult = nLeft + nRight;
	return g_nResult;
}

