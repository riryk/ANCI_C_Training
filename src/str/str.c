
#include "str.h"

#define MAX_STACK_SIZE 100
#define BUFFER 100
#define NUMBER -666
#define MAXOP 100

/* A number of element in stack from which items are free.
 * For example: [a1, a2, a3,...,stackFreePosition, 0, 0,..., 0,..., MAX_STACK_SIZE]
 */
int stackFreePosition = 0;

enum { NAME, PARENS, BRACKETS };

/* the type of the last read token. It can be NAME, PARENS or BRACKETS */
int tokenType;

/* A memory located for the stack */
double stackStorage[MAX_STACK_SIZE];

/* A temporary buffer where to keep symbols temporarily */
char charBuffer[BUFFER];
char name[BUFFER];

/* Here will be a result of syntax translation */
char out[1000];

/* Free position in the char buffer */
int charBufferFreePosition = 0;

/* Push item into stack */
int StackPush(double item)
{
    /* If there are enough space where to put a new item */
    if (stackFreePosition < MAX_STACK_SIZE)
    {
	   stackStorage[stackFreePosition++] = item;

	   /* Return 1 if success */
	   return 1;
    }

	/* return false */
	return 0;
}

/* Push item into stack */
double StackPop()
{
	/* If stack is empty, return false. */
    if (stackFreePosition == 0)
	{
		return 0.0;
	}
    
	/* Return value and decrement stack free position */
	return stackStorage[stackFreePosition--];
}

int GetNextCharFromString()
{ 
	return (charBufferFreePosition > 0) ? charBuffer[--charBufferFreePosition] : getchar();
}

void PutCharToBuffer(char c)
{
    if (charBufferFreePosition < BUFFER)
	{
        charBuffer[charBufferFreePosition++] = c;
	}
}

int IsDigit(char c)
{
    return c >= '0' && c <= '9';
}

/* Read operands from input stream and save them into char array */
int GetNextOperand(char Source[])
{
    int i;
	int c;

	/* Read the first non empty symbol. Skip all tabs and spaces */
    while ((Source[0] = c = GetNextCharFromString()) == ' ' || c == '\t')
		;
    
	/* Clear the end of string */
    Source[1] = '\0';

	/* If we have found a simple char, return it */
	if (!IsDigit(c) && c != '.')
	{
        return c;
	}

	/* If we are here, that means that we have found a digit */
	i = 0;

	/* Read all digit symbols */
    if (IsDigit(c))
	{
        while (IsDigit(Source[++i] = c = GetNextCharFromString()))
			;
	}

	/* If we have found a deouble delimiter, then read the fraction part */
	if (c == '.')
	{
        while (IsDigit(Source[++i] = c = GetNextCharFromString()))
			;
	}

	/* Clear all symbols in the end */
	Source[i] = '\0';

	/* If we have read too many symbols, give it back to a buffer */
	if (c != EOF)
	{
        PutCharToBuffer(c);
	}

	return NUMBER;
}

/* Converts string to double */
double ConvertStringToDouble(char Source[])
{
    double val;
	double power;

    int i;
	int sign;

    /* Skip all empty symbols */ 
	for (i = 0; Source[i] == ' ' || Source[i] == '\t'; i++)
		;
    
	/* Compute a sign of the number */
	sign = (Source[i] == '-') ? -1 : 1; 
    if (Source[i] == '+' || Source[i] == '-')
	{
        i++;   
	}
    
    /* Convert symbols up to '.' to a single number */ 
	for (val = 0.0; isdigit(Source[i]); i++)
	{
		val = 10 * val + Source[i] - '0';
	}

	/* If there is fraction */
	if (Source[i] == '.')
	{
        i++;
	}

    /* Convert fraction symbols after '.' */
	for (power = 1.0; isdigit(Source[i]); i++)
	{
        val = 10 * val + Source[i] - '0';
		power = power * 10.0;
	}

	return sign * val / power;
}

/* Calculates an expression written in reverse polish rules
 * For example:
 * the expression (1 - 2) * (4 + 5)
 * 1 2 - 4 5 + *
 * The stack history:
 * 1.  [1]
 * 2.  [1 2]
 * 3.  [-1]
 * 4.  [-1 4]
 * 5.  [-1 4 5]
 * 6.  [-1 9]
 * 7.  [-9]
 */
double PolishCalculate()
{
	int type;
	double convertedOperand;
	double op2;
    char nextOperand[MAXOP];
	 
    while ((type = GetNextOperand(nextOperand)) != EOF)
	{
		switch (type)
		{
		case NUMBER:
			convertedOperand = ConvertStringToDouble(nextOperand);
			StackPush(convertedOperand);
			break;
		case '+':
			StackPush(StackPop() + StackPop());
			break;
		case '*':
			StackPush(StackPop() * StackPop());
			break;
		case '-':
            op2 = StackPop();
			StackPush(StackPop() - op2);
			break;
		case '/':
			op2 = StackPop();
            StackPush(StackPop() / op2);
			break;
		case '\n':
			/* Return result */
            return StackPop(); 
		default:
            return 0;
		}
	}
}

/***
*Lexical grammar, which is used during declarations parsing
*  dcl        : optional '*' direct-dcl
*  direct-dcl : name
*                  |  (dcl)
*                  |  direct-dcl()
*                  |  direct-dcl[optional size]
* For example, how to use this grammar to parse this function: pfa
*   (*pfa[])() - this is an array of pointers to function without parameters
* 1. pfa will be recognized as name and thus as direct-dcl
* 2. pfa[] will be recognized as direct-dcl
* 3. *pfa[] will be dcl
* 4. (*pfa[]) will be direct-dcl
* 5. (*pfa[])() will be direct-dcl
*******************************************************************************/

/* part of lexical grammar, which implements dcl part */
void Dcl()
{
   int ns;

   /* Skip all elements until we find '*' and count them */
   for (ns = 0; GetToken() == '*'; ns++)
	   ;

   /* Lexical scan of direct - dcl */
   DirDcl();

   /* Process all tokens */
   while (ns-- > 0)
   {
	   strcat(out, " pointer to");
   }
}

int DirDcl()
{
   int type;

   /* In this case we have found (dcl) part of grammar.
    * So in this case we call Dcl() recursively
    */
   if (tokenType == '(')
   {
      Dcl();

	  /* We must have found ) according to the grammar */
	  if (tokenType != ')')
	  {
		  printf("error: missing )\n");
		  return 0;
	  }
   }
   else if (tokenType == NAME)
   {
      strcpy(name, charBuffer);
   }
   else
   {
      printf("error: unexpected name or (dcl))\n");
	  return 0;
   }

   /* Try to find any of '(', ')', '[', ']' symbols */
   while ((type = GetToken()) == PARENS || type == BRACKETS)
   {
	   if (type == PARENS)
	   {
           strcat(out, " function returning");
	   }
	   else
	   {
           strcat(out, " array");
           strcat(out, charBuffer);
           strcat(out, " of");
	   }
   }
}

char* ParseDeclaration(char * Declaration)
{
    return NULL;
}

/* Get the next tocken during function declaration parsing */
int GetToken()
{
	int c;
    char* token = charBuffer;

	/* First of all skip all tab and empty symbols */
	while((c = GetNextCharFromString()) == ' ' || c == '\t')
		;
    
	/* If we have met a left bracket */
    if (c == '(')
	{
		/* If the next is  */
        if ((c = GetNextCharFromString()) == ')')
		{
			StrCopy(token, "()");
            return tokenType = PARENS;
		}
		else
		{
            PutCharToBuffer(c);
			return tokenType = '(';
		}
	}
	else if (c == '[')
	{
		/* Read all content between '[' and ']' symbols into temp buffer */
		for (*token++ = c; (*token++ = GetNextCharFromString()) != ']';)
			;
        *token = '\0';
        return tokenType = BRACKETS;
	}
	else if (isalpha(c))
	{
        /* Read all name into temporary buffer */
        for (*token++ = c; isalnum(c = GetNextCharFromString());)
		{
			*token++ = c;
		}

        *token = '\0';
        PutCharToBuffer(c);
		return tokenType = NAME;
	}
	else
	{
		return tokenType = c;
	}
}

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
*int StrCompare(char* source, char* target) - compares two strings
*
*Entry:
*       char* source        - a source string
*       char* target        - a target string
*Exit:
*       Returns 0 if two strings are equal
*       Returns >0 if source string is bigger than target
*       Returns <0 if source string is less than target
*
*******************************************************************************/
int StrCompare(char* source, char* target)
{
    /* Go through two strings until we find not equal symbols */
    for (; *source == *target; source++, target++)
    {
		/* If we have met the end of the source string, we return 0 */
        if (*source == '\0')
		{
			return 0;
		}
    }

	/* If we are here, that means that we have met different symbols */
	return *source > *target;
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

void TestCharEncoding()
{
	/* sizeof(char) = 1 byte. It can represents all
	 * ANCI symbols [0...255]
	 */
	char charAnci = 'A'; 
	/* Here is an example of ANCI char string */
	char arrayAnci[100] = "A am an ANCI string";
    /* sizeof(wchar_t) = 2 rytes = 16 bits. This is an representation
	 * of UTF-16 string encoding.
	 */   
    wchar_t charUnicode = L'A';
	/* Here is an example of Unicode char string */
    wchar_t arrayUnicode[100] = L"A am an Unicode string";
}

BOOL StringReverseW(PWSTR pWideCharStr, DWORD cchLength)
{
    PWSTR pEndOfStr = pWideCharStr + wcsnlen_s(pWideCharStr, cchLength) - 1;
	wchar_t cCharT;
	while (pWideCharStr < pEndOfStr)
	{
		cCharT = *pWideCharStr;
		*pWideCharStr = *pEndOfStr;
        pWideCharStr++;
        pEndOfStr--;
	}
	return (TRUE);
}

BOOL StringReverseA(PSTR pMultiByteStr, DWORD cchLength)
{
   PWSTR pWideCharStr;
   int nLenOfWideCharStr;
   BOOL fOk = FALSE;
   /* Calculate the number of characters needed to hold 
    * the wide-character version of the string.
	* Using parameter CP_ACP the following string converts ANCI char string into
	* Unicode string. The return value is the number of characters written into wide char 
	* string. If the lpWideCharStr and cchWideChar is ommitted, function returns wide char string length.
	* It returns the number of characters needed for the wide characters string.
    */
   nLenOfWideCharStr = MultiByteToWideChar(CP_ACP, 0, pMultiByteStr, cchLength, NULL, 0);
   /* Allocate memory for a new string */
   pWideCharStr = (PWSTR)HeapAlloc(GetProcessHeap(), 0, nLenOfWideCharStr * sizeof(wchar_t));
   /* If we can't allocate a memory, we return false */
   if (pWideCharStr == NULL)
   {
	   return (fOk);
   }
   /* Convert ANCI char string to unicode */
   MultiByteToWideChar(CP_ACP, 0, pMultiByteStr, cchLength, pWideCharStr, nLenOfWideCharStr);
   /* Call a unicode function */ 
   fOk = StringReverseW(pWideCharStr, nLenOfWideCharStr);
   /* We successfully reversed unicode string. Now we need to convert it back
    * to ANCI string.
    */
   if (fOk)
   {
      WideCharToMultiByte(CP_ACP, 0, pWideCharStr, cchLength, pMultiByteStr, 
		  (int)strlen(pMultiByteStr), NULL, NULL); 
   }
   /* Free memory */
   HeapFree(GetProcessHeap(), 0, pWideCharStr);
   return (fOk);
}