/* Copyright (C) 2021 Andreas Andersson */

#include "log.h"
#include <Arduino.h>

Log::Log(int verbosity) : verbosity(verbosity)
{
}

void Log::log(const char ch, int verbosity, bool lf)
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

        if (lf)
        {
            Serial.print('\n');
        }
    }
}

void Log::log(const char* str, int verbosity, bool lf)
{
    if (verbosity <= this->verbosity)
    {
        int i = 0;
        while (str[i] != '\0')
        {
            log(str[i++], verbosity);
        }
        if (lf)
        {
            Serial.print('\n');
        }
    }
}

void Log::log(const int i, int verbosity, bool lf)
{
    if (verbosity <= this->verbosity)
    {
        Serial.print(i);
        if (lf)
        {
            Serial.print('\n');
        }
    }
}

void Log::setVerbosity(int verbosity)
{
    this->verbosity = verbosity;
}
