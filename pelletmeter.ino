/* Copyright (C) 2021 Andreas Andersson */

#include <SoftwareSerial.h>
#include "src/panic.h"
#include "src/wifi.h"
#include "src/distance.h"
#include "settings.h"

WiFi wifi(7, 6, 2);
Distance distance(11, 12);
char ipBuf[16];
const uint16_t POST_BUF_SIZE = 64;
char postBuf[POST_BUF_SIZE];

void setup()
{
    // Connect to WiFi
    Serial.begin(9600);
    Serial.println("Connecting to WiFi...");
    if (!wifi.connect(wifiSSID, wifiPassword))
    {
        PANIC("Failed to connect.");
    }
    Serial.println("Connected.");

    // Get IP
    wifi.getIP(ipBuf);
    Serial.print("IP address: ");
    Serial.println(ipBuf);
    Serial.println("Ready.");

}

void loop()
{
    static uint16_t cm = 0;

    // Detect distance
    cm = distance.detect();
    Serial.print("Distance: ");
    Serial.println(cm);

    // Post to server
    snprintf(postBuf, POST_BUF_SIZE, "POST / HTTP/1.1 \r\n\r\n%d", cm);
    wifi.sendTCP(postBuf, serverAddr, serverPort);

    // Wait 5s
    delay(5000);
}
