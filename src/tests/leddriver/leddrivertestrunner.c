#include "unity_fixture.h"
#include "leddrivertest.h"

TEST_GROUP_RUNNER(LedDriver)
{
   RUN_TEST_CASE(LedDriver, LedsOffAfterCreate);
   RUN_TEST_CASE(LedDriver, TurnOnLedOne);
   RUN_TEST_CASE(LedDriver, TurnOffLedOne);
}
