/* Copyright (C) 2021 Andreas Andersson */

#include "stub.h"
#include <cassert>
#include "Arduino.h"

const uint8_t N_PINS = 13;
bool init = false;
uint8_t stubPinMode[N_PINS];
unsigned long stubPulseIn;

void stubInit()
{
    init = true;
    for (uint8_t i = 0; i < N_PINS; i++)
    {
        stubPinMode[i] = PINMODE_NOT_SET;
    }
    stubPulseIn = 0;
}

void stubDeinit()
{
    init = false;
}

// Spies

uint8_t stubGetPinMode(uint8_t pin)
{
    assert(init);
    return stubPinMode[pin];
}

void stubSetPulseIn(unsigned long pulse)
{
    assert(init);
    stubPulseIn = pulse;
}

// Stubs

void pinMode(uint8_t pin, uint8_t mode)
{
    assert(init);
    stubPinMode[pin] = mode;
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    UNUSED(pin);
    UNUSED(val);
    assert(init);
}

void delayMicroseconds(unsigned int us)
{
    UNUSED(us);
    assert(init);
}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
    assert(init);
    UNUSED(pin);
    UNUSED(state);
    UNUSED(timeout);
    return stubPulseIn;
}
