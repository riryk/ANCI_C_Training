
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

/***
*int Atoi(char* str) - converts a string consists of digits into int:
*
*Entry:
*       char*  str       - input char array
*
*******************************************************************************/
int Atoi(char* str)
{
	int n = 0;

	/* Go through all chars in the string. */
	while (*str != EOF)
	{
		if (*str >= '0' && *str <= '9')
		{
			n = 10 * n + (*str - '0');
		}

		str++;
	}

	return n;
}

/***
*char* Zero(char* str) - converst all not ANCII symbols to zero '\0'
*
*Entry:
*       char*  str       - input char array
*
*******************************************************************************/
void Zero(char* str, int len)
{
	int i;

	/* Go through all chars in the string. */
	for (i = 0; i < len; i++)
	{
	 	if (str[i] < 0)
		{
			str[i] = '\0';
		}
	}
}

/***
*char* Lower(char* str) - converts a string consists of upper case symbols to 
* lower case symbols
*
*Entry:
*       char*  str       - input char array
*
*******************************************************************************/
char* Lower(char* str)
{
	int i;
    char* lowerStr;

	/* Compute the length of string */
	int len = LengthOfStr(str);

	/* Allocate memory for a new string */
	lowerStr = (char*)MemoryAlloc(len * sizeof(char));

	/* Go through all chars in the string. */
	for (i = 0; i < len; i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			lowerStr[i] = str[i] + 'a' - 'A';
		}
		else
		{
		    lowerStr[i] = str[i];
		}
	}

	/* Substitute all not ANCII symbols with zero */
	Zero(lowerStr, len);

	/* Return string */
	return lowerStr;
}

/***
*char* Squeeze(char* str, char c) - deletes all occurences of char c from a string
*
*Entry:
*       char*  str       - an input char array
*       char   c         - a char to delete
*
*Algorythm descriptions
*
* For example we have a string  asd[c]sdf[c]sdf[c]ssdf
* 1. Loop through the string until we reach the first [c] symbol
* 2. Substitute the symbol by its right neighbour: asdssdf[c]sdf[c]ssdf
*    Rotate symbols until we reach a new [c] char
*    asdssdf[c]sdf[c]ssdf 
*    asdsddf[c]sdf[c]ssdf
*    asdsdff[c]sdf[c]ssdf
* 3. If we meet a new [c] symbol, we skip it and move to the next neighbour
*    and rotate it
* 4. Let n is the number of [c] symbols in the array
*    In the end of the array we have n excessive symbols and we zero them.
*
*******************************************************************************/
void Squeeze(char* str, char c)
{
	/* pointer to the old string */
	int i;

	/* pointer to the new string */
	int j;

	/* Compute the length of string */
	int len = LengthOfStr(str);

	/* i pointer goes through all chars in the string */
	for (i = j = 0; i < len; i++)
	{
		/* If we meet not c symbol we left rotate j and i
		 * It is impossible to loss data because j <= i
		 * We increment j only when i != c
		 */
		if (str[i] != c)
		{
			str[j++] = str[i];
		}
	}

    /* After this j points to the last symbol of the new string.
	 * The symbols from j to i position are odd.
	 * The next loop clears them.
	 */
	while (j < i)
	{
		str[j++] = '\0';
	}
}

/***
*void ConcatString(char* source, char* strToConcat) - copy string to the end 
* of the source string. Source string must contain enough space for this
*
*Entry:
*       char* source       - input char array
*       char* strToConcat  - a string for adding to the source string
*
*******************************************************************************/
void ConcatString(char* source, char* strToConcat)
{
	int i;
	int j;

	i = j = 0;

	while (source[i] != '\0')
		i++;

	while ((source[i++] = strToConcat[j++]) != '\0')
		;
}

/***
*void ReverseString(char* source) - reverses a string
* For example string "abcdfg" will be transformed into "gfdcba"
*
*Entry:
*       char* source       - input char array
*
*******************************************************************************/
void ReverseString(char* source)
{
	int len;
	int i;
	int j;
	char temp;

	/* Calculate the length of the string */
	len = LengthOfStr(source);

	for (i = 0, j = len - 1; i < j; i++, j--)
	{
		temp = source[i];
		source[i] = source[j];
		source[j] = temp;
	}
}

/***
*int Trim(char* source, int n) - deletes odd symbols from the end of the string
*
*Entry:
*       char* source        - a string
*       int n               - a number of symbols in this string
*
*******************************************************************************/
int Trim(char* source, int n)
{
	int i;

	/* Go from the end of the array and skip odd symbols */
	for (i = n - 1; i >= 0; i--)
	{
		if (source[i] != ' ' && source[i] != '\t' && source[i] != '\n')
		{
			break;
		}
	}

	/* The symbol next to the latest not odd is set to 0 */
	source[i + 1] = '\0';
	return i;
}

/***
*void Dec2Bin(long decimal, char *binary) - convert a number from decimal system to
* binary.
*
*Entry:
*       long  decimal       - some number in decimal system
*       char  *binary       - output binary string
*
*Algorythm:
* n = [n/2]*2 + {n/2} - we need to evaluate {n/2} on every step
*******************************************************************************/
char* Dec2Bin(long decimal)
{    
	int k = 0; 
	int n = 0;  
	int i = 0;

	char neg_flag = '0';   
	int remain;   
	int old_decimal; 

	char temp[20];
	char binary[20];

	/* Fill all arrays with 0 */
	for (; i < 20; i++)
	{
		temp[i] = binary[i] = '0';
	}

	/* Determine whether the number is positive or negative.
	 * And make the bumber positive.
	 */
	if (decimal < 0)   
	{    
		decimal = -decimal;      
		neg_flag = '1';   
	}

	/* Divide the number until we reach 0 and write {n/2} into a temp array. */
	do   
	{    
		remain = decimal % 2;     
		decimal = decimal / 2;      

		/*Convert 0,1 numbers into appropriate characters*/
		temp[k++] = remain + '0';   
	}
	while (decimal > 0);    
	
	/* Fill the sign of the number into the temp array */
	if(neg_flag == '1')    
	{
		temp[k++] = '-'; 
	}

	/* Revert the temp string into a binary array */
	while (k >= 0)
	{
		binary[n++] = temp[--k];
	}

	/* Fill in the last char by \0 */
	//binary[n - 1] = '\0';
	return binary;
}

/* For example if we have a binary number ..0000000010011010001
 * p = 4, n = 3 We must get get bits on positions: 4, 3, 2: 110
 * The algorythm:
 *
 *  1. p + 1 - n - the number of bits from the beginning of the 
 *     number to skip:    1 0  0 1 1 0 1 0 0 0 1 
 *                        0 1  2 3 4 
 *
 *     we must skip the first 2 bits: 1 and 0
 *  2. x >> (p + 1 - n) we get a number which first bit matchs to
 *     the first item of our desired bits
 *     For example:   0 0 0 0 0 1 0 0 1 1 0 1 0 0 0 1
 *                    0 0 0 0 0 0 0 1 0 0 1 1 0 1 0 0
 *
 *  3. apply "not" bit operation to zero: ... 0 0 0 0 0 0
 *     we will get                        ... 1 1 1 1 1 1 
 *
 *  4. ~0 << n we will get:  0 0 0 0 0 1 0 0 1 1 0 1 0 0 0 1
 *                           0 0 0 0 0 0 0 1 0 0 1 1 0 1 0 0
 *                           1 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0
 *  5. ~(~0 << n) we will get : 
 *                           0 0 0 0 0 1 1 1 1 1 1 1 1 1 1 1
 *  6. (x >> (p + 1 - n)) & ~(~0 << n):
 *                           0 0 0 0 0 0 0 1 0 0 1 1 0 1 0 0
 *                           0 0 0 0 0 0 0 0 0 0 0 0 0 1 1 1
 *                           _______________________________
 *                           0 0 0 0 0 0 0 0 0 0 0 0 0 1 0 0
 */
unsigned int GetBits(unsigned x, int p, int n)
{ 
	return (x >> (p + 1 - n)) & ~(~0 << n);
}

/***
*char* Dec2BinBitwise(long i) - convert a number from decimal system to binary 
* using bitwise operations.
* Algorythm:
* Take for example i = 123 in binary system it will be 1111011
* i = [i] + {i}
* For computation of {i} we use bitwise &: 1111011 & 0000001 = 1
* i >>= 1 1111011 >>= 1 = 111101 = [i]
*/
char* Dec2BinBitwise(long i)
{
	char* str; 
	char* p;
	str = (char*)malloc(sizeof(long)*8*sizeof(char));
	p = str;
	while (i > 0)
	{
		/* bitwise AND operation with the last bit */
		(i & 0x1) ? (*p++ = '1') : (*p++ = '0'); 
		
		/* Bit shift to the right, when there are no bits left.
		 * When the value is 0, so the loop ends */
		i >>= 1; 
	}
	
	return str;
}

/* Algorythm:
 * 1. x = 123 = 1111011   7 = 0000111
 *    x & 7 =   011 
 */
int mod7(int x)
{
	while (x > 7)
	{
		x = x & 7 + x >> 3;
	}

	return (x == 7) ? 0 : x;
}

/* Algorythm:
 * 1. x = 123 =        11 110 11
 *    y = 99  =        11 000 11
 *  
 * 2. sum = x ^ y =    00 110 00
 *    carry = x & y =  11 000 11
 *  
 * 3.x = sum        =  00 110 00
 *   y = carry << 1 = 110 001 10
 * 
 * 4.sum = x ^ y =    110 111 10 
 *   carry = x & y =  000 000 00
 */
unsigned int BitwiseAdd(unsigned int x, unsigned int y)
{
	unsigned int sum;
	unsigned int carry;

	/* Ex - OR x and y */
	sum = x ^ y;     
	/* AND x and y */
    carry = x & y; 

	/* enter the loop  */
	while (carry != 0) 
    {
		/* left shift the carry */
        carry = carry << 1;    

		/* initialize x as sum */
        x = sum;             

		/* initialize y as carry */
        y = carry;             

		/* sum is calculated */
        sum = x ^ y;     

		/* carry is calculated and loop condition is checked and the process repeated until carry = 0 */
        carry = x & y;       
    }

	return sum;
}

/***
*unsigned int BinSearch(unsigned int* Array, unsigned int len, unsigned int Val) 
* - search the value Val in the Array of length len.
*
*Purpose:
*       We have a sorted array and we want to find a position of a value Val in the array.
*
*Entry:
*       unsigned int* Array       - an input sorted array
*       unsigned int len          - the length of the array
*       unsigned int Val          - a value to find
*
*Exception:
* 1 2 3 4 5 6 7 8  start = 0; end = 7; mid = 3
* 
*******************************************************************************/
unsigned int BinSearch(unsigned int* Array, unsigned int len, unsigned int Val)
{
	/* We put the start pointer to the beginning of the array */
    unsigned int start = 0;

	/* We put the finish pointer to the end of the array */
	unsigned int finish = len - 1;
	unsigned int mid = -1;

	/* Loop while there is space between start and finish to look for */
	while (start <= finish)
	{
		/* Find the middle point */
		mid = BitwiseAdd(start, finish) >> 1;

		/* If we have found the item, return the index */
		if (Array[mid] == Val)
		{
			return mid;
		}
		/* In this case find in a lower part of the array comparing with mid point */
		else if (Val < Array[mid])
		{
			finish = mid - 1;
		}
		/* Else find in the upper part ot the array */
		else 
		{
			start = mid + 1;
		}
	}

	return -1;
}

