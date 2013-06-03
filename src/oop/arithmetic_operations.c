
#include "arithmetic_operations.h"
#include "new.h"
#include "class.h"

static enum tokens token; /* current input symbol */
static double number;     /* if NUMBER: numeric value */
// static jmp_buf onError;

void product()
{

}

int sum()
{
	product();
   
    for (;;)
	{
		case '+':
		case '-':
            scan(0);
			product();
			continue;
	}

	return 0;
}

static enum tokens scan(char* buffer)
{
	static const char* bbp;

	if (buffer)
        bbp = buffer;

	while (isspace(*bbp))
		++bbp;

	if (isdigit(*bbp) || *bbp == '.')
	{
        errno = 0; 
		token = NUMBER;
        number = strtod(bbp, (char**)&bbp);
		if (errno == ERANGE)
			error("bad value: %s", strerror(errno));
	}
	else
		token = *bbp ? *bbp++ : 0;

	return token;
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