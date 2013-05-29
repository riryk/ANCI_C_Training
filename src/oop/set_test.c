
#include "new.h"
#include "set.h"
#include "object.h"

void set_test()
{
   void* s = new(Set);
   void* obj1 = new(Object);
   void* obj2 = new(Object);
   void* a = add(s, obj1);
   void* a1 = add(s, obj1);
   void* b = add(s, obj2);
   void* c = new(Object);
   
   void* s1 = new(Set);
   void* a2 = add(s1, new(Object));
   void* b1 = add(s1, new(Object));

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
