#include "unity_fixture.h"
#include "winsocket.h"
#include "mylibimport.h"

static void RunAllTests(void)
{
	//RUN_TEST_GROUP(sprintf);
    RUN_TEST_GROUP(LedDriver);
}

int main(int argc, char* argv[]) 
{
	int a1 = MyLib_Add(12, 23);
    int a2 = MyLib_Add(23, 13);
    int a3 = MyLib_Add(16, 33);
    int ee = MyLib_g_nResult;
	//TcpServer();
	//UdpReceiver();
	//UniversalServerIP61();
	//return UnityMain(argc, argv, RunAllTests);
}

