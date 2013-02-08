#include "memory.h"
#include "inout.h"
#include <stdio.h>

int main(int argc, char *argv[])
{   
	char c;

	while ((c = getchar()) != EOF)
       putchar(c);

	return 0;
}

