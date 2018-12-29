/*==================================================================
  File Name    : eep.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This is the header-file for eep.c
  ------------------------------------------------------------------
  EEP is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  EEP is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with EEP.  If not, see <http://www.gnu.org/licenses/>.
  ==================================================================
*/ 
#ifndef _EEP_H
#define _EEP_H

#include "stdint.h"

// EEPROM base address within STM8 uC
#define EEP_BASE_ADDR (0x4000)
#define DUL           (0x08)

#define EEP_MODE1 (0)
#define EEP_MODE2 (1)
#define EEP_MODE3 (2)
#define EEP_MODE4 (3)
#define EEP_VOL1  (4)
#define EEP_VOL2  (5)
#define EEP_VOL3  (6)
#define EEP_VOL4  (7)
#define EEP_CAL1  (8)
#define EEP_CAL2  (9)
#define EEP_CAL3 (10)
#define EEP_CAL4 (11)

// Function prototypes
uint16_t eeprom_read_config(uint8_t eeprom_address);
void     eeprom_write_config(uint8_t eeprom_address,uint16_t data);

#endif
