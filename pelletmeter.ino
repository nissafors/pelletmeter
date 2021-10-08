/* Copyright (C) 2021 Andreas Andersson */

#include <SoftwareSerial.h>
#include "src/panic.h"
#include "src/wifi.h"
#include "settings.h"

WiFi wifi(7, 6, 2);
char ipBuf[16];

const char html[] = "<!DOCTYPE html>\n<html>\n<body>\n<h1>PELLETMETER</h1>\n<p>It's working!</p>\n</body>\n</html>";
 
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
    if (wifi.getRequestDetected())
    {
        wifi.sendResponse(html);
        Serial.println("It was a GET request! Response sent.");
    }
}
