/* Copyright (C) 2021 Andreas Andersson */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "wifi.h"
#include "stub.h"
#include "Arduino.h"
#include "SoftwareSerial.h"

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

TEST(testGroupNormal, testConstruct)
{
    WiFi(10, 11, 2);
    CHECK_EQUAL( 10, stubGetSSReceivePin() );
    CHECK_EQUAL( 11, stubGetSSTransmitPin() );
    CHECK_EQUAL( 2, stubGetLogVerbosity() );
}

TEST(testGroupNormal, testConnectSuccessfully)
{
    auto wifi = WiFi(10, 11, 2);
    stubAppendAtCommand("AT+CWMODE=3", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CWJAP=\"testName\",\"testPwd\"", "\r\nWIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n");
    CHECK( wifi.connect("testName", "testPwd") );
}

// Functions

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
