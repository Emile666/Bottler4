/*==================================================================
  File Name    : i2c.h
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the I2C related functions for the STM8 uC.
  ------------------------------------------------------------------
  I2C is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  I2C is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with I2C. If not, see <http://www.gnu.org/licenses/>.
  ==================================================================
*/
#ifndef _I2C_H
#define _I2C_H   1

#include <iostm8s105.h>
#include <stdbool.h>
#include "stdint.h"

// Defines for the I2C_CR2 register
#define I2C_CR2_SWRST (0x80)
#define I2C_CR2_POS   (0x08)
#define I2C_CR2_ACK   (0x04)
#define I2C_CR2_STOP  (0x02)
#define I2C_CR2_START (0x01)

// Defines for the I2C_SR1 register when in Master Mode
#define I2C_SR1_TXE  (0x80)
#define I2C_SR1_RXNE (0x40)
#define I2C_SR1_BTF  (0x04)
#define I2C_SR1_ADDR (0x02)
#define I2C_SR1_SB   (0x01)

#define I2C_SR2_AF   (0x04) /* ACK Failure */

#define I2C_SR3_BUSY (0x02) /* Bus Busy */
#define I2C_SR3_MSL  (0x01) /* Master/Slave */

/* defines the data direction (reading from I2C device) in i2c_start(), i2c_rep_start() */
#define I2C_ACK     (0)
#define I2C_NACK    (1)
#define I2C_WRITE   (0)
#define I2C_READ    (1)
#define I2C_RETRIES (3)

//-----------------------------------------------------------------
// The LM92 sign bit is normally bit 12. The value read from the
// LM92 is SHL3. Therefore the sign bit is at bit 15
// Same for the Full Scale value, normally 2^12 SHL3 = 2^15.
//-----------------------------------------------------------------
#define LM92_ADDR      (0x90)
#define LM92_SIGNb     (0x8000)
#define LM92_FS        (32768)
#define LM92_ERR       (0x4000)

//-------------------------------------------------------------------------
// MCP23017 16-BIT IO Expander: Register names when BANK == 1
//-------------------------------------------------------------------------
// No bank addressing, seq. operation disabled, slew rate enabled
// HW addressing enabled
#define MCP23017_ADDR (0x40)
#define MCP23017_INIT (0x00)

// Defines for the MCP23017 with IOCON.BANK = 0
#define IODIRA   (0x00)
#define IODIRB   (0x01)
#define IPOLA    (0x02)
#define IPOLB    (0x03)
#define GPINTENA (0x04)
#define GPINTENB (0x05)
#define DEFVALA  (0x06)
#define DEFVALB  (0x07)
#define INTCONA  (0x08)
#define INTCONB  (0x09)
#define IOCON    (0x0A)
#define GPPUA    (0x0C)
#define GPPUB    (0x0D)
#define INTFA    (0x0E)
#define INTFB    (0x0F)
#define INTCAPA  (0x10)
#define INTCAPB  (0x11)
#define GPIOA    (0x12)
#define GPIOB    (0x13)
#define OLATA    (0x14)
#define OLATB    (0x15)

// DS2482 Configuration Register
// Standard speed (1WS==0), Strong Pullup disabled (SPU==0), Active Pullup enabled (APU==1)
#define DS2482_ADDR          (0x30)
#define DS2482_CONFIG        (0xE1)
#define DS2482_OW_POLL_LIMIT  (200)

// DS2482 commands
#define CMD_DRST   (0xF0)
#define CMD_WCFG   (0xD2)
#define CMD_CHSL   (0xC3)
#define CMD_SRP    (0xE1)
#define CMD_1WRS   (0xB4)
#define CMD_1WWB   (0xA5)
#define CMD_1WRB   (0x96)
#define CMD_1WSB   (0x87)
#define CMD_1WT    (0x78)

// DS2482 status bits 
#define STATUS_1WB  (0x01)
#define STATUS_PPD  (0x02)
#define STATUS_SD   (0x04)
#define STATUS_LL   (0x08)
#define STATUS_RST  (0x10)
#define STATUS_SBR  (0x20)
#define STATUS_TSB  (0x40)
#define STATUS_DIR  (0x80)

void    i2c_init(bool bb);           // Initializes the I2C Interface. Needs to be called only once
uint8_t i2c_start(uint8_t addr);     // Issues a start condition and sends address and transfer direction
uint8_t i2c_rep_start(uint8_t addr); // Issues a repeated start condition and sends address and transfer direction
void    i2c_stop(void);              // Terminates the data transfer and releases the I2C bus
void    i2c_write(uint8_t data);     // Send one byte to I2C device
uint8_t i2c_read(uint8_t ack);       // Read one byte from I2C device
void    i2c_readN(uint8_t num_bytes, uint8_t *buf); // read byte(s) from the I2C device
uint8_t i2c_read1(void);             // Lujji version of i2c_read()
void    i2c_read_arr(uint8_t *buf, int len); // Lujji version of i2c_readN()

int16_t lm92_read(uint8_t addr, uint8_t *err);

uint8_t mcp23008_init(void);
uint8_t mcp23017_init(void);
uint8_t mcp23017_read(uint8_t reg);
uint8_t mcp23017_write(uint8_t reg, uint8_t data);

int8_t  ds2482_reset(uint8_t addr);
int8_t  ds2482_reset_bb(uint8_t addr);
int8_t  ds2482_write_config(uint8_t addr);
int8_t  ds2482_detect(uint8_t addr);
int8_t  ds2482_detect_bb(uint8_t addr);
uint8_t ds2482_search_triplet(uint8_t search_direction, uint8_t addr);

#endif
