/* Copyright (C) 2021 Andreas Andersson */

#ifndef LOG_H
#define LOG_H

class Log
{
private:
    int verbosity;
public:
    Log(int verbosity);
    void log(const char ch, int verbosity);
    void log(const char* str, int verbosity);
    void setVerbosity(int verbosity);
};

#endif // LOG_H