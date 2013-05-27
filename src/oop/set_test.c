
#include "new.h"
#include "set.h"
#include "object.h"

void set_test()
{
   void* s = new(Set);
   void* a = add(s, new(Object));
   void* b = add(s, new(Object));
   void* c = new(Object);
   
   if (contains(s, a) && contains(s, b))
	   printf("ok");

   if (contains(s, c))
       printf("contains?");

   if (differ(a, add(s, a)))
       printf("differ?");

   if (contains(s, drop(s, a)))
       printf("differ?");

   delete(drop(s, b));
   delete(drop(s, c));
}
