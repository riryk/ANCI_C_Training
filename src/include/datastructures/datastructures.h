
#include <stdio.h>

#define MAXWORD 100
#define HASHSIZE 101

struct WORD_STAT {
	char* word;
	int count;
};

struct WORD_TREE_ITEM {
	char* word;
	int count;
	struct WORD_TREE_ITEM* leftChild;
	struct WORD_TREE_ITEM* rightChild;
};

struct WORD_STAT CWords[] = 
{
	"auto", 0,
	"break", 0,
	"case", 0,
	"char", 0,
	"const", 0,
	"continue", 0, 
	"default", 0,
	"unsigned", 0,
	"void", 0,
	"volatile", 0,
	"while", 0
};

#define NKEYS (sizeof CWords / sizeof (struct WORD_STAT))

struct HASHLIST {
	struct HASHLIST* nextItem;
	char* name;
	char* replacementText;
};

struct HASHLIST* hashtable[HASHSIZE];

int GetWord(char* word, int lim);
int WordBinSearch(char* word, struct WORD_STAT table[], int n);
void BuildWorsStatistic();