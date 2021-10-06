/* Copyright (C) 2021 Andreas Andersson */

#include <avr/sleep.h>
#include <avr/wdt.h>
#include <Arduino.h>

void panic(const char* msg, const char* file, int line)
{
  Serial.print("PANIC! ");
  Serial.println(msg);
  Serial.print("File: ");
  Serial.print(file);
  Serial.print(", Line: ");
  Serial.println(line);
  delay(100);
  wdt_disable();
  set_sleep_mode( SLEEP_MODE_PWR_DOWN );

  while ( true )
  {
    sleep_enable();
    cli();
    sleep_cpu();
  }
}

