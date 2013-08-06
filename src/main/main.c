#include "unity_fixture.h"
#include "winsocket.h"
#include "mylibimport.h"
#include "winsocketselect.h"
#include "winsocketasyncwindow.h"
#include "winsocketeventselect.h"

static void RunAllTests(void)
{
	//RUN_TEST_GROUP(sprintf);
    RUN_TEST_GROUP(LedDriver);
}

int main(int argc, char* argv[]) 
{
	return socket_server_event();
}

