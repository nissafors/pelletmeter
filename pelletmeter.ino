/* Copyright (C) 2021 Andreas Andersson */

#include <SoftwareSerial.h>
#include "src/panic.h"
#include "src/wifi.h"
#include "src/distance.h"
#include "src/request.h"
#include "src/leds.h"
#include "settings.h"

// Global data

const uint16_t POST_BUF_SIZE = 64;
char ipBuf[16];
uint8_t ledPins[] = {2, 4, 8};

WiFi wifi(7, 6, 2);
Distance distance(11, 12);
LEDs leds(ledPins);

// Functions

void setup()
{
    leds.sequence(LED_SEQUENCE_POWER_ON);
    delay(200);
    // Indicate connecting
    leds.shine(LED_YELLOW);

    // Connect to WiFi
    Serial.begin(9600);
    Serial.println("Connecting to WiFi...");
    if (!wifi.connect(wifiSSID, wifiPassword))
    {
        leds.extinguish(LED_YELLOW);
        leds.shine(LED_RED);
        PANIC("Failed to connect.");
    }
    Serial.println("Connected.");

    leds.sequence(LED_SEQUENCE_CONNECTED);
    delay(200);
}

void loop()
{
    static uint16_t cm = 0;
    static uint32_t lastRunTs = 0;
    
    // Run every interval seconds
    if (millis() - lastRunTs > interval * 1000)
    {
        lastRunTs = millis();
        // Indicate sending
        leds.shine(LED_GREEN);

        // Detect distance
        cm = distance.detect();
        Serial.print("Distance: ");
        Serial.println(cm);

        // Post to server
        char* request = createPostRequest(cm, serverAddr, serverPort);
        if (wifi.sendTCP(request, serverAddr, serverPort))
        {
            leds.sequence(LED_SEQUENCE_SEND_OK);
            Serial.println("Send OK.");
        }
        else
        {
            leds.sequence(LED_SEQUENCE_SEND_FAIL);
            Serial.println("Send failed!");
        }
    }
}
