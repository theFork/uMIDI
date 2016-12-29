/// \file
/// \brief  Commmon defines and data structures for Adafruit LED displays

/*
 * Copyright 2016 Sebastian Neuser
 *
 * This file is part of the uMIDI firmware.
 *
 * the uMIDI firmware is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The uMIDI firmware is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the uMIDI firmware.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _ADAFRUIT_DISPLAY_H
#define _ADAFRUIT_DISPLAY_H


//---------------- includes ----------------//

#include <stdint.h>


//---------------- constants ----------------//

/// \brief      Computes an Adafruit display's I2C address
/// \param      lsbs
///                 the three LSBs of the address
/// \returns    a full (7-bit, right-aligned) I2C address
#define ADAFRUIT_DISPLAY_ADDRESS(lsbs)     (0x70 + lsbs)


//---------------- data types ----------------//

/// \brief      Brightness of the LEDs
enum adafruit_display_brightness
{
    ADAFRUIT_DISPLAY_BRIGHTNESS_01_OF_16,   ///< 1/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_02_OF_16,   ///< 2/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_03_OF_16,   ///< 3/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_04_OF_16,   ///< 4/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_05_OF_16,   ///< 5/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_06_OF_16,   ///< 6/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_07_OF_16,   ///< 7/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_08_OF_16,   ///< 8/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_09_OF_16,   ///< 9/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_10_OF_16,   ///< 10/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_11_OF_16,   ///< 11/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_12_OF_16,   ///< 12/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_13_OF_16,   ///< 13/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_14_OF_16,   ///< 14/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_15_OF_16,   ///< 15/16th of maximum brightness
    ADAFRUIT_DISPLAY_BRIGHTNESS_16_OF_16    ///< maximum brightness
};

/// \brief      Possible LED colors
enum adafruit_display_color
{
    ADAFRUIT_DISPLAY_COLOR_BLACK,       ///< Black (LED off)
    ADAFRUIT_DISPLAY_COLOR_GREEN,       ///< Green
    ADAFRUIT_DISPLAY_COLOR_ORANGE,      ///< Orange
    ADAFRUIT_DISPLAY_COLOR_RED,         ///< Red
};

/// \brief      Blinking mode of the LEDs
enum adafruit_display_mode
{
    ADAFRUIT_DISPLAY_MODE_STATIC,       ///< LEDs are turned on statically
    ADAFRUIT_DISPLAY_MODE_BLINK_2HZ,    ///< LEDs blink with a frequency of 2 Hz
    ADAFRUIT_DISPLAY_MODE_BLINK_1HZ,    ///< LEDs blink with a frequency of 1 Hz
    ADAFRUIT_DISPLAY_MODE_BLINK_05HZ    ///< LEDs blink with a frequency of 0.5 Hz
};

/// \brief  Configuration of the bar graph
struct adafruit_display_config
{
    uint8_t                          address;       ///< I2C bus address of the device
    enum adafruit_display_brightness brightness;    ///< Brightness of the LEDs
    enum adafruit_display_mode       mode;          ///< Blink mode
};


//---------------- functions and procedures ----------------//


//---------------- EOF ----------------//
#endif // _ADAFRUIT_DISPLAY_H
