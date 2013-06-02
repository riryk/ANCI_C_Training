
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


#ifndef STRING_RING_H
#define STRING_RING_H

struct StringRingItem
{
	const void*             class;
	char*                   text;
    struct StringRingItem*  next;
	unsigned                count;
};

struct StringRingItem* ring;

void* StringRingItem_ctor(const void* self, va_list* app);
void* StringRingItem_dtor(void* self);
void* StringRingItem_clone(const void* _self);
int StringRingItem_differ(const void* self, const void* b);

void test_string_ring();

#endif