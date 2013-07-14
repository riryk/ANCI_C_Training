
#include <windows.h>
#include "mylibexport.h"

int MyLib_g_nResult;

int MyLib_Add(int nLeft, int nRight)
{
    MyLib_g_nResult = nLeft + nRight;
	return MyLib_g_nResult;
}

