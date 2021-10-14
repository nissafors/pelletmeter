/* Copyright (C) 2021 Andreas Andersson */

// Fake for wifi test.

#ifndef SOFTWARESERIAL_H
#define SOFTWARESERIAL_H

#include "Arduino.h"

class SoftwareSerial
{
public:
    SoftwareSerial(uint8_t receivePin, uint8_t transmitPin);
    void begin(long speed);
    size_t write(const char *str);
    int read();
    int available();
    size_t print(const char *str);
    size_t println(const char *str  );
    void flush();
};

#endif // SOFTWARESERIAL_H