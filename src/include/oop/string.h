
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#ifndef STRING_H
#define STRING_H

struct String
{
	const void*  class;
	char*        text;
};

void* String_ctor(const void* self, va_list* app);
void* String_dtor(void* self);
void* String_clone(const void* self);
int String_differ(const void* self, const void* b);
void test_string();

#endif