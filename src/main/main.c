#include "unity_fixture.h"
#include "winsockmulticast_IP_TTL.h"
#include "winsockheaders.h"

static void RunAllTests(void)
{
	//RUN_TEST_GROUP(sprintf);
    RUN_TEST_GROUP(LedDriver);
}

int main(int argc, char* argv[]) 
{
	BOOL bSender = 0;
	//return main_so_acceptconn(argc, argv);
	return main_so_broadcast(argc, argv, bSender);
}

