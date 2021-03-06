/* Copyright (C) 2021 Andreas Andersson */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "wifi.h"
#include "stub.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include <stdexcept>

// Function prototypes
void connect(WiFi* wifiPtr);

// Test groups

TEST_GROUP(testGroupBasic)
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
TEST(testGroupBasic, testConstruct)
{
    WiFi(10, 11, 2);
    CHECK_EQUAL( 10, stubGetSSReceivePin() );
    CHECK_EQUAL( 11, stubGetSSTransmitPin() );
    CHECK_EQUAL( 2, stubGetLogVerbosity() );
}

// Test that connect reports true when WiFi connection is successful
TEST(testGroupBasic, testConnectSuccessfully)
{
    auto wifi = WiFi(10, 11, 2);
    // Expected AT commands to set disconnect, set station mode and accept single connection, shall respond OK.
    stubAppendAtCommand("AT+CWQAP", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CWMODE=1", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CIPMUX=0", "\r\n\r\nOK\r\n");
    // Expected AT command to connect to WiFi, shall respond OK.
    stubAppendAtCommand("AT+CWJAP=\"testName\",\"testPwd\"", "\r\nWIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n");

    CHECK( wifi.connect("testName", "testPwd") );
}

// Test that connect fails when credentials are wrong.
TEST(testGroupBasic, testConnectFailed)
{
    auto wifi = WiFi(10, 11, 2);
    // Expected AT commands to set disconnect, set station mode and accept single connection, shall respond OK.
    stubAppendAtCommand("AT+CWQAP", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CWMODE=1", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CIPMUX=0", "\r\n\r\nOK\r\n");
    // Expected AT command to attempt connecting to WiFi, shall respond FAIL.
    stubAppendAtCommand("AT+CWJAP=\"wrongName\",\"wrongPwd\"", "+CWJAP:3\r\n\r\nFAIL\r\n");

    CHECK_FALSE(wifi.connect("wrongName", "wrongPwd"));
}

// Test that connection times out if the wifi chip fails to read expected response from serial.
TEST(testGroupBasic, testConnectTimeout)
{
    auto wifi = WiFi(10, 11, 2);
    // Expected AT commands to set disconnect, set station mode and accept single connection, shall respond OK.
    stubAppendAtCommand("AT+CWQAP", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CWMODE=1", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CIPMUX=0", "\r\n\r\nOK\r\n");
    // Expected AT command to attempt connecting to WiFi, response is rubish.
    stubAppendAtCommand("AT+CWJAP=\"anyName\",\"anyPwd\"", "493qpcn8sl");

    // Fake implementation of millis() throws out_of_range if it grows too big.
    CHECK_FALSE(wifi.connect("anyName", "anyPwd"));
}

// Test that can send as TCP client
TEST(testGroupBasic, testSendTCP)
{
    auto wifi = WiFi(10, 11, 2);
    connect(&wifi);

    // Expected AT command to connect as TCP client, shall return OK.
    stubAppendAtCommand("AT+CIPSTART=\"TCP\",\"http://www.fake.com\",80", "\r\n\r\nOK\r\n");
    // Expected AT command to send 12 bytes ("test message"), shall return OK.
    stubAppendAtCommand("AT+CIPSEND=12", "\r\n\r\nOK\r\n");
    // This is what we send.
    stubAppendAtCommand("test message", "Recv 12 bytes\r\n\r\nSEND OK\r\n");
    // Closing the connection
    stubAppendAtCommand("AT+CIPCLOSE", "\r\r\nCLOSED\r\n\r\nOK\r\n");
    CHECK(wifi.sendTCP("test message", "http://www.fake.com", 80));
}

// Functions

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}

void connect(WiFi* wifiPtr)
{
    stubAppendAtCommand("AT+CWQAP", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CWMODE=1", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CIPMUX=0", "\r\n\r\nOK\r\n");
    stubAppendAtCommand("AT+CWJAP=\"testName\",\"testPwd\"", "\r\nWIFI CONNECTED\r\nWIFI GOT IP\r\n\r\nOK\r\n");
    wifiPtr->connect("testName", "testPwd");
}