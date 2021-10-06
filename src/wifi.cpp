/* Copyright (C) 2021 Andreas Andersson */

#include "wifi.h"
#include "panic.h"
#include <Arduino.h>

// Public

WiFi::WiFi(int rx, int tx) : esp8266(rx, tx)
{
    connected = false;
}

bool WiFi::connect(const char* name, const char* pwd)
{
    esp8266.begin(9600);

    if (sendAT("AT+CWMODE=3", 5000))
    {
        snprintf( cmdBuf, CMD_BUF_SIZE, "AT+CWJAP=\"%s\",\"%s\"", name, pwd);
        connected = sendAT(cmdBuf, 10000);
    }

    return connected;
}

// Private

bool WiFi::sendAT(const char* cmd, int timeout)
{
    esp8266.setTimeout(timeout);
    Serial.print("Sending: ");
    Serial.println(cmd);
    esp8266.println(cmd);
    if (esp8266.find((char*)"OK"))
    {
        Serial.println("Got OK");
        return true;
    }
    Serial.println("Timed out wating for OK");
    return false;
}
