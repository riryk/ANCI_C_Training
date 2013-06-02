
#include "string_ring.h"
#include "new.h"
#include "class.h"

static const struct Class _StringRingItem = 
{
	sizeof(struct StringRingItem),
    StringRingItem_ctor,
    StringRingItem_dtor,
    StringRingItem_clone,
    StringRingItem_differ
};

const void* StringRingItem = &_StringRingItem;

static void* StringRingItem_ctor(const void* self, va_list* app)
{
	struct StringRingItem* selfCopy = self;
	const char* text = va_arg(*app, const char*);

	selfCopy->text = malloc(strlen(text) + 1);
	assert(selfCopy->text);
    strcpy(selfCopy->text, text);

    if (ring)
	{
        struct StringRingItem* p = ring;
		do
		{
			if (strcmp(p->text, text) == 0)
		    {
				++p->count;
				free(selfCopy);
				return p;
		    }
		}
		while ((p = p->next) != ring);
	}
	else
        ring = selfCopy;
    
	selfCopy->next = ring->next;
	ring->next = selfCopy;
	selfCopy->count = 1;
}

static void* StringRingItem_dtor(void* self)
{
    struct StringRingItem* selfCopy = self;

	if (--selfCopy->count > 0)
		return 0;

	assert(ring);

    if (ring == selfCopy)
		ring = selfCopy->next;

	if (ring == selfCopy)
		ring = 0;
	else
	{
        struct StringRingItem* p = ring;
		while (p->next != selfCopy)
		{
			p = p->next;
			assert(p != ring);
		}
		p->next = selfCopy->next;
	}

	free(selfCopy->text);
	selfCopy->text = 0;

	return selfCopy;
}

static void* StringRingItem_clone(const void* _self)
{
    struct StringRingItem* self = (void*)_self;

    ++self->count;
    return self;
}

static int StringRingItem_differ(const void* self, const void* b)
{
    const struct StringRingItem* selfCopy = self;   
    const struct StringRingItem* bCopy = b;   

    if (selfCopy == bCopy)
        return 0;

	if (!bCopy || bCopy->class != StringRingItem)
		return 1;

	return strcmp(selfCopy->text, bCopy->text);
}

void test_string_ring()
{
    void* a = new(StringRingItem, "a");
	void* aa = clone(a);
	void* b = new(StringRingItem, "b");

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
