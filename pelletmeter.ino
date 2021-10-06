/* Copyright (C) 2021 Andreas Andersson */

#include <SoftwareSerial.h>
#include "src/panic.h"
#include "src/wifi.h"
#include "settings.h"

WiFi wifi(7, 6);

void setup()
{
    Serial.begin(9600);
    Serial.println("Connecting to WiFi...");
    if (!wifi.connect(wifiSSID, wifiPassword))
    {
        PANIC("Failed to connect.");
    }
    Serial.println("Connected.");
    
}

void loop()
{
}
