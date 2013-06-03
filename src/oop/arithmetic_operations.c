
#include "arithmetic_operations.h"
#include "new.h"
#include "class.h"

static enum tokens token; /* current input symbol */
// static jmp_buf onError;

int sum()
{
	return 0;
}

int scan(char* buffer)
{
	return 0;
}

int main_process()
{
	volatile int errors = 0;
	char buf[1024];

    // if (setjmp(onError))
	//   ++errors;

	while (getchar(buf))
	{
		if (scan(buf))
		{
			void* e = sum();
            if (token)
				error("trash after sum");
		}
	}
}

void error(const char* fmt,...)
{
    va_list ap;

	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	putc('\n', stderr);
	va_end(ap);
	//longjmp(onError, 1);
}