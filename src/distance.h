/* Copyright (C) 2021 Andreas Andersson */

#ifndef DISTANCE_H
#define DISTANCE_H

// Distance measurements using the HC-SR04 sensor.

#include <stdint.h>

class Distance
{
public:
    int trigPin;
    int echoPin;
    Distance(int trigPin, int echoPin);
    uint16_t detect();
};

#endif // DISTANCE_H