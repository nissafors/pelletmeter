/* Copyright (C) 2021 Andreas Andersson */

#ifndef DISTANCE_H
#define DISTANCE_H

#include <stdint.h>

/**
 * Distance measurements using the HC-SR04 sensor.
 */ 
class Distance
{
private:
    int trigPin;
    int echoPin;

public:
    /**
     * Construct a new Distance instance.
     * 
     * @param trigPin   Number of pin connected to HC-SR04 trig.
     * @param echoPin   number of pin connected to HC-SR04 echo.
     */
    Distance(int trigPin, int echoPin);

    /**
     * Detect distance.
     * 
     * @return The distance to the nearest object in cm.
     */
    uint16_t detect();
};

#endif // DISTANCE_H