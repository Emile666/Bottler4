/*==================================================================
  File Name    : bottler4_stm8s105_main.c
  Author       : Emile
  ------------------------------------------------------------------
  Purpose : This files contains the main() function and all the 
            hardware related functions for the STM8S105C6T6 uC.
            It is meant for the bottler-4 project.
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
  along with BOTTLER4_STM8S105. If not, see <http://www.gnu.org/licenses/>.
  ==================================================================
*/ 
#include <stdio.h>
#include "bottler4_stm8s105_main.h"
#include "stm8_hw_init.h"
#include "scheduler.h"
#include "eep.h"
#include "i2c.h"
#include "uart.h"
#include "comms.h"
#include "lcd_i2c.h"
#include "delay.h"
#include "rgb_led.h"

// X0 = LED off, 01 = LED on, 11 = LED blinking
extern uint8_t  rgb_leds[];   // RED = Bit 54, GREEN = Bit 32, BLUE = Bit 10
extern uint32_t t2_millis;    // needed for delay_msec()
extern uint8_t  rs232_inbuf[];

// Eeprom parameters
uint8_t  cal[4] = {100,100,100,100};
int8_t   vol_2_bottle[]         = {SIZE_033L, SIZE_033L, SIZE_033L, SIZE_033L};
const uint16_t ml_si[VOL_MAX+1] = {ML250_SI , ML300_SI , ML330_SI , ML500_SI , ML750_SI,
                                   ML1000_SI, ML1500_SI, ML2000_SI, ML5000_SI};
const uint16_t ml[VOL_MAX+1]    = {ML250 , ML300 , ML330 , ML500 , ML750,
                                   ML1000, ML1500, ML2000, ML5000};

@eeprom int16_t eedata[] =
{
   MODE_ON  , MODE_ON  , MODE_ON  , MODE_ON  , /* 4 x MODE */
   SIZE_050L, SIZE_050L, SIZE_050L, SIZE_050L, /* 4 x SIZE_ML */
   100      , 100      , 100      , 100        /* 4 x CAL */
}; // eedata[]

bool     rotenc_sw = false;
uint8_t  valve_on = 0x00; // V1..V4 bits, directly controls relay
uint16_t flow[4] = {0,0,0,0};
uint16_t old_flow[4];
uint8_t  stdi[4]       = {STD_DISABLED,STD_DISABLED,STD_DISABLED,STD_DISABLED};
uint16_t filled[4]     = {0,0,0,0}; // Value for mL filled into a bottle
uint16_t vbottle_start[4];
uint8_t  keypressed[4] = {false,false,false,false};
int8_t   mode[4]       = {MODE_ON,MODE_ON,MODE_ON,MODE_ON};

/*--------------------------------------------------------------------
  Purpose  : This is the task that controls the RGB LEDs. 
             It is called every 500 msec.
  Returns  : -
  --------------------------------------------------------------------*/
void led_task(void)
{
	uint8_t i;
	
	for (i = 0; i < 4; i++)
	{
		one_led_one_colour(i,RED);   // Set color with or without blinking
		one_led_one_colour(i,GREEN); // Set color with or without blinking
		one_led_one_colour(i,BLUE);  // Set color with or without blinking
	} // for i
} // led_task()

/*--------------------------------------------------------------------
  Purpose  : This function calculates the actual amount of milli-litres
             for a bottle-line. It uses the global variable cal[].
             It is called by std()
  Variables: net_flow: the actual pulses from the flow-sensor
             ccal    : the calibration constant (default: 100 [%])
  Returns  : the number of milli-litres
  --------------------------------------------------------------------*/
uint16_t calc_mL(uint16_t net_flow, uint8_t ccal)
{
	uint32_t temp;
    
    temp  = (uint32_t)net_flow;
    temp *= ccal;   // mpy with calibration constant
    temp += C2ML_2; // add rounding
    temp /= C2ML;   // divide to get mL
    return (uint16_t)temp;
} // calc_mL()

/*------------------------------------------------------------------
  Purpose  : This function is the State Transition Diagram for every
             bottle-line. It is called every 100 msec. and uses the 
             following global variables:
			 mode[]     : input from lcd_task()
			 
			 valve_on[] : output for valves_task()
			 rgb_leds[] : output for led_task()
  Variables:
	      x: [V1,V2,V3,V4], the number of the bottle-line
  Returns  : -
  ------------------------------------------------------------------*/
void std(uint8_t x)
{
	uint16_t temp;
    uint8_t  mask = (V1b << x);
    char     s[20];
    
	switch (stdi[x])
	{
		case STD_DISABLED:
			 rgb_leds[x] = NO_COLOUR;
			 if (!(valve_on & (mask << 4))) // Valve command via UART?
                valve_on   &= ~mask; // disable valve 
			 switch (mode[x])
             {
                case MODE_ON : vbottle_start[x] = flow[x]; // remember start volume
				               stdi[x] = STD_FILLING_STANDBY;
                               break;
                case MODE_CLN: stdi[x] = STD_CLEANING_STANDBY;
                               break;
                case MODE_CAL: vbottle_start[x] = flow[x]; // remember start volume
                               stdi[x] = STD_CALIBRATION_STANDBY;
                               break;
			    // else remain in this state
             } // switch
			 break;
             
		case STD_CLEANING_STANDBY:
			 rgb_leds[x] = BLUE_ON;
			 if (mode[x] != MODE_CLN) stdi[x] = STD_DISABLED;
			 else if (keypressed[x])
			 {
				keypressed[x] = false; // reset key indicator
				stdi[x]       = STD_CLEANING_ACTIVE;
			 } // else
			 break;
             
		case STD_CLEANING_ACTIVE:
			 rgb_leds[x] = BLUE_ON | BLUE_BLINKING;
			 if (!(valve_on & (mask << 4))) // Valve command via UART?
                valve_on |= mask; // enable valve
			 if (mode[x] != MODE_CLN) stdi[x] = STD_DISABLED;
			 else if (keypressed[x])
			 {
				keypressed[x] = false; // reset key indicator
				stdi[x]       = STD_FILLING_STANDBY;
			 } // else
			 break;
             
		case STD_FILLING_STANDBY:
			 rgb_leds[x] = GREEN_ON;
		     if (!(valve_on & (mask << 4))) // Valve command via UART?
                valve_on &= ~mask; // disable valve;
			 if (mode[x] != MODE_ON) stdi[x] = STD_DISABLED;
			 else if (keypressed[x])
			 {
				keypressed[x] = false; // reset key indicator
				stdi[x]       = STD_FILLING_ACTIVE;
			 } // else
			 break;
             
		case STD_FILLING_ACTIVE:
			 rgb_leds[x]  = GREEN_ON | GREEN_BLINKING;
             if (!(valve_on & (mask << 4))) // Valve command via UART?
                valve_on    |= mask; // enable valve
             temp = flow[x] - vbottle_start[x];
			 filled[x] = calc_mL(temp,cal[x]);  // calculate milli-litres
             if (keypressed[x]) keypressed[x] = false; // no double keypresses allowed
			 if (mode[x] != MODE_ON) stdi[x] = STD_DISABLED;
			 else if (filled[x] > ml_si[vol_2_bottle[x]])
			 { 
				vbottle_start[x] = flow[x]; // remember start volume for next bottle
				stdi[x]          = STD_FILLING_STANDBY;
			 } // else if
			 break;
             
		case STD_CALIBRATION_STANDBY:
			 rgb_leds[x] = RED_ON;
			 if (mode[x] != MODE_CAL) stdi[x] = STD_DISABLED;
			 else if (keypressed[x])
			 {
				keypressed[x] = false; // reset key indicator
				stdi[x]       = STD_CALIBRATION_ACTIVE;
			 } // else
			 break;

		case STD_CALIBRATION_ACTIVE:
			 rgb_leds[x]  = RED_ON | RED_BLINKING;
             if (!(valve_on & (mask << 4))) // Valve command via UART?
                valve_on    |= mask; // enable valve
             temp = flow[x] - vbottle_start[x];
			 filled[x] = calc_mL(temp,cal[x]);  // calculate milli-litres
			 if (mode[x] != MODE_CAL) stdi[x] = STD_DISABLED;
			 else if (filled[x] > (VOL_CAL_SI - ML500_SI)) 
			 {  // make sure that flow has started
                old_flow[x] = flow[x];
				stdi[x] = STD_CALIBRATION_END;
			 } // else if
			 break;
             
		case STD_CALIBRATION_END:
			 rgb_leds[x]  = BLUE_ON | RED_ON | BLUE_BLINKING | RED_BLINKING;
             if (!(valve_on & (mask << 4))) // Valve command via UART?
                valve_on    |= mask; // enable valve
			 temp = flow[x] - vbottle_start[x];
			 filled[x] = calc_mL(temp,cal[x]);  // calculate milli-litres
             if (keypressed[x]) keypressed[x] = false; // no double keypresses allowed
			 if (mode[x] != MODE_CAL) stdi[x] = STD_DISABLED;
			 else if (flow[x] - old_flow[x] < 2) 
			 {  // we are done
                cal[x] = (uint8_t)((VOL_CAL * 100UL + (temp>>1)) / temp);
                eeprom_write_config(EEP_CAL1+x,cal[x]); // write to eeprom
                sprintf(s,"CAL[%d]=%d\n\r",(uint16_t)x,(uint16_t)cal[x]);
                xputs(s); // print to uart
                mode[x] = MODE_ON; // back to normal mode
				stdi[x] = STD_DISABLED;
			 } // else if
             old_flow[x] = flow[x];
			 break;
	} // switch
} // std()

/*--------------------------------------------------------------------
  Purpose  : This function is a wrapper for calling the state-transition
             diagram for every bottle-line. It is called every 100 msec.
  Returns  : -
  --------------------------------------------------------------------*/
void ctrl_task(void)
{
	std(V1);
    std(V2);
    std(V3);
    std(V4);
} // ctrl_task()

/*--------------------------------------------------------------------
  Purpose  : This prints the default line to the LCD-display for 1
             valve/flow combination, which is also 1 line on the display.
            v: [V1,V2,V3,V4]
  Returns  : -
  --------------------------------------------------------------------*/
void print_normal(uint8_t v)
{
    char s[20];
    
	lcd_i2c_setCursor(0,v);
	lcd_i2c_send('1'+v,DR);
	switch (mode[v])
    {
        case MODE_OFF: lcd_i2c_print(":OFF "); break;
        case MODE_ON : lcd_i2c_print(":ON  "); break;
        case MODE_CAL: lcd_i2c_print(":CAL "); break;
        case MODE_CLN: lcd_i2c_print(":CLN "); break;
    } // switch
    if (valve_on & (1<<v)) lcd_i2c_send('1',DR);
    else                   lcd_i2c_send('0',DR);
	lcd_i2c_setCursor(7,v);
    sprintf(s," %04d/",filled[v]);
    lcd_i2c_print(s);
    lcd_i2c_setCursor(13,v);
    if (mode[v] == MODE_CAL)
         sprintf(s,"%04d mL",(uint16_t)VOL_CAL_SI);
    else sprintf(s,"%04d mL",ml_si[vol_2_bottle[v]]);
    lcd_i2c_print(s);
} // print_normal()

/*--------------------------------------------------------------------
  Purpose  : This is the task that switches the valves on and off.
             It is called every 100 msec.
  Returns  : -
  --------------------------------------------------------------------*/
void valves_task(void)
{
	static uint8_t std = 0;
    
	switch (std)
	{
		case 0: if (valve_on & V1b) PC_ODR |=  VALVE1;
				else                PC_ODR &= ~VALVE1;
				std = 1;
				break;
		case 1: if (valve_on & V2b) PC_ODR |=  VALVE2;
				else                PC_ODR &= ~VALVE2;
				std = 2;
				break;
		case 2: if (valve_on & V3b) PC_ODR |=  VALVE3;
				else                PC_ODR &= ~VALVE3;
				std = 3;
				break;
		case 3: if (valve_on & V4b) PC_ODR |=  VALVE4;
				else                PC_ODR &= ~VALVE4;
				std = 0;
                uart_write('.');
				break;
	} // switch
} // valves_task()

/*--------------------------------------------------------------------
  Purpose  : This is the task that updates the LCD display with all
			 relevant values. It is called every 100 msec.
             This is the only task that prints to the LCD-I2C display.
  Returns  : -
  --------------------------------------------------------------------*/
void lcd_task(void)
{
    char     s[20];
    uint16_t rotenc;
    static int8_t   x,y;
    static uint8_t  std = STD_LCD_DISP1, std1 = 0;
    static uint16_t old_rotenc;
    static uint8_t  tmr;
    static bool     eep_save = false;
    
    rotenc = get_timer1_value();
   
	switch (std)
	{
		case STD_LCD_DISP1:
             lcd_i2c_blink_off();
             lcd_i2c_cursor_off();
             switch (std1)
             {
                case 0: print_normal(V1); std1 = 1; break;
                case 1: print_normal(V2); std1 = 2; break;
                case 2: print_normal(V3); std1 = 3; break;
                case 3: print_normal(V4); std1 = 0; break;
             } // switch
             if (rotenc_sw) 
             {
                rotenc_sw = false; // reset switch
                tmr       = 0;     // reset timer
                std       = STD_LCD_UI;
                x         = 2;
                y         = 0;
             } // if
			 break;

		case STD_LCD_UI:
            lcd_i2c_setCursor(x,y);
            lcd_i2c_blink_on();
            lcd_i2c_cursor_on();
            if (rotenc > old_rotenc)      std = STD_LCD_UI_UP;
            else if (rotenc < old_rotenc) std = STD_LCD_UI_DOWN;
            else if (rotenc_sw)           std = STD_LCD_UI_2;
            else if (++tmr > 100)         std = STD_LCD_DISP1;
            break;
            
        case STD_LCD_UI_UP:
             if (x == 2)  x = 13;
             else         
             {
                x = 2;
                if (++y > 3) y = 0;
             } // else
             tmr = 0;
             std = STD_LCD_UI;
             break;

        case STD_LCD_UI_DOWN:
             if (x == 13)  x = 2;
             else
             {
                x = 13;
                if (--y < 0) y = 3;
             } // else
             tmr = 0;
             std = STD_LCD_UI;
             break;

        case STD_LCD_UI_2:
             if (x == 2) 
                  std  = STD_LCD_ENA; // Enable/Disable bottler
             else std  = STD_LCD_VOL; // Set required volume 
             rotenc_sw = false;
             break;

        case STD_LCD_ENA: 
             if (rotenc > old_rotenc)               std = STD_LCD_ENA_UP;
             else if (rotenc < old_rotenc)          std = STD_LCD_ENA_DOWN;
             else if ((rotenc_sw) || (++tmr > 100)) std = STD_LCD_ENA_EEP;
             break;

        case STD_LCD_ENA_UP:
             if (++mode[y] > MODE_CLN) mode[y] = 0;
             std = STD_LCD_ENA_END;
             break;

        case STD_LCD_ENA_DOWN:
             if (--mode[y] < 0) mode[y] = MODE_CLN;
             std = STD_LCD_ENA_END;
             break;

        case STD_LCD_ENA_END:
             print_normal(y);
             lcd_i2c_setCursor(2,y);
             tmr = 0;
             std = STD_LCD_ENA;
             break;

        case STD_LCD_VOL: 
             if (rotenc > old_rotenc)               std = STD_LCD_VOL_UP;
             else if (rotenc < old_rotenc)          std = STD_LCD_VOL_DOWN;
             else if ((rotenc_sw) || (++tmr > 100)) std = STD_LCD_VOL_EEP;
             break;

        case STD_LCD_VOL_UP:
             if (++vol_2_bottle[y] > VOL_MAX) vol_2_bottle[y] = 0;
             std = STD_LCD_VOL_END;
             break;

        case STD_LCD_VOL_DOWN:
             if (--vol_2_bottle[y] < 0) vol_2_bottle[y] = VOL_MAX;
             std = STD_LCD_VOL_END;
             break;
             
        case STD_LCD_VOL_END:
             print_normal(y);
             lcd_i2c_setCursor(13,y);
             tmr = 0;
             std = STD_LCD_VOL;
             break;
             
        case STD_LCD_ENA_EEP:
             eeprom_write_config(EEP_MODE1,mode[V1] ? MODE_ON : MODE_OFF);
             eeprom_write_config(EEP_MODE2,mode[V2] ? MODE_ON : MODE_OFF);
             eeprom_write_config(EEP_MODE3,mode[V3] ? MODE_ON : MODE_OFF);
             eeprom_write_config(EEP_MODE4,mode[V4] ? MODE_ON : MODE_OFF);
             std = STD_LCD_DISP1;
             break;

        case STD_LCD_VOL_EEP:
             eeprom_write_config(EEP_VOL1,vol_2_bottle[V1]);
             eeprom_write_config(EEP_VOL2,vol_2_bottle[V2]);
             eeprom_write_config(EEP_VOL3,vol_2_bottle[V3]);
             eeprom_write_config(EEP_VOL4,vol_2_bottle[V4]);
             std = STD_LCD_DISP1;
             break;

        default: 
			std = STD_LCD_DISP1;
			break;
	} // switch
    old_rotenc = rotenc;
} // lcd_task()

/*-----------------------------------------------------------------------------
  Purpose  : This function reads all parameters from eeprom.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
void read_eep_pars(void)
{
   mode[V1]         = (int8_t)eeprom_read_config(EEP_MODE1);
   mode[V2]         = (int8_t)eeprom_read_config(EEP_MODE2);
   mode[V3]         = (int8_t)eeprom_read_config(EEP_MODE3);
   mode[V4]         = (int8_t)eeprom_read_config(EEP_MODE4);
   vol_2_bottle[V1] = (uint8_t)eeprom_read_config(EEP_VOL1);
   vol_2_bottle[V2] = (uint8_t)eeprom_read_config(EEP_VOL2);
   vol_2_bottle[V3] = (uint8_t)eeprom_read_config(EEP_VOL3);
   vol_2_bottle[V4] = (uint8_t)eeprom_read_config(EEP_VOL4);
   cal[V1]          = (uint8_t)eeprom_read_config(EEP_CAL1);
   cal[V2]          = (uint8_t)eeprom_read_config(EEP_CAL2);
   cal[V3]          = (uint8_t)eeprom_read_config(EEP_CAL3);
   cal[V4]          = (uint8_t)eeprom_read_config(EEP_CAL4);
} // read_eep_pars()

/*-----------------------------------------------------------------------------
  Purpose  : This is the main entry-point for the entire program.
             It initialises everything, starts the scheduler and dispatches
             all tasks.
  Variables: -
  Returns  : -
  ---------------------------------------------------------------------------*/
int main(void)
{
	char    s[30];     // Needed for xputs() and sprintf()
    int     ee = eedata[0]; // This is to prevent the linker from removing .eeprom section
    uint8_t ok, buf[5], bb = false;
	
    disable_interrupts();
    initialise_system_clock(); // Set system-clock to 16 MHz
    uart_init();               // UART init. to 115200,8,N,1
    setup_gpio_ports();        // Init. needed output-ports for LED and keys
    i2c_init(bb);              // Init. I2C bus
    setup_timers();            // Set Timer 2 to 1 kHz and Timer 1 for a Rotary Encoder

    // Initialise all tasks for the scheduler
    scheduler_init();          // clear task_list struct
	add_task(valves_task, "valves", 100, 100);
	add_task(ctrl_task  , "ctrl"  , 125, 100);
	add_task(lcd_task   , "lcd"   , 150, 100);
	add_task(led_task   , "led"   , 175, 500);
    enable_interrupts();
    lcd_i2c_init(0x4E,20,4,LCD_5x8DOTS); // Needs working interrupts!
    print_version_number(); // print version nr to UART and LCD-I2C display
    read_eep_pars();        // Read parameters from eeprom
    delay_msec(2000);       // Leave version nr 2 seconds on display 
    
    while (1)
    {   // background-processes
        dispatch_tasks();                // Run task-scheduler()
		switch (rs232_command_handler()) // run command handler continuously
		{
			case ERR_CMD: xputs("Command Error\n\r"); break;
			case ERR_NUM: sprintf(s,"Number Error (%s)\n\r",rs232_inbuf);
						  xputs(s);  
						  break;
			default     : break;
		} // switch
        //wait_for_interrupt(); // do nothing
    } // while
} // main()
