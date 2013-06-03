
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>

enum tokens
{
	NUMBER,
	START,
	END
};

void product();
int getchar(char* buffer);
enum tokens scan(char* buffer);
void aprocess(void* e);
void adelete(void* e);
int sum();
void error(const char* fmt,...);