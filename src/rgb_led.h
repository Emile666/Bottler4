#ifndef RGB_LED_H
#define RGB_LED_H
/*==================================================================
  File Name    : rgb_led.h
  Author       : Emile
  ------------------------------------------------------------------
  This is the header file for rgb_led.c.
  ------------------------------------------------------------------
  RGB_LED is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  RGB_LED is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with RGB_LED. If not, see <http://www.gnu.org/licenses/>.
  ==================================================================*/
#include <iostm8s105.h>
#include <stdbool.h>
#include "stdint.h"

// Defines for RGB LED 
#define RED            (0x04)
#define GREEN          (0x02)
#define BLUE           (0x01)
#define WHITE          (RED|GREEN|BLUE)
#define BLACK          (0x00)
#define NO_COLOUR      (0x00)

// Defines for rgb_leds[]
#define RED_BLINKING   (0x20)
#define RED_ON         (0x10)
#define GREEN_BLINKING (0x08)
#define GREEN_ON       (0x04)
#define BLUE_BLINKING  (0x02)
#define BLUE_ON        (0x01)
#define COLOR_OFF	   (0x00)
#define COLOR_ON       (0x01)
#define BLINKING_ON    (0x02)
#define COLOR_BLINKING (0x03)

// Function prototypes
void rgb_led_clr(uint8_t x, uint8_t col);
void rgb_led_set(uint8_t x, uint8_t col);
void one_led_one_colour(uint8_t x, uint8_t col);

#endif // RGB_LED_H