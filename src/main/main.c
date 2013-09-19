#include "unity_fixture.h"
#include "winsockmulticast_IP_TTL.h"

static void RunAllTests(void)
{
	//RUN_TEST_GROUP(sprintf);
    RUN_TEST_GROUP(LedDriver);
}

int main(int argc, char* argv[]) 
{
	int r = main_address_list();

	main_multicast_receive();
    //main_multicast_send();
    RunAllTests();
	//return main_scalable(argc, argv);
}

