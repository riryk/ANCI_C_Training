
#include "set.h"
#include "new.h"
#include "object.h"

struct Type
{
	size_t size;        /* size of an object */
	void (*dtor)(void*); /* destructor */
};

struct String
{
	const void* class;
	char*       text;           /* dynamic string */
};

struct Set
{
	const void* class;
    unsigned    count;
};

struct Object
{
	unsigned count;
	struct Set* in;
};

static const size_t SetSize = sizeof(struct Set);
static const size_t ObjectSize = sizeof(struct Object);

const void* Set = &SetSize;
const void* Object = &ObjectSize;

void* new(const void* type)
{
   const size_t size = *(const size_t*)type;
   void* p = calloc(1, size);

   assert(p);
   return p;
}

void delete(void* item)
{
   free(item);
}

void* add(void* set, const void* element)
{
	struct Set* setLocal = set;
    struct Object* elemLocal = (void*)element;

    assert(setLocal);
	assert(elemLocal);

	if (!elemLocal->in)
        elemLocal->in = setLocal;
	else
		assert(elemLocal->in == setLocal);

	++elemLocal->count;
	++setLocal->count;

	return element;
}

void* find(const void* set, const void* element)
{
    struct Object* elemLocal = (void*)element;

	assert(set);
	assert(element);

	return elemLocal->in == set ? (void*)element : 0;
}

int contains(const void* set, const void* element)
{
    return find(set, element) != 0;
}

int differ(const void* a, const void* b)
{
   return a != b;
}

void* drop(void* set, const void* element)
{
	struct Set* setLocal = set;
	struct Object* elemLocal = find(set, element);

	if (elemLocal)
	{
		if (--elemLocal->count == 0)
			elemLocal->in = 0;
        --elemLocal->count;
	}

	return elemLocal;
}

unsigned count(const void* set)
{
    struct Set* setLocal = set;
	assert(setLocal);
	return setLocal->count;
}