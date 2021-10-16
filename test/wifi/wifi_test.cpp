/* Copyright (C) 2021 Andreas Andersson */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "wifi.h"
#include "stub.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include <stdexcept>

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

// Test that transmit- and receive pins and log verbosity are set by constructor.
TEST(testGroupNormal, testConstruct)
{
    WiFi(10, 11, 2);
    CHECK_EQUAL( 10, stubGetSSReceivePin() );
    CHECK_EQUAL( 11, stubGetSSTransmitPin() );
    CHECK_EQUAL( 2, stubGetLogVerbosity() );
}

// Test that connect reports true when WiFi connection is successful
TEST(testGroupNormal, testConnectSuccessfully)
{
    auto wifi = WiFi(10, 11, 2);
    // Expected AT command to set SoftAP+Station, shall respond OK.
    stubAppendAtCommand("AT+CWMODE=3", "\r\n\r\nOK\r\n");
    // Expected AT command to connect to WiFi, shall respond OK.
    stubAppendAtCommand("AT+CWJAP=\"testName\",\"testPwd\"", "\r\nWIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n");

    CHECK( wifi.connect("testName", "testPwd") );
}

// Test that connect fails when credentials are wrong.
TEST(testGroupNormal, testConnectFailed)
{
    auto wifi = WiFi(10, 11, 2);
    // Expected AT command to set SoftAP+Station, shall respond OK.
    stubAppendAtCommand("AT+CWMODE=3", "\r\n\r\nOK\r\n");
    // Expected AT command to attempt connecting to WiFi, shall respond FAIL.
    stubAppendAtCommand("AT+CWJAP=\"wrongName\",\"wrongPwd\"", "+CWJAP:3\r\n\r\nFAIL\r\n");

    CHECK_FALSE( wifi.connect("wrongName", "wrongPwd") );
}

// Test that connection times out if the wifi chip fails to read expected response from serial.
TEST(testGroupNormal, testConnectTimeout)
{
    auto wifi = WiFi(10, 11, 2);
    // Expected AT command to set SoftAP+Station, shall respond OK.
    stubAppendAtCommand("AT+CWMODE=3", "\r\n\r\nOK\r\n");
    // Expected AT command to attempt connecting to WiFi, response is rubish.
    stubAppendAtCommand("AT+CWJAP=\"anyName\",\"anyPwd\"", "493qpcn8sl");

    // Fake implementation of millis() throws out_of_range if it grows too big.
    CHECK_FALSE(wifi.connect("anyName", "anyPwd"));
}

// Test that getIP returns false when not connected.
TEST(testGroupNormal, testGetIPFailWhenNotConnected)
{
    auto wifi = WiFi(10, 11, 2);
    char ip[16] = "aaa.bbb.ccc.ddd";
    CHECK_FALSE(wifi.getIP(ip));
}

// Test that can get IP address if connected.
TEST(testGroupNormal, testGetIPWhenConnected)
{
    auto wifi = WiFi(10, 11, 2);
    // Connect to WiFi
    stubAppendAtCommand("AT+CWMODE=3", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CWJAP=\"testName\",\"testPwd\"", "\r\nWIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n");
    wifi.connect("testName", "testPwd");

    // Full length IP: 192.168.100.100
    // Expected AT command to get IP address, shall respond with info and OK.
    stubAppendAtCommand("AT+CIFSR", "+CIFSR:APIP,\"192.168.4.1\"\r\n+CIFSR:APMAC,\"86:f3:eb:5b:10:e6\"\r\n+CIFSR:STAIP,\"192.168.100.100\"\r\n+CIFSR:STAMAC,\"84:f3:eb:5b:10:e6\"\r\n\r\nOK\r\n");
    // Get IP
    char ip[16] = "aaa.bbb.ccc.ddd";
    CHECK(wifi.getIP(ip));
    STRCMP_EQUAL( "192.168.100.100", ip );

    // Shorter IP: 192.168.1.100
    stubAppendAtCommand("AT+CIFSR", "+CIFSR:APIP,\"192.168.4.1\"\r\n+CIFSR:APMAC,\"86:f3:eb:5b:10:e6\"\r\n+CIFSR:STAIP,\"192.168.1.100\"\r\n+CIFSR:STAMAC,\"84:f3:eb:5b:10:e6\"\r\n\r\nOK\r\n");
    CHECK(wifi.getIP(ip));
    STRCMP_EQUAL( "192.168.1.100", ip );
}

// Test that getIP return false on unexpected response.
TEST(testGroupNormal, testGetIPFailOnBadResponse)
{
    auto wifi = WiFi(10, 11, 2);
    // Connect to WiFi
    stubAppendAtCommand("AT+CWMODE=3", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CWJAP=\"testName\",\"testPwd\"", "\r\nWIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n");
    wifi.connect("testName", "testPwd");

    // Expected AT command to get IP address, malformed response.
    stubAppendAtCommand("AT+CIFSR", "+CIFSR:APIP,\"192.168.4.1\"\r\n+CIFSR:AP___RUBBISH___:f3:eb:5b:10:e6\"\r\n\r\nOK\r\n");
    // Get IP
    char ip[16] = "aaa.bbb.ccc.ddd";
    CHECK_FALSE(wifi.getIP(ip));
}

// Functions

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
