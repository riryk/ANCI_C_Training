#include "test.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	int c;

	while ((c = getchar()) != EOF)
       putchar(c);

    test_method("");
}