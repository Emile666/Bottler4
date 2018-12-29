#ifndef BOTTLER4_STM8S105_MAIN_H
#define BOTTLER4_STM8S105_MAIN_H
/*==================================================================
  File Name    : bottler4_stm8s105_main.h
  Author       : Emile
  ------------------------------------------------------------------
  This is the main header file for the bottler-4 project.
  ------------------------------------------------------------------
  BOTTLER4_STM8S105 is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  BOTTLER4_STM8S105 is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with BOTTLER_STM8S105. If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdbool.h>
#include "stdint.h"

//---------------------------------------------------
// YF-S402 water flow sensor
// F = 73 * Q = 4380 pulses per 1000 mL
// An interrupt is generated for a rising AND
// falling edge, but only the falling edge is counted.
//
// FLOW * 1000[mL] * CAL[%] / (4380 * 100[%]) = FLOW * CAL[%] / 438
//---------------------------------------------------
#define FLOW_PER_L     (3500)
#define C2ML           (FLOW_PER_L/10)
#define C2ML_2         (C2ML>>1)

#define SIZE_025L (0)
#define SIZE_030L (1)
#define SIZE_033L (2)
#define SIZE_050L (3)
#define SIZE_075L (4)
#define SIZE_1L   (5)
#define SIZE_1L5  (6)
#define SIZE_2L   (7)
#define SIZE_5L   (8)
#define VOL_MAX   (SIZE_5L)

#define ML250  (FLOW_PER_L / 4)
#define ML300  (FLOW_PER_L * 3 / 10)
#define ML330  (FLOW_PER_L / 3)
#define ML500  (FLOW_PER_L / 2)
#define ML750  (FLOW_PER_L * 3 / 4)
#define ML1000 (FLOW_PER_L)
#define ML1500 (FLOW_PER_L * 3 / 2)
#define ML2000 (FLOW_PER_L * 2)
#define ML5000 (FLOW_PER_L * 5)

#define ML250_SI  (250)
#define ML300_SI  (300)
#define ML330_SI  (330)
#define ML500_SI  (500)
#define ML750_SI  (750)
#define ML1000_SI (1000)
#define ML1500_SI (1500)
#define ML2000_SI (2000)
#define ML5000_SI (5000)

#define VOL_CAL_SI (ML2000_SI)
#define VOL_CAL    (ML2000)

#define STD_LCD_TMR			(50) /* 100 msec. steps */
#define STD_LCD_TMR_UI	   (100) 

// Defines for STD / lcd_task()
#define STD_LCD_DISP1       (0)
#define STD_LCD_UI          (1)
#define STD_LCD_UI_UP       (2)
#define STD_LCD_UI_DOWN     (3)
#define STD_LCD_UI_2        (4)
#define STD_LCD_ENA         (5)
#define STD_LCD_ENA_UP      (6)
#define STD_LCD_ENA_DOWN    (7)
#define STD_LCD_ENA_END     (8)
#define STD_LCD_VOL  		(9)
#define STD_LCD_VOL_UP     (10)
#define STD_LCD_VOL_DOWN   (11)
#define STD_LCD_VOL_END    (12)
#define STD_LCD_VOL_EEP    (13)
#define STD_LCD_ENA_EEP    (14)

// Defines for STD per channel / ctrl_task()
#define STD_DISABLED            (0)
#define STD_CLEANING_STANDBY    (1)
#define STD_CLEANING_ACTIVE     (2)
#define STD_FILLING_STANDBY     (3)
#define STD_FILLING_ACTIVE      (4)
#define STD_CALIBRATION_STANDBY (5)
#define STD_CALIBRATION_ACTIVE  (6)
#define STD_CALIBRATION_END     (7)

// Function prototypes
void     led_task(void);
uint16_t calc_mL(uint16_t net_flow, uint8_t ccal);
void     std(uint8_t x);
void     ctrl_task(void);
void     print_normal(uint8_t v);
void     valves_task(void);
void     lcd_task(void);
void     read_eep_pars(void);

#endif // BOTTLER4_STM8S105_MAIN_H