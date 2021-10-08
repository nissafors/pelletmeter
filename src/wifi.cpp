/* Copyright (C) 2021 Andreas Andersson */

#include "wifi.h"
#include "panic.h"
#include <Arduino.h>

static const int SHORT_TIMEOUT = 5000;
static const int LONG_TIMEOUT = 10000;

// Public

WiFi::WiFi(int rx, int tx, int verbosity) : esp8266(rx, tx), logger(verbosity)
{
    connected = false;
    requestBufIdx = 0;
    memset(requestBuf, 0, REQUEST_BUF_SIZE);
}

bool WiFi::connect(const char* name, const char* pwd)
{
    esp8266.begin(9600);

    if (executeAT("AT+CWMODE=1", rcvBuf, RCV_BUF_SIZE))
    {
        snprintf(cmdBuf, CMD_BUF_SIZE, "AT+CWJAP=\"%s\",\"%s\"", name, pwd);
        connected = executeAT(cmdBuf, rcvBuf, RCV_BUF_SIZE);
    }

    return connected;
}

bool WiFi::startServer(int port)
{
    bool result = false;

    if (connected && executeAT("AT+CIPMUX=1", rcvBuf, RCV_BUF_SIZE))
    {
        snprintf(cmdBuf, CMD_BUF_SIZE, "AT+CIPSERVER=1,%d", port);
        result = executeAT(cmdBuf, rcvBuf, RCV_BUF_SIZE);
    }

    return result;
}

bool WiFi::getIP(char* ipPtr)
{
    bool result = false;

    if (connected)
    {
        int i, j;
        int nRead = executeAT("AT+CIFSR", rcvBuf, RCV_BUF_SIZE);
        for (i = 0, j = 25; i < 15; i++, j++)
        {
            if (rcvBuf[j] == '"')
            {
                break;
            }
            ipPtr[i] = rcvBuf[j];
        }
        ipPtr[i] = '\0';
    }

    return result;
}

bool WiFi::getRequestDetected()
{
    bool result = false;

    while (esp8266.available() && requestBufIdx < REQUEST_BUF_SIZE)
    {
        requestBuf[requestBufIdx] = esp8266.read();
        logger.log(requestBuf[requestBufIdx], 1);

        if (requestBufIdx >= 4 && strncmp(&requestBuf[requestBufIdx-3], "+IPD", 4) == 0)
        {
            requestBufIdx = 0;
            result = isGetRequest();
            break;
        }

        requestBufIdx++;
    }

    if (requestBufIdx >= REQUEST_BUF_SIZE)
    {
        requestBufIdx = 0;
    }

    return result;
}

void WiFi::sendResponse(const char* body)
{
//    static const char* headerPart1 = "HTTP/1.1 200 OK\r\nContent-Length: 85\r\nContent-Type: text/html\r\nConnection: Closed\r\n\r\n";
//
//    if (sendAT("AT+CIPSEND=0,85", SHORT_TIMEOUT))
//    {
//        esp8266.print(headerPart1);
//    }
}

// Private

bool WiFi::executeAT(const char* cmd, char* rcvPtr, int len)
{
    bool result = false;
    int i = 0;

    esp8266.println(cmd);

    // Flush buffers
    esp8266.flush();
    while (esp8266.available())
    {
        esp8266.read();
    }

    // Send command and parse response
    while (i < len)
    {
        if (esp8266.available())
        {
            rcvPtr[i] = esp8266.read();
            logger.log(rcvPtr[i], 1);
            if (i > 3 && strncmp(&rcvPtr[i-3], "OK\r\n", 4) == 0)
            {
                result = true;
                break;
            }
            if (i > 6 && strncmp(&rcvPtr[i-6], "ERROR\r\n", 7) == 0)
            {
                result = false;
                break;
            }
            i++;
        }
    }
    
    return result;
}

bool WiFi::isGetRequest()
{
    bool result = false;

    for (requestBufIdx = 0; requestBufIdx < REQUEST_BUF_SIZE; requestBufIdx++)
    {
        if (!readByte(&requestBuf[requestBufIdx], 50))
        {
            result = false;
            break;
        }
        logger.log(requestBuf[requestBufIdx], 1);
        if (requestBufIdx >= 14)
        {
            if (!result && strncmp(&requestBuf[requestBufIdx-13], "GET / HTTP/1.1", 14) == 0)
            {
                result = true;
            }
            if (strncmp(&requestBuf[requestBufIdx-3], "\r\n\r\n", 4) == 0)
            {
                break;
            }
        }
    }

    return result;
}

bool WiFi::readByte(char* chPtr, int timeout)
{
    bool result = false;
    unsigned long start = millis();

    while (millis() - start < timeout)
    {
        if (esp8266.available())
        {
            *chPtr = esp8266.read();
            result = true;
            break;
        }
    }

    return result;
}
