/* Copyright (C) 2021 Andreas Andersson */

#include "stub.h"
#include <cassert>
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "log.h"
#include <string>
#include <deque>
#include <stdexcept>

const unsigned long MAX_MS = 0xFFFF;

bool init = false;
int logVerbosity;
uint8_t ssRcvPin;
uint8_t ssTmtPin;
std::deque<std::string> atCommands;
std::deque<std::string> atResponses;
std::string buffer;
int bufIdx;
unsigned long ms;

void stubInit()
{
    init = true;
    logVerbosity = 0;
    ssRcvPin = 0;
    ssTmtPin = 0;
    atCommands.clear();
    buffer.clear();
    bufIdx = 0;
    ms = 0;
}

void stubDeinit()
{
    init = false;
}

// Spies
int stubGetLogVerbosity()
{
    assert(init);
    return logVerbosity;
}

uint8_t stubGetSSReceivePin()
{
    assert(init);
    return ssRcvPin;

}

uint8_t stubGetSSTransmitPin()
{
    assert(init);
    return ssTmtPin;
}

void stubAppendAtCommand(std::string cmd, std::string response)
{
    atCommands.push_back(cmd);
    atResponses.push_back(response);
}

// Stubs

SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin)
{
    ssRcvPin = receivePin;
    ssTmtPin = transmitPin;
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
    char byte = buffer[0];
    buffer.erase(0, 1);  
    return byte;
}

int SoftwareSerial::available()
{
    return buffer.length();
}

size_t SoftwareSerial::print(const char* str)
{
    return println(str);
}

size_t SoftwareSerial::println(const char* str)
{
    if (atCommands.front() == str)
    {
        atCommands.pop_front();
        buffer += atResponses.front();
        atResponses.pop_front();
    }
    return strlen(str);
}

void SoftwareSerial::flush()
{
}

Log::Log(int verbosity)
{
    logVerbosity = verbosity;
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
    if (++ms > MAX_MS)
    {
        throw std::out_of_range("ms too big.");
    }
    return ms;
}

void delay(unsigned long ms)
{
    UNUSED(ms);
}