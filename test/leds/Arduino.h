/* Copyright (C) 2021 Andreas Andersson */

// Fake Arduino.h for distance test
#ifndef ARDUINO_H 
#define ARDUINO_H

#include <stdint.h>

#define LOW 0
#define HIGH 1
#define OUTPUT 2

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
void delay(unsigned long ms);

#endif // ARDUINO_H