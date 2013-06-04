
#include "arithmetic_operations.h"
#include "new.h"
#include "class.h"

static enum tokens token; /* current input symbol */
static double number;     /* if NUMBER: numeric value */
// static jmp_buf onError;

void* product()
{

}

void* anew(const void* type,...)
{
	va_list ap;
	void* result;

	assert(type && ((struct Type*)type)->anew);

	va_start(ap, type);

	va_end(ap);
}

int sum()
{
	void* result = product();
	const void* type;
   
    for (;;)
	{
		switch (token)
		{
		case '+':
			type = ADD;
			break;
		case '-':
			type = SUB;
            break;
		default:
			return result;
		}
		scan(0);
        result = new(type, result, product());
	}

	return 0;
}

static void* factor()
{
	void* result;

	switch (token)
	{
	case '+':
		scan(0);
		return factor();
	case '-':
		scan(0);
		return new (MINUS, factor());
	default:
		error("bad factor: '%c' 0x%x", token, token);
	case NUMBER:
        result = new(VALUE, number);
		break;
	case '(':
		scan(0);
		result = sum();
		if (token != ')')
			error("expecting )");
	}
	scan(0);
	return result;
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