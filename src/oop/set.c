
#include "set.h"
#include "new.h"
#include "object.h"

#if ! defined MANY || MANY < 1
#define MANY 10
#endif

static int heap[MANY];

void* new(const void* type)
{
   int* p;

   for (p = heap + 1; p < heap + MANY; ++p)
   {
      if (!*p)
	  {
		  break;
	  }
   }

   assert(p < heap + MANY);
   *p = MANY;

   return p;
}

void delete(void* item)
{
   int* p_item = item;
   if (item)
   {
	   assert(item > heap && item < heap + MANY);
	   *p_item = 0;
   }
}

void* add(void* set, const void* element)
{
   int* int_set = set;
   int* int_elem = element;

   assert(set > heap && set < heap + MANY);
   assert(*int_set == MANY);
   assert(element > heap && element < heap + MANY);

   if (*int_elem == MANY)
	   *int_elem = int_set - heap;
   else
	   assert(*int_elem == int_set - heap);

   return (void*)int_elem;
}

void* find(const void* set, const void* element)
{
    const int* int_set = set;
	const int* int_elem = element;

	assert(int_set > heap && int_set < heap + MANY);
	assert(*int_set == MANY);
	assert(int_elem > heap && int_elem < heap + MANY);
	assert(*int_elem);

	return *int_elem == int_set - heap ? element : 0;
}

void* drop(void* set, const void* element)
{
   int* elem = find(set, element);
   if (elem)
	   *elem = MANY;
   return elem;
}

int contains(const void* set, const void* element)
{
   return find(set, element) != 0;
}

int differ(const void* a, const void* b)
{
   return a != b;
}
