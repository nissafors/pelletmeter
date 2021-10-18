/* Copyright (C) 2021 Andreas Andersson */

#include "stub.h"
#include <cassert>
#include "Arduino.h"
#include "leds.h"

const int MAX_PIN = 14;
bool init = false;
uint8_t pinModes[MAX_PIN];
uint8_t pinStates[MAX_PIN];

void stubInit()
{
    init = true;
    for (int i = 0; i < MAX_PIN; i++)
    {
        pinModes[i] = UNSET;
        pinStates[i] = UNSET;
    }
}

void stubDeinit()
{
    init = false;
}

// Spies

uint8_t stubGetPinMode(uint8_t pin)
{
    assert(init);
    return pinModes[pin];
}

uint8_t stubGetPinState(uint8_t pin)
{
    assert(init);
    return pinStates[pin];
}

void stubSetPinState(uint8_t pin, uint8_t state)
{
    assert(init);
    pinStates[pin] = state;
}

// Stubs

void pinMode(uint8_t pin, uint8_t mode)
{
    assert(init);
    pinModes[pin] = mode;
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    assert(init);
    pinStates[pin] = val;
}

void delay(unsigned long ms)
{
    UNUSED(ms);
}