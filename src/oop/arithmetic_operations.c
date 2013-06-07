
#include "arithmetic_operations.h"
#include "new.h"
#include "class.h"

static enum tokens token; /* current input symbol */
static double number;     /* if NUMBER: numeric value */
// static jmp_buf onError;

static struct Type _Add = { "+", Bin_ctor, binEmit, Bin_free };
static struct Type _Sub = { "-", Bin_ctor, binEmit, Bin_free };
static struct Type _Value = { "", Val_ctor, Val_emit, free };

const void* Add = &_Add;
const void* Sub = &_Sub;
const void* Value = &_Value;

static void* Val_ctor(va_list ap)
{
    struct Val* node = malloc(sizeof(struct Val));

	assert(node);
	node->value = va_arg(ap, double);
	return node;
}

static void Val_emit(const void* tree)
{
	printf(" %g", ((struct Val*)tree)->value);
}

void* product()
{ }

static void exec(const void* tree)
{
    assert(tree 
	   && *(struct Type**)tree
	   && (*(struct Type**)tree)->exec);

	(*(struct Type**)tree)->exec(tree);
}

static void binEmit(const void* tree)
{
	exec(((struct Bin*)tree)->left);
	exec(((struct Bin*)tree)->right);
	printf(" %s", (*(struct Type**)tree)->name);
}

void process(const void* tree)
{
	putchar('\t');
	exec(tree);
	putchar('\n');
}

void* anew(const void* type,...)
{
	va_list ap;
	void* result;

	assert(type && ((struct Type*)type)->anew);

	va_start(ap, type);

	result = ((struct Type*)type)->anew(ap);
	*(const struct Type**)result = type;

	va_end(ap);
	return result;
}

static void* Bin_ctor(va_list ap)
{
	struct Bin* node = malloc(sizeof(struct Bin));

	assert(node);
	node->left = va_arg(ap, void*);
	node->right = va_arg(ap, void*);
	return node;
}

static void Bin_delete(void* tree)
{
    assert(tree 
	   && *(struct Type**)tree
	   && (*(struct Type**)tree)->adelete);

    (*(struct Type**)tree)->adelete(tree);
}                

static void Bin_free(void* tree)
{
	Bin_delete(((struct Bin*)tree)->left);  
	Bin_delete(((struct Bin*)tree)->right); 

	free(tree);
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