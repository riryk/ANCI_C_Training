#include <stdint.h>

void LedDriver_Create(uint16_t* address);
void LedDriver_Destroy();
void LedDriver_TurnOn(int ledNumber);
void LedDriver_TurnOff(int ledNumber);