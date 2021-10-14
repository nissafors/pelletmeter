/* Copyright (C) 2021 Andreas Andersson */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "wifi.h"
#include "stub.h"
#include "Arduino.h"

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

TEST(testGroupNormal, testFail)
{
    FAIL("Always fail!");
}

// Functions

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
