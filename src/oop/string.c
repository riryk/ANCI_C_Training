
#include "string.h"
#include "new.h"


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
