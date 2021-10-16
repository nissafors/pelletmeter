/* Copyright (C) 2021 Andreas Andersson */

#ifndef WIFI_H
#define WIFI_H

#include <SoftwareSerial.h>
#include "log.h"

static const uint8_t CMD_BUF_SIZE = 64;
static const uint16_t RCV_BUF_SIZE = 256;

class WiFi
{
private:
    SoftwareSerial esp8266;
    Log logger;
    bool connected;
    char cmdBuf[CMD_BUF_SIZE];
    char rcvBuf[RCV_BUF_SIZE];
    bool executeAT(const char* cmd, char* rcvPtr, int len, int timeout);
    bool readByte(char* chPtr, int timeout);
public:
    WiFi(int rx, int tx, int verbosity);
    bool connect(const char* ssid, const char* pwd);
    bool getIP(char* ipPtr);
    bool sendTCP(const char* msg, const char* address, int port);
};

#endif // WIFI_H