/*==================================================================
  File Name    : rgb_led.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the functions for the RGB LEDs.
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
  ==================================================================
*/ 
#include "rgb_led.h"
#include "stm8_hw_init.h"

// X0 = LED off, 01 = LED on, 11 = LED blinking
uint8_t rgb_leds[4]     = {0,0,0,0}; // RED = Bit 54, GREEN = Bit 32, BLUE = Bit 10
uint8_t old_rgb_leds[4] = {0,0,0,0};

/*------------------------------------------------------------------
  Purpose  : This function clears one RGB LED color 
  Variables: 
		  x: [0,1,2,3], the number of the RGB LED
		col: [RED,GREEN,BLUE]
  Returns  : -
  ------------------------------------------------------------------*/
void rgb_led_clr(uint8_t x, uint8_t col)
{
	switch (x)
	{
		case 0:
		    switch (col)
            {   // RGB LED 1
            case RED:       PD_ODR |= PD_R1; break;
                case GREEN: PD_ODR |= PD_G1; break;
                case BLUE:  PD_ODR |= PD_B1; break;
                default:    PD_ODR |= (PD_R1 | PD_G1 | PD_B1); break; // white
            } // switch
		    break;
		case 1:
            switch (col)
            {   // RGB LED 2
                case RED:   PE_ODR |= PE_R2; break;
                case GREEN: PE_ODR |= PE_G2; break;
                case BLUE:  PE_ODR |= PE_B2; break;
                default:    PE_ODR |= (PE_R2 | PE_G2 | PE_B2); break;
            } // switch
			break;
		case 2:
            switch (col)
            {   // RGB LED 3
                case RED:   PB_ODR |= PB_R3; break;
                case GREEN: PB_ODR |= PB_G3; break;
                case BLUE:  PB_ODR |= PB_B3; break;
                default:    PB_ODR |= (PB_R3 | PB_G3 | PB_B3); break;
            } // switch
			break;
		case 3:
            switch (col)
            {   // RGB LED 4
                case RED:   PB_ODR |= PB_R4; break;
                case GREEN: PB_ODR |= PB_G4; break;
                case BLUE:  PB_ODR |= PB_B4; break;
                default:    PB_ODR |= (PB_R4 | PB_G4 | PB_B4); break;
            } // switch
			break;
		default:
			break;
	} // switch
} // rgb_led_clr()

/*------------------------------------------------------------------
  Purpose  : This function sets one RGB LED color 
  Variables:
	      x: [0,1,2,3], the number of the RGB LED
	    col: [RED,GREEN,BLUE,WHITE,BLACK]
  Returns  : -
  ------------------------------------------------------------------*/
void rgb_led_set(uint8_t x, uint8_t col)
{
	switch (x)
	{
		case 0:
		    switch (col)
            {   // RGB LED 1
                case RED:   PD_ODR &= ~PD_R1; break;
                case GREEN: PD_ODR &= ~PD_G1; break;
                case BLUE:  PD_ODR &= ~PD_B1; break;
                default:    PD_ODR &= ~(PD_R1 | PD_G1 | PD_B1); break; // white
            } // switch
		    break;
		case 1:
            switch (col)
            {   // RGB LED 2
                case RED:   PE_ODR &= ~PE_R2; break;
                case GREEN: PE_ODR &= ~PE_G2; break;
                case BLUE:  PE_ODR &= ~PE_B2; break;
                default:    PE_ODR &= ~(PE_R2 | PE_G2 | PE_B2); break;
            } // switch
			break;
		case 2:
            switch (col)
            {   // RGB LED 3
                case RED:   PB_ODR &= ~PB_R3; break;
                case GREEN: PB_ODR &= ~PB_G3; break;
                case BLUE:  PB_ODR &= ~PB_B3; break;
                default:    PB_ODR &= ~(PB_R3 | PB_G3 | PB_B3); break;
            } // switch
			break;
		case 3:
            switch (col)
            {   // RGB LED 4
                case RED:   PB_ODR &= ~PB_R4; break;
                case GREEN: PB_ODR &= ~PB_G4; break;
                case BLUE:  PB_ODR &= ~PB_B4; break;
                default:    PB_ODR &= ~(PB_R4 | PB_G4 | PB_B4); break;
            } // switch
			break;
		default:
			break;
	} // switch
} // rgb_led_set()

/*--------------------------------------------------------------------
  Purpose  : This functions sets one of the colors of an RGB LED
             on, off or blinking. the global variable rgb_leds[] is 
			 used and has the following bit positions:
				 bit 5: 1 = RED blinking
				 bit 4: 1 = RED ON, 0 = RED OFF
				 bit 3: 1 = GREEN blinking
				 bit 2: 1 = GREEN ON, 0 = GREEN OFF
				 bit 1: 1 = BLUE blinking
				 bit 0: 1 = BLUE ON, 0 = BLUE OFF			 
		  x: [0,1,2,3], the number of the RGB LED
		col: [RED,GREEN,BLUE]  
  Returns  : -
  --------------------------------------------------------------------*/
void one_led_one_colour(uint8_t x, uint8_t col)
{
	uint8_t mask, shr;
	
	switch (col)
	{
		case RED  : mask = 0x30; shr = 4; break;
		case GREEN: mask = 0x0C; shr = 2; break;
		case BLUE : mask = 0x03; shr = 0; break;
		default   : mask = 0x00; shr = 0; break;
	} // switch
	if ((mask == 0x00) || (x > 3)) return;
	
	switch ((rgb_leds[x] & mask) >> shr)
	{
		case COLOR_OFF:      /* fall through */
		case BLINKING_ON:    rgb_led_clr(x,col);
						     break;
		case COLOR_ON:       rgb_led_set(x,col);
					         break;
		case COLOR_BLINKING: if (old_rgb_leds[x] & mask)
							 {	
								 rgb_led_clr(x,col);
								 old_rgb_leds[x] &= ~mask;
							 }				
							 else 
							 {
								rgb_led_set(x,col);
								old_rgb_leds[x] |= mask;
							 } // else
							 break;
	} // switch	
} // one_led_one_colour()