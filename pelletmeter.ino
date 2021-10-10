/* Copyright (C) 2021 Andreas Andersson */

#include <SoftwareSerial.h>
#include "src/panic.h"
#include "src/wifi.h"
#include "src/distance.h"
#include "settings.h"

WiFi wifi(7, 6, 2);
Distance distance(11, 12);
char ipBuf[16];
int delayCounter = 0;

const int HTML_MAX_LEN = 128;
const char html[] = "<!DOCTYPE html>\n<html>\n<body>\n<h1>PELLETMETER</h1>\n<p>Distance: %d</p>\n</body>\n</html>";

void setup()
{
    Serial.begin(9600);
    Serial.println("Connecting to WiFi...");
    if (!wifi.connect(wifiSSID, wifiPassword))
    {
        PANIC("Failed to connect.");
    }
    Serial.println("Connected.");
    if (!wifi.startServer(80))
    {
        PANIC("Failed to connect.");
    }
    Serial.println("Server started.");
    wifi.getIP(ipBuf);
    Serial.print("IP address: ");
    Serial.println(ipBuf);
    Serial.println("Ready.");
}

void loop()
{
    static char htmlOut[HTML_MAX_LEN];
    static uint16_t cm = 0;

    if (wifi.getRequestDetected())
    {
        snprintf(htmlOut, HTML_MAX_LEN, html, cm);
        wifi.sendResponse(htmlOut);
        Serial.println("It was a GET request! Response sent.");
    }

    if (++delayCounter >= 100)
    {
        delayCounter = 0;
        cm = distance.detect();
        Serial.print("Distance: ");
        Serial.println(cm);
    }

    delay(10);
}
