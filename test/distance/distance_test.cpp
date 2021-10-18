/* Copyright (C) 2021 Andreas Andersson */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "distance.h"
#include "stub.h"
#include "Arduino.h"
#include <math.h>

// Function prototypes

unsigned long cmToPulse(int cm);

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

TEST(testGroupNormal, testConstruct)
{
    Distance distance(1, 2);
    CHECK_EQUAL(OUTPUT, stubGetPinMode(1));
    CHECK_EQUAL(INPUT, stubGetPinMode(2));
}

TEST(testGroupNormal, testDetect)
{
    Distance distance(1, 2);
    stubSetPulseIn(cmToPulse(5));
    CHECK_EQUAL(5, distance.detect());
    stubSetPulseIn(cmToPulse(100));
    CHECK_EQUAL(100, distance.detect());
}

// Functions

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

unsigned long cmToPulse(int cm)
{
    return (unsigned long) round((double) cm * 58.2);
}