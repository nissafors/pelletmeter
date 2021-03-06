/* Copyright (C) 2021 Andreas Andersson */

#ifndef STUB_H
#define STUB_H

#include <stdint.h>

#define PINMODE_NOT_SET 0xFF

void stubInit();
void stubDeinit();
uint8_t stubGetPinMode(uint8_t pin);
void stubSetPulseIn(unsigned long pulse);

#endif // STUB.H