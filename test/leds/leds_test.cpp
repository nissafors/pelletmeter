/* Copyright (C) 2021 Andreas Andersson */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "stub.h"
#include "Arduino.h"
#include "leds.h"

// Function prototypes

// Test groups

TEST_GROUP(testGroupNormal)
{
    void setup()
    {
        stubInit();
    }

    void teardown()
    {
        stubDeinit();
    }
};

// Tests

TEST(testGroupNormal, testPinsSetToOutputOnCreate)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);

    // Assigned pins should be in OUTPUT mode
    CHECK_EQUAL(OUTPUT, stubGetPinMode(5));
    CHECK_EQUAL(OUTPUT, stubGetPinMode(7));
    CHECK_EQUAL(OUTPUT, stubGetPinMode(9));
}

TEST(testGroupNormal, testLedsOffOnCreate)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);

    // Assigned pins should be driven LOW
    CHECK_EQUAL(LOW, stubGetPinState(5));
    CHECK_EQUAL(LOW, stubGetPinState(7));
    CHECK_EQUAL(LOW, stubGetPinState(9));
}

TEST(testGroupNormal, testShineGreen)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);

    leds.shine(LED_GREEN);

    // The pin for the green led alone should be driven HIGH
    CHECK_EQUAL(HIGH, stubGetPinState(5));
    CHECK_EQUAL(LOW, stubGetPinState(7));
    CHECK_EQUAL(LOW, stubGetPinState(9));
}

TEST(testGroupNormal, testShineYellow)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);

    leds.shine(LED_YELLOW);

    // The pin for the yellow led alone should be driven HIGH
    CHECK_EQUAL(LOW, stubGetPinState(5));
    CHECK_EQUAL(HIGH, stubGetPinState(7));
    CHECK_EQUAL(LOW, stubGetPinState(9));
}

TEST(testGroupNormal, testShineRed)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);

    leds.shine(LED_RED);

    // The pin for the red led alone should be driven HIGH
    CHECK_EQUAL(LOW, stubGetPinState(5));
    CHECK_EQUAL(LOW, stubGetPinState(7));
    CHECK_EQUAL(HIGH, stubGetPinState(9));
}

TEST(testGroupNormal, testTurnOff)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);

    // Shine and put out every LED
    leds.shine(LED_GREEN);
    leds.extinguish(LED_GREEN);
    leds.shine(LED_YELLOW);
    leds.extinguish(LED_YELLOW);
    leds.shine(LED_RED);
    leds.extinguish(LED_RED);

    // All pins should be driven low
    CHECK_EQUAL(LOW, stubGetPinState(5));
    CHECK_EQUAL(LOW, stubGetPinState(7));
    CHECK_EQUAL(LOW, stubGetPinState(9));
}

TEST(testGroupNormal, testAllLedsOffAfterPowerOnSequence)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);
    // Unset pin states so we fail if nothing happens in the sequence
    stubSetPinState(5, UNSET);
    stubSetPinState(7, UNSET);
    stubSetPinState(9, UNSET);

    // Check that all pins are driven low after sequence
    leds.sequence(LED_SEQUENCE_POWER_ON);
    CHECK_EQUAL(LOW, stubGetPinState(5));
    CHECK_EQUAL(LOW, stubGetPinState(7));
    CHECK_EQUAL(LOW, stubGetPinState(9));
}

TEST(testGroupNormal, testAllLedsOffAfterConnectedSequence)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);
    // Unset pin states so we fail if nothing happens in the sequence
    stubSetPinState(5, UNSET);
    stubSetPinState(7, UNSET);
    stubSetPinState(9, UNSET);

    // Check that all pins are driven low after sequence
    leds.sequence(LED_SEQUENCE_CONNECTED);
    CHECK_EQUAL(LOW, stubGetPinState(5));
    CHECK_EQUAL(LOW, stubGetPinState(7));
    CHECK_EQUAL(LOW, stubGetPinState(9));
}

TEST(testGroupNormal, testAllLedsOffAfterSendOKSequence)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);
    // Unset pin states so we fail if nothing happens in the sequence
    stubSetPinState(5, UNSET);
    stubSetPinState(7, UNSET);
    stubSetPinState(9, UNSET);

    // Check that all pins are driven low after sequence
    leds.sequence(LED_SEQUENCE_SEND_OK);
    CHECK_EQUAL(LOW, stubGetPinState(5));
    CHECK_EQUAL(LOW, stubGetPinState(7));
    CHECK_EQUAL(LOW, stubGetPinState(9));
}

TEST(testGroupNormal, testAllLedsOffAfterSendFailSequence)
{
    // Pins for green, yellow and red (the order in which they are enumerated in leds.h)
    uint8_t pins[] = {5, 7, 9};
    LEDs leds(pins);
    // Unset pin states so we fail if nothing happens in the sequence
    stubSetPinState(5, UNSET);
    stubSetPinState(7, UNSET);
    stubSetPinState(9, UNSET);

    // Check that all pins are driven low after sequence
    leds.sequence(LED_SEQUENCE_SEND_FAIL);
    CHECK_EQUAL(LOW, stubGetPinState(5));
    CHECK_EQUAL(LOW, stubGetPinState(7));
    CHECK_EQUAL(LOW, stubGetPinState(9));
}

// Functions

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
