#include "unity_fixture.h"
#include "winsockregisterservice.h"

static void RunAllTests(void)
{
	//RUN_TEST_GROUP(sprintf);
    RUN_TEST_GROUP(LedDriver);
}

int main(int argc, char* argv[])
{
	//testFenwikTree();
	return main_register_service(argc, argv, 1);
}

