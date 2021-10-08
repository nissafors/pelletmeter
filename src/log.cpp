/* Copyright (C) 2021 Andreas Andersson */

#include "log.h"
#include <Arduino.h>

Log::Log(int verbosity) : verbosity(verbosity)
{
}

void Log::log(const char ch, int verbosity)
{
    if (verbosity <= this->verbosity)
    {
        if (ch == '\n')
        {
            Serial.print("<LF>\n");
        }
        else if (ch == '\r')
        {
            Serial.print("<CR>");
        }
        else if (ch >= 32 && ch <= 126)
        {
            Serial.print(ch);
        }
        else
        {
            Serial.print('<');
            Serial.print((uint8_t) ch, DEC);
            Serial.print('>');
        }
    }
}

void Log::log(const char* str, int verbosity)
{
    if (verbosity <= this->verbosity)
    {
        Serial.print(str);
    }
}

void Log::setVerbosity(int verbosity)
{
    this->verbosity = verbosity;
}
