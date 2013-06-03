
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

enum tokens
{
	START,
	END
};

int getchar(char* buffer);
int scan(char* buffer);
void aprocess(void* e);
void adelete(void* e);
int sum();
void error(const char* fmt,...);