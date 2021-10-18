/* Copyright (C) 2021 Andreas Andersson */

#include "leds.h"

// Public

const int BLINK_DELAY_MS = 100;

LEDs::LEDs(const uint8_t* pins)
{
    for (int i = 0; i < N_LEDS; i++)
    {
        this->pins[i] = pins[i];
        pinMode(pins[i], OUTPUT);
        digitalWrite(pins[i], LOW);
    }
}

void LEDs::shine(const led_t led)
{
    digitalWrite(pins[led], HIGH);
}

void LEDs::extinguish(led_t led)
{
    digitalWrite(pins[led], LOW);
}

void LEDs::sequence(const sequence_t sequence)
{
    static const led_t allLEDs[] = {LED_GREEN, LED_YELLOW, LED_RED};
    static const uint8_t nLEDs = 3;
    
    extinguishAll();

    switch (sequence)
    {
    case LED_SEQUENCE_POWER_ON:
        // Blink each LED once
        for(int i = 0; i < nLEDs; i++)
        {
            blink(&allLEDs[i], 1);
        }
        // Blink all once
        blink(allLEDs, nLEDs);
        break;
    case LED_SEQUENCE_CONNECTED:
        // Blink green x5
        for (int i = 0; i < 5; i++)
        {
            blink(&allLEDs[LED_GREEN], 1);
        }
        break;
    case LED_SEQUENCE_SEND_OK:
        // Blink green twice
        blink(&allLEDs[LED_GREEN], 1);
        blink(&allLEDs[LED_GREEN], 1);
        break;
    case LED_SEQUENCE_SEND_FAIL:
        // Blink red twice
        blink(&allLEDs[LED_RED], 1);
        blink(&allLEDs[LED_RED], 1);
        break;
    default:
        break;
    }
}

// Private

void LEDs::extinguishAll()
{
    for (int i = 0; i < N_LEDS; i++)
    {
        digitalWrite(pins[i], LOW);
    }
}

void LEDs::blink(const led_t* leds, const uint8_t nLeds)
{
    for (int i = 0; i < nLeds; i++)
    {
        digitalWrite(pins[leds[i]], HIGH);
    }

    delay(BLINK_DELAY_MS);

    for (int i = 0; i < nLeds; i++)
    {
        digitalWrite(pins[leds[i]], LOW);
    }

    delay(BLINK_DELAY_MS);
}