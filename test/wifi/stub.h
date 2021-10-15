/* Copyright (C) 2021 Andreas Andersson */

#ifndef STUB_H
#define STUB_H

#include <stdint.h>
#include <string>

void stubInit();
void stubDeinit();

int stubGetLogVerbosity();
uint8_t stubGetSSReceivePin();
uint8_t stubGetSSTransmitPin();
void stubAppendAtCommand(std::string cmd, std::string response);

#endif // STUB_H