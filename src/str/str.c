
#include "str.h"


/***
*int LengthOfStr(char *str) - calculate length of string
*
*Purpose:
*       Calculating the length of string
*
*Entry:
*       char     *str       - char array, a pointer to the first array symbol
*
*Exit:
*       Returns total length of string or 0
*      
*Exceptions:
*
*******************************************************************************/
int LengthOfStr(char *str)
{
   int len; 
   
   /* 1. First of all we set len to 0.
   *  2. Then we move through all string until we reach the not existed symbol '\0'.
   *     str -  is the pointer to any char of string.
   *     *str - the fact value of the pointer to the string.
   *  3. During the loop we increment len.
   */
   for (len = 0; *str != '\0'; str++)
   {
	   len++;
   }

   /* Return length */
   return len;
}


