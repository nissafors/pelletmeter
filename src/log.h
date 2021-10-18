/* Copyright (C) 2021 Andreas Andersson */

#ifndef LOG_H
#define LOG_H

/**
 * Serial logger.
 */
class Log
{
private:
    int verbosity;

public:
    /**
     * Construct a new Log instance.
     * 
     * @param verbosity Only messages with a verbosity level at or below this value will be logged.
     */
    Log(int verbosity);

    /**
     * Log one chararacter.
     * 
     * @param ch        The character to log.
     * @param verbosity The verbosity level of this message.
     * @param lf        If true, add linefeed at the end of the message. (Default: false.)
     */
    void log(const char ch, int verbosity, bool lf = false);

    /**
     * Log a string of characters.
     * 
     * @param str       The string to log.
     * @param verbosity The verbosity level of this message.
     * @param lf        If true, add linefeed at the end of the message. (Default: false.)
     */
    void log(const char* str, int verbosity, bool lf = false);

    /**
     * Log an integer.
     * 
     * @param ch        The integer to log.
     * @param verbosity The verbosity level of this message.
     * @param lf        If true, add linefeed at the end of the message. (Default: false.)
     */
    void log(const int i, int verbosity, bool lf = false);

    /**
     * Set at which verbosity level messages are logged.
     * 
     * @param verbosity Only messages with a verbosity level at or below this value will be logged.
     */
    void setVerbosity(int verbosity);
};

#endif // LOG_H