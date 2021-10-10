#include <stdint.h>

#define PINMODE_NOT_SET 0xFF

void stubInit();
void stubDeinit();
uint8_t stubGetPinMode(uint8_t pin);
void stubSetPulseIn(unsigned long pulse);