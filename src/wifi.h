/* Copyright (C) 2021 Andreas Andersson */

#ifndef WIFI_H
#define WIFI_H

#include <SoftwareSerial.h>
#include "log.h"

static const uint16_t AT_BUF_SIZE = 256;

/**
 * Connect to WiFi and send TCP message using the AT commands with the ESP8266 module.
 */
class WiFi
{
private:
    SoftwareSerial esp8266;
    Log logger;
    bool connected;
    char atBuf[AT_BUF_SIZE];

    /**
     * Execute an AT command on the ESP8266 module.
     * 
     * @param cmd   The AT command to run.
     * @param rcvPtr    Pointer to a buffer large enough to contain the response to the AT command.
     * @param len       The size of the response buffer,
     * @param timeout   Command timeout in milliseconds.
     * 
     * @return true if command returned OK, false otherwise.
     */
    bool executeAT(const char* cmd, char* rcvPtr, int len, int timeout);

    /**
     * Read a byte from the ESP8266 module.
     * 
     * @param chPtr     Storage pointer.
     * @param timeout   Read timeout in milliseconds.
     * 
     * @return true if a byte could be read, false otherwise.
     */
    bool readByte(char* chPtr, int timeout);

public:
    /**
     * Construct a new WiFi instance.
     * 
     * @param rx        RX pin.
     * @param tx        TX pin.
     * @param verbosity Log level (see the Log class for details).
     */
    WiFi(int rx, int tx, int verbosity);

    /**
     * Connect to WiFi.
     * 
     * @param ssid  The SSID of the WiFi to connect to.
     * @param pwd   The password for the WiFi to connect to.
     * 
     * @return true if connected, false otherwise.
     */
    bool connect(const char* ssid, const char* pwd);

    /**
     * Send a message over a TCP link.
     *
     * @param msg       The data to send.
     * @param address   IP or URL to the host.
     * @param port      The host port.
     * 
     * @treturn true if send was successful, false otherwise.
     */
    bool sendTCP(const char* msg, const char* address, int port);
};

#endif // WIFI_H