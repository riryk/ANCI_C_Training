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
* A tree has one important property:
*   leftChild < parent < rightChild
*
* Every sorted array can be represented as binary tree
*
*******************************************************************************/
struct WORD_TREE_ITEM* AddTreeItem(struct WORD_TREE_ITEM* TreeNode, char* newWord)
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
		TreeNode->word = (char*)malloc(sizeof(newWord) + 1);
		if (TreeNode->word != NULL)
		{
			strcpy(TreeNode->word, newWord); 
		}

		TreeNode->leftChild = NULL;
		TreeNode->rightChild = NULL;
	}
	/* Let's check if this items sutisfies search conditions */
	else if ((c = strcmp(newWord, TreeNode->word)) == 0)
	{
		TreeNode->count++;
	}
	/* If TreeNode != NULL and TreeNode->word != newWord, we try to find it  
	 * If newWord < TreeNode->word, then we try to find it in the left subtree
	 * If newWord > TreeNode->word, then we try to find it in the right subtree
	 */
	else if (c < 0)
	{
		AddTreeItem(TreeNode->leftChild, newWord);
	}
	else
	{
		AddTreeItem(TreeNode->rightChild, newWord);
	}

	return TreeNode;
}

/***
*void TreePrint(struct WORD_TREE_ITEM* TreeNode) 
* - Provided that tree contains a sorted array, we can print all elements 
*   in sorted order
* For example, we have a tree:
*                   parent1
*                  /      \
*       leftChild1          rightChild1
*      /          \  
* leftChild2    rightChild2
*
* First of all, we print leftChild2, then leftChild1, rightChild2 and parent1, rightChild1
*
*******************************************************************************/
void TreePrint(struct WORD_TREE_ITEM* TreeNode)
{
	/* We do not print NULL items */
	if (TreeNode == NULL)
	{
		return;
	}

	/* Print left child node */
	TreePrint(TreeNode->leftChild);

	/* Print tree node */
	printf("%4d %s\n", TreeNode->count, TreeNode->word);

	/* Print right child node */
	TreePrint(TreeNode->rightChild);
}

/***
*void BuildWorsStatisticUsingTree() - reads words from input and refresh words count
*  and then writes result into console output using 
*
*******************************************************************************/
void BuildWorsStatisticUsingTree()
{
	struct WORD_TREE_ITEM* Root;
	char word[MAXWORD];

	/* Read output until we meet EOF symbol */
	while (GetWord(word, MAXWORD) != EOF)
	{
		if (isalpha(word[0]))
		{
			Root = AddTreeItem(Root, word);
		}
	}

	/* Then print all tree */
	TreePrint(Root);
}

/***
*unsigned ComputeHash(char* s) - for every string it computes a hash value,
* which must be inside a gap [0,...,101]. The most important property of 
* a hash function must be to sutisfy uniformity of hash values
*
*******************************************************************************/
unsigned ComputeHash(char* s)
{
    unsigned hashVal;

	/* Compute a sum of all chars in the string with additional multiplication */
    for (hashVal = 0; *s != '\0'; s++)
	{
        hashVal = *s + 31 * hashVal;
	}

	/* This provides generationg a hash from [0,...,HASHSIZE - 1] array */
	return hashVal % HASHSIZE;
}

/***
*struct HASHLIST* HashLookUp(struct HASHLIST* hashtable[HASHSIZE], char* name)-
* finds item by name in a hash table. First of all the function calculates
* a hash value for the string name, then finds an appropriate position in 
* the hashtable and then, if needed, finds the item in the one-directed list
* The structure of a hash table:
* If we have string11, string12, string13 and string21, string22, string23
* using ComputeHash(char* s) we have computed that 
* string11  \            string21  \
* string12  -  hash1     string22  -  hash2    string31 - hash3 
* string13  /            string23  /
* In this case a hash table will look like:
*
* hashtable[hash1] -> [string11] -> [string12] -> [string13] -> NULL
* hashtable[hash2] -> [string21] -> [string22] -> [string23] -> NULL
* hashtable[hash3] -> [string31] -> NULL
* hashtable[hash4] -> NULL
*
*******************************************************************************/
struct HASHLIST* HashLookUp(struct HASHLIST* hashtable[HASHSIZE], char* name)
{
   unsigned hashValue;
   struct HASHLIST* list;

   /* Compute name's hash */
   hashValue = ComputeHash(name);

   /* Find an appropriate hashtable item and go through the list of items 
    * with this hash.
    */
   for (list = hashtable[hashValue]; list != NULL; list = list->nextItem)
   {
	   /* We have found it, we return a pointer */
	   if (strcmp(name, list->name) == 0)
	   {
		   return list;
	   }
   }

   /* We have not found it */
   return NULL;
}

/***
*void InstallHashTable(struct HASHLIST* hashtable[HASHSIZE], char* name)-
* Looking for an element in a hashtble and if it is impossible to find it,
* it adds the item into the hastable, otherwise, if we have found the item and 
* its name is distinguishing from name parameter, we change it.
*
*******************************************************************************/
struct HASHLIST* InstallHashTable(struct HASHLIST* hashtable[HASHSIZE], char* name)
{
    unsigned hashValue;
    struct HASHLIST* list;

	/* If we have not found name in the hashtable */
	if ((list = HashLookUp(hashtable, name)) == NULL)
	{
		/* Allocate memory for a new item */
		list = (struct HASHLIST*)malloc(sizeof(struct HASHLIST));
		/* Set name */
		list->name = name;
		hashValue = ComputeHash(name);
		/* Insert into a head of a list. hashtable[hashValue] is a link to the 
		 * head of the list. This list can be empty or filled
		 * Empty list:  hashtable[hashValue] -> NULL
		 * Filled list: hashtable[hashValue] -> item1 -> item2 -> ... -> itemn -> NULL
		 * The fastest approach to insert a value into hashtable is to insert into a head.
		 * newItem -> hashtable[hashValue] -> item1 -> item2 -> ... -> itemn -> NULL
		 * hashtable[hashValue] -> newItem -> old_hashtable[hashValue] -> item1 -> item2 -> ... -> itemn -> NULL
		 */
		list->nextItem = hashtable[hashValue];
		hashtable[hashValue]->nextItem = list;
	}
	/* Here we have found a item. So let's change the value.
	 * The common mistake is when:
	 * list->name = NULL;
	 * list->name = (char*)malloc(20);
	 * list->name = (char*)malloc(10);
	 * Here we have gained a memory leak. Because the memory (char*)malloc(20) are not released to a heap.
	 * If we repeat this several times, we will be run out of free memory.
	 * The correct approach is to always free allocated memory using malloc before changing the reference:
	 * list->name = NULL;
	 * list->name = (char*)malloc(20);
	 * free(list->name);
	 * list->name = (char*)malloc(10);
	 */
	else
	{
		free((void*)list->name);
		/* If we cannot allocate memory of name is NULL we return NULL */
		if ((list->name = strdup(name)) == NULL)
		{
			return NULL;
		}

		return list;
	}
}

