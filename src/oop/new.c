
#include "new.h"

#if ! defined MANY || MANY < 1
#define MANY 10
#endif

static int heap[MANY];

void* new(const void* type)
{
   int* p;

   for (p = heap + 1; p < heap + MANY; ++p)
      if (!*p)
		  break;

   assert(p < heap + MANY);
   *p = MANY;

   return p;
}

void delete(void* item)
{
   if (item)
   {
	   assert(item > heap && item < heap + MANY);
	   *item = 0;
   }
}

