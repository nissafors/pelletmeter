/* Copyright (C) 2021 Andreas Andersson */

#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>

typedef enum
{
    LED_GREEN,
    LED_YELLOW,
    LED_RED,
    N_LEDS 
} led_t;

typedef enum
{
    LED_SEQUENCE_POWER_ON,
    LED_SEQUENCE_CONNECTED,
    LED_SEQUENCE_SEND_OK,
    LED_SEQUENCE_SEND_FAIL,
    N_LED_SEQUENCES
} sequence_t;

class LEDs
{
private:
    uint8_t pins[N_LEDS];
    void extinguishAll();
    void blink(const led_t* leds, const uint8_t nLeds);
public:
    LEDs(const uint8_t* pins);
    void shine(const led_t led);
    void extinguish(const led_t led);
    void sequence(const sequence_t sequence);
};

#endif // LEDS_H