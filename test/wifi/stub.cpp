/* Copyright (C) 2021 Andreas Andersson */

#include "stub.h"
#include <cassert>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "log.h"

bool init = false;

void stubInit()
{
    init = true;
}

void stubDeinit()
{
    init = false;
}

// Spies

// Stubs

SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin)
{
    UNUSED(receivePin);
    UNUSED(transmitPin);
}

void SoftwareSerial::begin(long speed)
{
    UNUSED(speed);
}

size_t SoftwareSerial::write(const char *str)
{
    UNUSED(str);
    return 0;
}

int SoftwareSerial::read()
{
    return 0;
}

int SoftwareSerial::available()
{
    return 0;
}

size_t SoftwareSerial::print(const char* str)
{
    UNUSED(str);
    return 0;
}

size_t SoftwareSerial::println(const char* str)
{
    UNUSED(str);
    return 0;
}

void SoftwareSerial::flush()
{
}

Log::Log(int verbosity)
{
    UNUSED(verbosity);
}

void Log::log(const char ch, int verbosity, bool lf)
{
    UNUSED(ch);
    UNUSED(verbosity);
    UNUSED(lf);
}

void Log::log(const char* str, int verbosity, bool lf)
{
    UNUSED(str);
    UNUSED(verbosity);
    UNUSED(lf);
}

void Log::log(const int i, int verbosity, bool lf)
{
    UNUSED(i);
    UNUSED(verbosity);
    UNUSED(lf);
}

unsigned long millis()
{
    return 0L;
}