
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#ifndef STRING_H
#define STRING_H

const void* String;

struct String
{
	const void*  class;
	char*        text;
};

#endif