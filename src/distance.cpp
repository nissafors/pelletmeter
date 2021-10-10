/* Copyright (C) 2021 Andreas Andersson */

#include "distance.h"
#include <Arduino.h>
#include <math.h>

// Public

Distance::Distance(int trigPin, int echoPin) : trigPin(trigPin), echoPin(echoPin)
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

uint16_t Distance::detect()
{
    uint32_t cm = measure();

    return cm;
}

// Private

uint16_t Distance::measure()
{
    // A 10ms pulse triggers the sensor.
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Incoming pulse length is the echo time in us.
    pinMode(echoPin, INPUT);
    uint32_t duration = pulseIn(echoPin, HIGH);

    // Convert to cm and return.
    return (uint16_t)round(((double)duration / 2.0) / 29.1);
}