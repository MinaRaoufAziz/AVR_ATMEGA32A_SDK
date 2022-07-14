/*
 * main.c
 *
 * Created: 6/19/2022 10:47:59 PM
 * Author : Mina Raouf
 */ 
#include "lcd.h"
#include "gpio.h"
#include "timer.h"
#include "bit_manipulation.h"
#include "errors.h"
#include "common.h"
#include <avr/delay.h>

#ifndef F_CPU
#define F_CPU	16000000
#endif

#define STRING		"Hello Mina Raouf"

void generic_timer_callback(enum_timer_index_type enum_timer_index);

static const tstr_timer_config gstr_timer_0_config =	{
															.enum_timer_index				= TIMER_INDEX_0,
															.enum_timer_interrupt_usage		= TIMER_USAGE_INTERRUPT,
															.enum_timer_mode				= TIMER_MODE_OVF,
															.enum_timer_prescalar_value		= TIMER_8_PRESCALAR,
															.pointer_func_timer_callback	= generic_timer_callback,
														};

static const tstr_timer_config gstr_timer_2_config =	{
															.enum_timer_index				= TIMER_INDEX_2,
															.enum_timer_interrupt_usage		= TIMER_USAGE_INTERRUPT,
															.enum_timer_mode				= TIMER_MODE_CTC,
															.enum_timer_prescalar_value		= TIMER_8_PRESCALAR,
															.pointer_func_timer_callback	= generic_timer_callback,
														};

static const tstr_timer_config gstr_timer_0_pwm_config =	{
																.enum_timer_index				= TIMER_INDEX_0,
																.enum_timer_interrupt_usage		= TIMER_USAGE_INTERRUPT,
																.enum_timer_mode				= TIMER_MODE_PWM,
																.enum_timer_prescalar_value		= TIMER_8_PRESCALAR,
																.pointer_func_timer_callback	= generic_timer_callback,
															};

void generic_timer_callback(enum_timer_index_type enum_timer_index)
{
	switch(enum_timer_index)
	{
		case TIMER_INDEX_0:
		{
			/*Toggle LED 0*/
			//gpio_mainpulate_pin(ENU_OPERATION_TOGGLE, ENU_PORT_C, 2);
			break;
		}
		case TIMER_INDEX_1:
		{
			/*Toggle LED 1*/
			break;
		}
		case TIMER_INDEX_2:
		{
			/*Toggle LED 2*/
			//gpio_mainpulate_pin(ENU_OPERATION_TOGGLE, ENU_PORT_D, 3);
			break;
		}
		default:
		{
			break;
		}
	}	
}

int main(void)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	lcd_init();
	gpio_set_pin_direction(ENU_PORT_B, 0, ENU_DIRECTION_INPUT);
	gpio_set_pin_direction(ENU_PORT_D, 3, ENU_DIRECTION_OUTPUT);
	gpio_set_pin_direction(ENU_PORT_C, 2, ENU_DIRECTION_OUTPUT);
	//lcd_write_one_byte(LCD_CHARACTER_DATA, 'A');
	//lcd_clear_screen();
	//lcd_write_buffer(STRING, (sizeof(STRING) - 1));
	//lcd_goto_row_column(1,0);
	//_delay_ms(5000);
	//lcd_clear_screen();
	//lcd_write_buffer(__FUNCTION__, sizeof(__FUNCTION__)-1);
	//lcd_goto_row_column(1,0);
	//lcd_write_integral_data(LCD_INTEGRAL_DECIMAL, __LINE__);
	
	//gpio_mainpulate_pin(ENU_OPERATION_SET, ENU_PORT_D, 3);
	//timer_init(&gstr_timer_2_config);
	timer_init(&gstr_timer_0_pwm_config);
	gpio_mainpulate_pin(ENU_OPERATION_TOGGLE, ENU_PORT_D, 3);
	//timer_init(&gstr_timer_0_config);
	
	sint32_retval = timer_generate_pwm(TIMER_INDEX_0, 5);

	//sint32_retval = timer_delay(TIMER_INDEX_0, 5000);
	//sint32_retval = timer_delay(TIMER_INDEX_2, 2000);


	//lcd_write_integral_data(LCD_INTEGRAL_DECIMAL, F_CPU);
	
	
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

