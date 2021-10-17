/* Copyright (C) 2021 Andreas Andersson */

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestHarness.h"
#include "Arduino.h"
#include "request.h"

// Function prototypes

// Test groups

TEST_GROUP(testGroupNormal)
{
};

// Tests

TEST(testGroupNormal, testCreatePostRequest)
{
    const char* expected = "POST / HTTP/1.1 \r\nHost: www.example.com:80\r\nContent-Type: application/json\r\nContent-Length: 13\r\n\r\n{\n  cm: 111\n}";
    char* actual = createPostRequest(111, "www.example.com", 80);
    STRCMP_EQUAL( expected, actual );
}

// Functions

int main(int ac, char** av)
{
    return CommandLineTestRunner::RunAllTests(ac, av);
}
