#include "unity_fixture.h"
#include "winsockmulticast_IP_TTL.h"

static void RunAllTests(void)
{
	//RUN_TEST_GROUP(sprintf);
    RUN_TEST_GROUP(LedDriver);
}

int main(int argc, char* argv[]) 
{
	return main_get_atm_address(argc, argv);
	//return main_addr_list_query(argc, argv);
    //RunAllTests();
}

