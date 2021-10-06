/* Copyright (C) 2021 Andreas Andersson */

#ifndef PANIC_H
#define PANIC_H

#define PANIC(msg) panic(msg, __FILE__, __LINE__)

void panic(const char* msg, const char* file, int line);

#endif // PANIC_H