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
}

bool WiFi::connect(const char* ssid, const char* pwd)
{
    esp8266.begin(9600);

    // Rest a moment before making sure we're not connected to any access points.
    delay(200);
    if (executeAT("AT+CWQAP", atBuf, AT_BUF_SIZE, LONG_TIMEOUT))
    {
        // Wait a moment to avoid strange serial buffer behavior.
        delay(100);
        // Set station mode and single connection.
        if (executeAT("AT+CWMODE=1", atBuf, AT_BUF_SIZE, LONG_TIMEOUT) && executeAT("AT+CIPMUX=0", atBuf, AT_BUF_SIZE, SHORT_TIMEOUT))
        {
            // Connect to access point.
            snprintf(atBuf, AT_BUF_SIZE, "AT+CWJAP=\"%s\",\"%s\"", ssid, pwd);
            connected = executeAT(atBuf, atBuf, AT_BUF_SIZE, LONG_TIMEOUT);
        }
    }

    return connected;
}

bool WiFi::sendTCP(const char* msg, const char* address, int port)
{
    bool result = false;

    // Connect to given address
    snprintf(atBuf, AT_BUF_SIZE, "AT+CIPSTART=\"TCP\",\"%s\",%d", address, port);
    if (executeAT(atBuf, atBuf, AT_BUF_SIZE, LONG_TIMEOUT))
    {
        // Enter send mode
        snprintf(atBuf, AT_BUF_SIZE, "AT+CIPSEND=%d", (int) strlen(msg));
        if (executeAT(atBuf, atBuf, AT_BUF_SIZE, LONG_TIMEOUT))
        {
            // Send data
            logger.log(msg, 2, true);
            esp8266.print(msg);
            char byte;
            int i = 0;
            // Await SEND OK
            while(readByte(&byte, 100))
            {
                logger.log(byte, 1);
                atBuf[i] = byte;
                if (i > 8 && strncmp(&atBuf[i-8], "SEND OK\r\n", 9) == 0)
                {
                    result = true;
                }
                i++;
            }
        }
    }

    return result;
}

// Private

bool WiFi::executeAT(const char* cmd, char* rcvPtr, int len, int timeout)
{
    bool result = false;
    int i = 0;

    // Flush rx buffer
    while (esp8266.available())
    {
        esp8266.read();
    }

    // Send command
    esp8266.println(cmd);
    esp8266.flush();

    unsigned long then = millis();

    // Parse response
    while (i < len)
    {
        if (millis() - then > (unsigned long) timeout)
        {
            break;
        }

        if (readByte(&rcvPtr[i], 100))
        {
            logger.log(rcvPtr[i], 1);

            if (i > 3 && strncmp(&rcvPtr[i-3], "OK\r\n", 4) == 0)
            {
                result = true;
                break;
            }
            else if (i > 6 && strncmp(&rcvPtr[i-6], "ERROR\r\n", 7) == 0)
            {
                result = false;
                break;
            }
            else if (i > 6 && strncmp(&rcvPtr[i-4], "FAIL\r\n", 5   ) == 0)
            {
                result = false;
                break;
            }

            ++i;
        }
    }
    
    return result;
}

bool WiFi::readByte(char* chPtr, int timeout)
{
    bool result = false;
    unsigned long start = millis();

    while (millis() - start < (unsigned long) timeout)
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
