#ifndef _STM8_HW_INIT_H
#define _STM8_HW_INIT_H
/*==================================================================
  File Name    : stm8_hw_init.h
  Author       : Emile
  ------------------------------------------------------------------
  This is the header file for stm8_hw_init.c.
  ------------------------------------------------------------------
  STM8_HW_INIT is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  STM8_HW_INIT is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with STM8_HW_INIT. If not, see <http://www.gnu.org/licenses/>.
  ------------------------------------------------------------------
  Schematic of the connections to the MCU.
  
                                 STM8S105C6T6
      MCU pin-name            Function    |    MCU pin-name        Function
   ---------------------------------------|--------------------------------
   01 NRST                                | 13 VDDA
   02 PA1/OSC                 -           | 14 VSSA
   03 PA2/OSCOUT              -           | 15 PB7/AIN7            R4
   04 VSSIO_1                             | 16 PB6/AIN6            G4
   05 VSS                                 | 17 PB5/AIN5[I2C_SDA]   B4
   06 VCAP                                | 18 PB4/AIN4[I2C_SCL]   -
   07 VDD                                 | 19 PB3/AIN3[TIM1_ETR]  -
   08 VDDIO_1                             | 20 PB2/AIN2[TIM1_CH3N] R3
   09 PA3/TIM2_CH3[TIME3_CH1] FLOW1       | 21 PB1/AIN1[TIM1_CH2N] G3
   10 PA4                     FLOW2       | 22 PB0/AIN0[TIM1_CH1N] B3
   11 PA5                     FLOW3       | 23 PE7/AIN8            R2
   12 PA6                     FLOW4       | 24 PE6/AIN9            G2
   ---------------------------------------|--------------------------------
   25 PE5/SPI_NSS             B2          | 37 PE3/TIM1_BKIN       SW1
   26 PC1/TIM1_CH1/UART2_CK   ROTENC_A    | 38 PE2/I2C_SDA         I2C_SDA
   27 PC2/TIM1_CH2            ROTENC_B    | 39 PE1/I2C_SCL         I2C_SCL
   28 PC3/TIM1_CH3            ROTENC_SW   | 40 PE0/CLK_CC0         -
   29 PC4/TIM1_CH4            V1          | 41 PD0/TIM3_CH2...     ISR_TIME
   30 PC5/SPI_SCK             V2          | 42 PD1/SWIM            SWIM
   31 VSSIO_2                             | 43 PD2/TIM3_CH1...     B1 
   32 VDDIO_2                             | 44 PD3/TIM2_CH2...     G1
   33 PC6/SPI_MOSI            V3          | 45 PD4/TIM2_CH1[BEEP]  R1
   34 PC7/SPI_MISO            V4          | 46 PD5/UART2_TX        TX
   35 PG0                     -           | 47 PD6/UART2_RX        RX
   36 PG1                     -           | 48 PD7/TLI[TIM1_CH4]   -
   ---------------------------------------------------------------------
   
   NOTE  : PA1, PA2, PG0 and PG1 do NOT have interrupt capability!
   NOTE 2: PCB v01 has ROTENC_SW connected to PA3
*/
#include <stdbool.h>
#include "stdint.h"

//-----------------------------------------------------------------------
// Hardware defines for register definitions
// These value were defined in IAR, but not in Cosmic STM8
//-----------------------------------------------------------------------
#define CLK_ICKCR_HSIEN   (0x01)
#define CLK_ICKCR_HSIRDY  (0x02)
#define CLK_SWCR_SWBSY    (0x01)
#define CLK_SWCR_SWEN     (0x02)
#define ADC_CR1_SPSEL_MSK (0x70)
#define TIM2_IER_UIE      (0x01)
#define TIM2_CR1_CEN      (0x01)
#define TIM2_SR1_UIF      (0x01)
#define UART2_CR2_TIEN    (0x80) /* Transmitter Interrupt Enable */
#define UART2_CR2_RIEN    (0x20) /* Receiver Interrupt Enable */
#define UART2_CR2_TEN     (0x08) /* Transmitter Enable */
#define UART2_CR2_REN     (0x04) /* Receiver Enable */
#define UART2_SR_TC       (0x40) /* Transmission Complete */
#define UART2_SR_RXNE     (0x20) /* Read data register not empty */
#define UART2_CR3_CLKEN   (0x08) /* Uart clock enable */

#define enable_interrupts()  {_asm("rim\n");} /* enable interrupts */
#define disable_interrupts() {_asm("sim\n");} /* disable interrupts */
#define wait_for_interrupt() {_asm("wfi\n");} /* Wait For Interrupt */

//-----------------------------------------------------------------------
// PORTA Inputs and Outputs
//-----------------------------------------------------------------------
#define FLOW4     (0x40) /* Flowsensor 4 output */
#define FLOW3     (0x20) /* Flowsensor 3 output */
#define FLOW2     (0x10) /* Flowsensor 2 output */
#define FLOW1     (0x08) /* Flowsensor 1 output */
#define PA_NC     (0x87) /* Unused pins */

//-----------------------------------------------------------------------
// PORTB Inputs and Outputs
//-----------------------------------------------------------------------
#define PB_R4     (0x80) /* RGB LED 4, RED   */
#define PB_G4     (0x40) /* RGB LED 4, GREEN */
#define PB_B4     (0x20) /* RGB LED 4, BLUE  */
#define PB_R3     (0x04) /* RGB LED 3, RED   */
#define PB_G3     (0x02) /* RGB LED 3, GREEN */
#define PB_B3     (0x01) /* RGB LED 3, BLUE  */
#define PB_NC     (0x18) /* Unused pins */

//-----------------------------------------------------------------------
// PORTC Inputs and Outputs
//-----------------------------------------------------------------------
#define VALVE4    (0x80) /* Valve 4 relay  */
#define VALVE3    (0x40) /* Valve 4 relay  */
#define VALVE2    (0x20) /* Valve 4 relay  */
#define VALVE1    (0x10) /* Valve 4 relay  */
#define ROTENC_SW (0x08) /* Rotary Encoder switch  */
#define ROTENC_B  (0x04) /* Rotary Encoder B input */
#define ROTENC_A  (0x02) /* Rotary Encoder A input */
#define PC_NC     (0x01) /* Unused pins */

//-----------------------------------------------------------------------
// PORTD Inputs and Outputs
//-----------------------------------------------------------------------
#define RX       (0x40) /* UART2 RXD input */
#define TX       (0x20) /* UART2 TXD output */
#define PD_R1    (0x10) /* RGB LED 1, RED   */
#define PD_G1    (0x08) /* RGB LED 1, GREEN */
#define PD_B1    (0x04) /* RGB LED 1, BLUE */
#define SWIM     (0x02) /* do NOT initialize or debugging will not work */
#define ISR_TIME (0x01) /* Time-measurement Interrupt */
#define PD_NC    (0x80) /* Unused pins */

//-----------------------------------------------------------------------
// PORTE Inputs and Outputs
//-----------------------------------------------------------------------
#define PE_R2    (0x80) /* RGB LED 2, RED   */
#define PE_G2    (0x40) /* RGB LED 2, GREEN */
#define PE_B2    (0x20) /* RGB LED 2, BLUE */
#define PE_SW1   (0x08) /* Pushbutton */
#define I2C_SDA  (0x04)
#define I2C_SCL  (0x02)
#define PE_NC    (0x11)

#define V1       (0)
#define V2       (1)
#define V3       (2)
#define V4       (3)
#define V1b      (1<<V1)
#define V2b      (1<<V2)
#define V3b      (1<<V3)
#define V4b      (1<<V4)

#define MODE_OFF (0)
#define MODE_ON  (1)
#define MODE_CAL (2)
#define MODE_CLN (3)

// Function prototypes
uint16_t get_timer1_value(void);
void     initialise_system_clock(void);
void     setup_timers(void);
void     setup_gpio_ports(void);

#endif // _STM8_HW_INIT_H