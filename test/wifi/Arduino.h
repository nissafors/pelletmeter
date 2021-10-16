/* Copyright (C) 2021 Andreas Andersson */

// Fake Arduino.h for wifi test

#ifndef ARDUINO_H 
#define ARDUINO_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <cstddef>

unsigned long millis();
void delay(unsigned long ms);

#endif // ARDUINO_H