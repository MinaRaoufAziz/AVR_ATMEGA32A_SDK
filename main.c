/*
 * main.c
 *
 * Created: 6/19/2022 10:47:59 PM
 * Author : Mina Raouf
 */ 
#include "lcd.h"
#include "gpio.h"
#include "bit_manipulation.h"

#ifndef F_CPU
#define F_CPU	16000000
#endif

#define STRING		"Hello Mina Raouf"


int main(void)
{
	lcd_init();
	gpio_set_pin_direction(ENU_PORT_B, 0, ENU_DIRECTION_INPUT);
	gpio_set_pin_direction(ENU_PORT_A, 3, ENU_DIRECTION_OUTPUT);
	lcd_write_one_byte(LCD_CHARACTER_DATA, 'A');
	lcd_clear_screen();
	lcd_write_buffer(STRING, (sizeof(STRING) - 1));
	lcd_goto_row_column(1,0);
	lcd_write_integral_data(LCD_INTEGRAL_DECIMAL, -1234);
	
	
    /* Replace with your application code */
    while (1) 
    {
		if(GET_BIT(REG_PINB, 0) == 1)
		{
			gpio_mainpulate_pin(ENU_OPERATION_SET, ENU_PORT_A, 3);
		}
		else
		{
			gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_A, 3);
		}
    }
}

