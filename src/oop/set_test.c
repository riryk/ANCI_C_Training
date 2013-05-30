
#include "new.h"
#include "set.h"
#include "object.h"

void set_test()
{
   void* s = new_set(Set);
   void* obj1 = new_set(Object);
   void* obj2 = new_set(Object);
   void* a = add_set(s, obj1);
   void* a1 = add_set(s, obj1);
   void* b = add_set(s, obj2);
   void* c = new_set(Object);
   
   void* s1 = new_set(Set);
   void* a2 = add_set(s1, new_set(Object));
   void* b1 = add_set(s1, new_set(Object));

   if (contains_set(s, a) && contains_set(s, b))
	   printf("ok");

   if (contains_set(s, c))
       printf("contains?");

   if (differ_set(a, add_set(s, a)))
       printf("differ?");

   if (contains_set(s, drop_set(s, a)))
       printf("differ?");

   delete_set(drop_set(s, b));
   delete_set(drop_set(s, c));
}
