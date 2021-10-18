/* Copyright (C) 2021 Andreas Andersson */

#ifndef STUB_H
#define STUB_H

#include <stdint.h>

const int UNSET = 0xFF;

enum seqEvent
{
    SEQ_EVENT_NONE,
    SEQ_EVENT_LED_GREEN,
    SEQ_EVENT_LED_YELLOW,
    SEQ_EVENT_LED_RED,
    SEQ_EVENT_DELAY
};

void stubInit();
void stubDeinit();

uint8_t stubGetPinMode(uint8_t pin);
uint8_t stubGetPinState(uint8_t pin);
void stubSetPinState(uint8_t pin, uint8_t state);

#endif // STUB.H