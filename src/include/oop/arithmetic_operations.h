
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>

enum tokens
{
	NUMBER,
	ADD,
	SUB,
	MINUS,
	VALUE
};

struct ScanNode
{
	enum tokens token;
    struct ScanNode* left;
	struct ScanNode* right;
};

struct Type
{
	char* name;
	void* (*anew)(va_list ap);
	double (*exec)(const void* tree);
	void (*adelete)(void* tree);
};

struct Bin
{
    const void* type;
	void* left;
	void* right;
};

struct Val
{
    const void* type;
	double value;
};

void* product();
int getchar(char* buffer);
enum tokens scan(char* buffer);
void aprocess(void* e);
void adelete(void* e);
void* factor();
int sum();
void error(const char* fmt,...);