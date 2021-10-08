/* Copyright (C) 2021 Andreas Andersson */

#ifndef LOG_H
#define LOG_H

class Log
{
private:
    int verbosity;
public:
    Log(int verbosity);
    void log(const char ch, int verbosity, bool lf = false);
    void log(const char* str, int verbosity, bool lf = false);
    void log(const int i, int verbosity, bool lf = false);
    void setVerbosity(int verbosity);
};

#endif // LOG_H