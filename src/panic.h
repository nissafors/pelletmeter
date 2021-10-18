/* Copyright (C) 2021 Andreas Andersson */

#ifndef PANIC_H
#define PANIC_H

/**
 * Macro to invoke panic.
 * 
 * Example:
 *    PANIC("A non-recoverable error occured!");
 * 
 * @param msg   Panic message to print to serial.
 */
#define PANIC(msg) panic(msg, __FILE__, __LINE__)

/**
 * Print panic message and sleep processor.
 * 
 * DO NOT RUN THIS COMMAND! Use the macro above instead.
 * 
 * @param msg   Panic message to print to serial.
 * @param file  The file in which the panic occured.
 * @param line  The line on which the panic occured.
 */
void panic(const char* msg, const char* file, int line);

#endif // PANIC_H