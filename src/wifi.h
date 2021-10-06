/* Copyright (C) 2021 Andreas Andersson */

#ifndef WIFI_H
#define WIFI_H

#include <SoftwareSerial.h>

static const uint8_t CMD_BUF_SIZE = 64;

class WiFi
{
private:
    SoftwareSerial esp8266;
    bool connected;
    char cmdBuf[CMD_BUF_SIZE];
    bool sendAT(const char* cmd, int timeout);
public:
    WiFi(int rx, int tx);
    bool connect(const char* name, const char* pwd);
};

#endif // WIFI_H