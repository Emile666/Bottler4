/*==================================================================
  File Name    : comms.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains functions for the uart commands.
  ------------------------------------------------------------------
  COMMS is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
 
  COMMS is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with COMMS. If not, see <http://www.gnu.org/licenses/>.
  ==================================================================
*/ 
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart.h"
#include "comms.h"
#include "i2c.h"
#include "scheduler.h"
#include "lcd_i2c.h"
#include "eep.h"

extern uint8_t valve_on;
extern uint8_t rgb_leds[]; // RED = Bit 54, GREEN = Bit 32, BLUE = Bit 10
extern uint8_t cal[];      // Calibration constants

uint8_t rs232_inbuf[UART_BUFLEN]; // buffer for RS232 commands
uint8_t rs232_ptr = 0;            // index in RS232 buffer

/*-----------------------------------------------------------------------------
  Purpose  : This function prints the version number info to both the UART and
             the LCD I2C display.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void print_version_number(void)
{
    const char s[] = "Bottler-4 V0.3.2912";
    xputs(s);               // print to uart
    uart_write('\n');       // linefeed
    uart_write('\r');       // carriage return
    lcd_i2c_clear();        // ckear LCD I2C display
    lcd_i2c_setCursor(0,0); // cursor home
    lcd_i2c_backlight_on(); // turn backlight on
    lcd_i2c_print(s);       // print version nr string
} // print_version_number()

/*-----------------------------------------------------------------------------
  Purpose  : Scan all devices on the I2C bus
  Variables: -
 Returns  : -
  ---------------------------------------------------------------------------*/
void i2c_scan(void)
{
	char    s[50]; // needed for printing to serial terminal
	uint8_t x = 0;
	int     i;     // Leave this as an int!
	
	xputs("I2C: ");
    for (i = 0x00; i < 0xff; i+=2)
	{
		if (i2c_start(i) == I2C_ACK)
		{
			sprintf(s,"0x%0x ",i);
		    xputs(s);
			x++;
		} // if
		i2c_stop();
	} // for
	if (!x) xputs("-");
	xputs("\n");
} // i2c_scan()

/*-----------------------------------------------------------------------------
  Purpose  : Non-blocking RS232 command-handler via the UART
  Variables: -
  Returns  : [NO_ERR, ERR_CMD, ERR_NUM, ERR_I2C]
  ---------------------------------------------------------------------------*/
uint8_t rs232_command_handler(void)
{
  uint8_t ch;
  static bool cmd_rcvd = 0;
  
  if (!cmd_rcvd && uart_kbhit())
  { // A new character has been received
    ch = tolower(uart_read()); // get character as lowercase
    uart_write(ch);
	switch (ch)
	{
		case '\r': break;
		case '\n': cmd_rcvd  = true;
		           rs232_inbuf[rs232_ptr] = '\0';
		           rs232_ptr = false;
				   break;
		default  : rs232_inbuf[rs232_ptr++] = ch;
				   break;
	} // switch
  } // if
  if (cmd_rcvd)
  {
	  cmd_rcvd = false;
	  return execute_single_command(rs232_inbuf);
  } // if
  else return NO_ERR;
} // rs232_command_handler()

/*-----------------------------------------------------------------------------
  Purpose: interpret commands which are received via the UART:
   - S0           : Chess version number
	 S1           : List all connected I2C devices  
	 S2           : List all tasks
 
  Variables: 
          s: the string that contains the command from UART
  Returns  : [NO_ERR, ERR_CMD, ERR_NUM, ERR_I2C] or ack. value for command
  ---------------------------------------------------------------------------*/
uint8_t execute_single_command(char *s)
{
   uint8_t  num  = atoi(&s[1]); // convert number in command (until space is found)
   uint8_t  rval = NO_ERR, err, i, num2;
   uint16_t temp;
   char     s2[30]; // Used for printing to RS232 port
   
   switch (s[0])
   {
	   case 'c': // calibration constants
                 if (num && ((s[2] != ' ') || (strlen(s) < 3)))
				 {  // check for error in command: 'cx y'
					rval = ERR_CMD; 
				 } // if				 
	             else if (num > 4)
				 {
					 rval = ERR_NUM;
				 } // else if
				 else if (!num)
                 {
                    sprintf(s2,"CAL=%d,%d,%d,%d\n\r",(uint16_t)cal[0],
                           (uint16_t)cal[1],(uint16_t)cal[2],(uint16_t)cal[3]);
                    xputs(s2);
                 }
				 else
                 {
					num2 = atoi(&s[3]);    // convert to number
                    cal[num-1] = num2;
                    eeprom_write_config(EEP_CAL1+num-1,num2);
                    sprintf(s2,"CAL[V%d]=%d\n\r",(uint16_t)num,(uint16_t)num2);
                    xputs(s2); // print to uart
                 } // else
                 break;
	   case 'l': // RGB LED control
                 if ((s[2] != ' ') || (strlen(s) < 3))
				 {  // check for error in command: 'lx y'
					rval = ERR_CMD; 
				 } // if				 
	             else if (!num || (num > 4))
				 {
					 rval = ERR_NUM;
				 } // else if
				 else
				 {
					num2 = atoi(&s[3]);   // convert to number
                    rgb_leds[num-1] = num2;
                    sprintf(s2,"num2=%d\n\r",(uint16_t)num2);
                    xputs(s2);
                 } // else
                 break;
       case 's': // System commands
				 switch (num)
				 {
					 case 0: // Chess revision number
							 print_version_number();
							 break;
					 case 1: // List all I2C devices
					         i2c_scan();
							 break;
					 case 2: // List all tasks
							 list_all_tasks(); 
							 break;	
					 default: rval = ERR_NUM;
							  break;
				 } // switch
				 break;
	   case 'v': // valve control
                 if ((s[2] != ' ') || (strlen(s) < 3))
				 {  // check for error in command: 'vx y'
					rval = ERR_CMD; 
				 } // if				 
	             else if (!num || (num > 4))
				 {
					 rval = ERR_NUM;
				 } // else if
				 else
				 {
                    i    = 0x01 << (num-1); // bit-position for valve
                    i   |= (i << 4);        // Upper 4 bits for Fix-position
					num2 = atoi(&s[3]);     // convert to number
                    if (num2)
                         valve_on |=  i;
                    else valve_on &= ~i;
                 } // else
                 break;
	   default: rval = ERR_CMD;
				sprintf(s2,"ERR.CMD[%s]\n\r",s);
				xputs(s2);
	            break;
   } // switch
   return rval;	
} // execute_single_command()
