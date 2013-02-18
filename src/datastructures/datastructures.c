#include "datastructures.h"

/***
*int GetWord(char* word, int lim) - reads a new word or an isolated symbol 
* from console input.
*
*Purpose:
*       This function is used in calculating of ANCI reserved 
*       word count in output string.
*
*Entry:
*       char* word                - an output word
*       int lim                   - maximum length of a word. If a word exceeds 
*                                   this restriction, we divide it into two words
*Exit:
*       Returns the first symbol of the word
*
*******************************************************************************/
int GetWord(char* word, int lim)
{
	int c;
	char* w = word;

	/* First of all, skip all spaces from the input stream */
	while (isspace(c = GetNextCharFromString()))
		;

	/* If we are not in the end of input stream, we move further */
	if (c != EOF)
	{
		*w++ = c;
	}

	/* If the current symbol is not alphabetic, 
	 * we end the string and move further */
	if (!isalpha(c))
	{
		*w = '\0';
		return c;
	}

	/* Read symbols into output array word until we meet not alphanumeric
	 * symbol or not number */
    for (; --lim > 0; w++)
	{
		if (!isalnum(*w = GetNextCharFromString()))
		{
			PutCharToBuffer(*w);
			break;
		}
	}

	*w = '\0';
	return word[0];
}

/***
*int WordBinSearch(char* word, struct WORD_STAT table[], int n) 
* - finds a word "word" in a sorted array of strings using standard string
*   lexic order
*
*Purpose:
*       This function is used in calculating of ANCI reserved 
*       word count in output string. If we have read a new word from console 
*       input, we try to find it in the table. If we have not found, we insert 
*       it into the array using lexicographical order.
*
*Entry:
*       char* word                - an input word we have read from console input
*       struct WORD_STAT table[]  - a pointer to a table, which contains C reserved 
*                                   words statistic.
*       int n                     - the number of words in the table array
*
*Exit:
*       Returns the index to the word in the array we have found.
*       If we have not found, we return -1
*
*******************************************************************************/
int WordBinSearch(char* word, struct WORD_STAT table[], int n)
{
	int start;
	int end;
	int mid;
	int cond;

	start = 0;
	end = n - 1;

	while (start < end)
	{
		mid = (start + end) >> 1;
		if ((cond = strcmp(word, table[mid].word)) < 0)
		{
			end = mid - 1;
		}
		else if (cond > 0)
		{
			start = mid + 1;
		}
		else
		{
			return mid;
		}
	}

	return -1;
}

/***
*void BuildWorsStatistic() - reads words from input and refresh words count
*  and then writes result into console output
*
*******************************************************************************/
void BuildWorsStatistic()
{
	int n;
	int i;
	char word[MAXWORD];

	/* Read output until we meet EOF symbol */
	while (GetWord(word, MAXWORD) != EOF)
	{
		if (isalpha(word[0]))
		{
			/* If there is this word in the table, we increase word count */
			if ((n = WordBinSearch(word, CWords, NKEYS)) != -1)
			{
				CWords[n].count++;
			}
		}
	}

	/* Print results */
	for (i = 0; i < NKEYS; i++)
	{
		if (CWords[i].count > 0)
		{
			printf("%4d %s\n", CWords[i].count, CWords[i].word);
		}
	}
}

/***
*void AddTreeItem(struct WORD_TREE_ITEM* TreeNode, char* newWord) 
* - inserts a new word into appropriate place into binary tree or
*   increments word count if we have found an item with newWord.
*
*******************************************************************************/
void AddTreeItem(struct WORD_TREE_ITEM* TreeNode, char* newWord)
{
	/* Compare result */
	int c;

	/* If we need to insert data into NULL node, we alloc memory,
	 * copy string into new allocated memory and set NULL to leftChild and rightChild
	 */
	if (TreeNode == NULL)
	{
		TreeNode = (struct WORD_TREE_ITEM*)malloc(sizeof(struct WORD_TREE_ITEM));
		TreeNode->count = 1;
		/* strdup - allocates new memory for string and copies newWord string 
		 * into newly allocated memory.
		 */
		TreeNode->word = strdup(newWord);
		TreeNode->leftChild = NULL;
		TreeNode->rightChild = NULL;
	}
	/* Let's check if this items sutisfies search conditions */
	else if ((c = strcmp(newWord, TreeNode->word)) == 0)
	{
		TreeNode->count++;
	}
	/* If TreeNode != NULL and TreeNode->word != newWord, we try to find it  */
	else
	{

	}
}
