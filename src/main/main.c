#include "unity_fixture.h"
#include "winsocket.h"
#include "mylib.h"

static void RunAllTests(void)
{
	//RUN_TEST_GROUP(sprintf);
    RUN_TEST_GROUP(LedDriver);
}

int main(int argc, char* argv[]) 
{
	int a1 = Add(12, 23);
    int a2 = Add(23, 13);
    int a3 = Add(16, 33);
    //int ee = g_nResult;
	//TcpServer();
	//UdpReceiver();
	//UniversalServerIP61();
	//return UnityMain(argc, argv, RunAllTests);
}

