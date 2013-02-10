
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

/***
*void StrCopy(char* source, char* target) - copy one string to another
*
*Purpose:
*       We have an empty string and want to fill it with chars from another string
*
*Entry:
*       char*  source       - char array to copy from
*       char*  target       - char array to copy to
*
*******************************************************************************/
void StrCopy(char* source, char* target)
{
    for (; (*target = *source) != '\0';)
	{
		source++; 
		target++;
	}
}

/***
*void StrCopyNum(char* source, char* target, int number) - 
* copy the first number symbols from one string to another
*
*Purpose:
*       It is almost the same as previous but copies only part of string
*
*Entry:
*       char*  source       - char array to copy from
*       char*  target       - char array to copy to
*       int number          - the number of the first chars to copy
*
*******************************************************************************/
void StrCopyNum(char* source, char* target, int number)
{
	int i;

	for (i = 0; i < number; i++)
	{
		target[i] = source[i];
	}
}

/***
*int GetNumberOfLines(char* str) - computes the number of lines in the string
*
*Purpose:
*       If the string contains k numbers of \n symbols, 
*       function returns k + 1 lines
*
*Entry:
*       char*  str       - input char array
*
*******************************************************************************/
int GetNumberOfLines(char* str)
{
    int num = 0;

	/* Go through all chars in the string.
	 * If we meet the symbol \n we increment number counter
	 */
	while (*str != EOF)
	{
		if (*str == '\n')
		{
			num++;
		}

        str++;  
	}

	/* Add the last line */
	return num + 1;
}

/***
*int GetWordCount(char* str) - computes the number of words in the string
*
*Entry:
*       char*  str       - input char array
*
*******************************************************************************/
int GetWordCount(char* str)
{
	int num = 0;

	/* This is an indicator where the pointer is: inside a word or outside.
	 * Before start loop we are outside any words
	 */
    int state = OUT; 

	/* Go through all chars in the string. */
	while (*str != EOF)
	{
		/* If we meet space, tab or enter, we change state to out. */
        if (*str == ' ' || *str == '\t' || *str == '\n')
		{
            state = OUT;
		}
		/* If we meet word symbol we have 2 cases:
		 * 1. If we are inside a word, we simply go over to the next symbol.
		 * 2. If we are outside a word, we need to change state from OUT to IN.
		 *    When state is changed from OUT to IN we meet a new word 
		 *    and we increment word count number.
		 */
		else if (state == OUT)
		{
            state = IN;
			num++;
		}

		str++;
	}

	return num;
}

/***
*SYMBOL_STAT GetSymbolStat(char* str) - gets the symbol statistic in the word:
* the number of spaces, digits and other symbols
*
*Entry:
*       char*  str       - input char array
*
*******************************************************************************/
SYMBOL_STAT GetSymbolStat(char* str)
{
	SYMBOL_STAT stat;
    int num = 0;
    int i;

	/* Fill all static with 0 values */
	stat.others = 0;
	stat.spaces = 0;

	for (i = 0; i < 10; i++)
	{
		stat.digits[i] = 0;
	}

    /* Go through all chars in the string. */
	while (*str != EOF)
	{
        /* If we meet space, increment spaces count. */
        if (*str == ' ')
		{
			stat.spaces++;
		}
		/* If we meet a digit, increment an appropriate counter */
		else if (*str >= '0' && *str <= '9')
		{
			stat.digits[*str - '0']++;
		}
		/* Else increment other symbols count */
		else
		{
			stat.others++;
		}

		str++;
	}

	return stat;
}
