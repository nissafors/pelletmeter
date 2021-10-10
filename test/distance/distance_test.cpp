#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "distance.h"
#include "stub.h"
#include "Arduino.h"

TEST_GROUP(FirstTestGroup)
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

TEST(FirstTestGroup, testConstruct)
{
    Distance distance(1, 2);
    CHECK_EQUAL(1, distance.trigPin);
    CHECK_EQUAL(2, distance.echoPin);
    CHECK_EQUAL(OUTPUT, stubGetPinMode(distance.trigPin));
    CHECK_EQUAL(INPUT, stubGetPinMode(distance.echoPin));
}

TEST(FirstTestGroup, detect)
{
    Distance distance(1, 2);
    stubSetPulseIn(291); // 291us = 5cm
    CHECK_EQUAL(5, distance.detect());
    stubSetPulseIn(5820); // 5820 = 100cm
    CHECK_EQUAL(100, distance.detect());
}

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}