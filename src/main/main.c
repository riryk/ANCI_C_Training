#include "unity_fixture.h"
#include "winsocket.h"
#include "mylibimport.h"
#include "winsocketselect.h"
#include "winsocketasyncwindow.h"
#include "winsocketeventselect.h"
#include "winsocketoverlapped1.h"
#include "str.h"
#include "winsocketcallback.h"
#include "winsocketcomport.h"
#include "winsocketscalable.h"

static void RunAllTests(void)
{
	//RUN_TEST_GROUP(sprintf);
    RUN_TEST_GROUP(LedDriver);
}

int main(int argc, char* argv[]) 
{
	return main_scalable(argc, argv);
}

