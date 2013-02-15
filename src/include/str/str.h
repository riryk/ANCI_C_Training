/***
* Purpose:
*       This file include the function declarations 
*       of base operations for strings and char arrays
*
*******************************************************************************/

#include <stdio.h>
#include "memory.h"

/* Those flags are used to determine whether pointer is inside
 * or outside a word.
 * This is used in int GetWordCount(char* str) function
 */
#define IN  1
#define OUT 0

enum escapes 
{
	BELL = '\a',
	BACKSPACE = '\b',
	TAB = '\t',
	NEWLINE = '\n',
	VTAB = '\v',
	RETURN = '\r'
};

struct _symbol_stat {
	int others;
	int spaces;
	int digits[10];
};
typedef struct _symbol_stat SYMBOL_STAT;

int LengthOfStr(char *str);

void StrCopy(char* source, char* target);
void StrCopyNum(char* source, char* target, int number);

int GetNumberOfLines(char* str);
int GetWordCount(char* str);
SYMBOL_STAT GetSymbolStat(char* str);
int Atoi(char* str);
char* Lower(char* str);
void Squeeze(char* str, char c);
void ConcatString(char* source, char* strToConcat);
void ReverseString(char* source);

char* ConvertToBinary(int n, char *str);
char* Dec2Bin(long decimal);
unsigned GetBits(unsigned x, int p, int n);
char* Dec2BinBitwise(long i);

int mod7(int x);
unsigned int BinSearch(unsigned int* Array, unsigned int len, unsigned int Val);