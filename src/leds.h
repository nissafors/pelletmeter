/* Copyright (C) 2021 Andreas Andersson */

#ifndef LEDS_H
#define LEDS_H

#include <Arduino.h>

/**
 * Available LEDs.
 */
typedef enum
{
    LED_GREEN,  /**< Green LED */
    LED_YELLOW, /**< Yellow LED */
    LED_RED,    /**< Red LED */
    N_LEDS      /**< Total number of LEDs */
} led_t;

/**
 * LED sequences.
 */
typedef enum
{
    LED_SEQUENCE_POWER_ON,  /**< LED sequence to run at power on. */
    LED_SEQUENCE_CONNECTED, /**< LED sequence to run when connected to WiFi. */
    LED_SEQUENCE_SEND_OK,   /**< LED sequence to run when send is successful. */
    LED_SEQUENCE_SEND_FAIL, /**< LED sequence to run when send fails. */
    N_LED_SEQUENCES         /**< Total number of LED sequences. */
} sequence_t;

/**
 * LED driver.
 */
class LEDs
{
private:
    uint8_t pins[N_LEDS];

    /**
     * Turn off all LEDs.
     */
    void extinguishAll();

    /**
     * Blink one or more LEDs.
     * 
     * @param leds  An array of led_t elements to blink.
     * @param nLeds Number of elements in the leds array.
     */
    void blink(const led_t* leds, const uint8_t nLeds);

public:
    /**
     * Construct a new LEDs instance.
     * 
     * @param pins  An array of pins to assign to each of the LEDs enumerated in led_t.
     */
    LEDs(const uint8_t* pins);

    /**
     * Turn on an LED.
     * 
     * @param led   The LED to turn on.
     */
    void shine(const led_t led);

    /**
     * Turn off an LED.
     * 
     * @param led   The LED to turn off.
     */
    void extinguish(const led_t led);

    /**
     * Run a sequence of LED blinks.
     * 
     * @param sequence  The sequence to run.
     */
    void sequence(const sequence_t sequence);
};

#endif // LEDS_H