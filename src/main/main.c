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
	NBATest1();
}

