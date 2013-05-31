
#include "string.h"
#include "new.h"
#include "class.h"

static const struct Class _String = 
{
	sizeof(struct String),
    String_ctor,
    String_dtor,
    String_clone,
    String_differ
};

const void* String = &_String;

static void* String_ctor(const void* self, va_list* app)
{
    struct String* selfCopy = self;
	const char* text = va_arg(*app, const char*);

	selfCopy->text = malloc(strlen(text) + 1);
	assert(selfCopy->text);
    strcpy(selfCopy->text, text);

	return self;
}

static void* String_dtor(void* self)
{
    struct String* selfCopy = self;

	free(selfCopy->text);
	selfCopy->text = 0;

	return selfCopy;
}

static void* String_clone(const void* self)
{
    const struct String* selfCopy = self;

    return new(String, selfCopy->text);
}

static int String_differ(const void* self, const void* b)
{
    const struct String* selfCopy = self;   
    const struct String* bCopy = b;   

    if (selfCopy == bCopy)
        return 0;

	if (!bCopy || bCopy->class != String)
		return 1;

	return strcmp(selfCopy->text, bCopy->text);
}

void test_string()
{
	void* a = new(String, "a");
	void* aa = clone(a);
	void* b = new(String, "b");

	printf("sizeOf(a) == %u\n", sizeof(a));
    if (differ(a, b))
		puts("ok");

	if (differ(a, aa))
		puts("differ?");

	if (a == aa)
		puts("clone?");

	delete(a);
	delete(aa);
    delete(b);

	return 0;
}
