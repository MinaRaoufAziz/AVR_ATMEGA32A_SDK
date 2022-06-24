/*
 * lcd.c
 *
 * Created: 6/10/2022 10:47:59 PM
 * Author : Mina Raouf
 */ 

#include "lcd.h"
#include "errors.h"
#include "common.h"
#include "gpio.h"
#include "avr/delay.h"

#define LCD_FALSE				0
#define LCD_TRUE				1


static uint8_type guint8_is_lcd_initialized = LCD_FALSE;
/**
    @fn: lcd_init
    @brief: This function is used to configure the LCD operation parameters based on datasheet.
    @return: SUCCESS_RETVAL in case of Success, ERROR_MODULE_ALREADY_INITAILIZED in case of the LCD was initialized before.
*/
sint32_type lcd_init(void)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	if(guint8_is_lcd_initialized == LCD_FALSE)
	{
		do
		{
			/*This means that we didn't call the lcd_init before. (This is the first call )*/
			guint8_is_lcd_initialized = LCD_TRUE;

			sint32_retval = gpio_set_pin_direction(ENU_PORT_B, LCD_RS_PIN, ENU_DIRECTION_OUTPUT);
			BREAK_ON_ERROR(sint32_retval);
			sint32_retval = gpio_set_pin_direction(ENU_PORT_B, LCD_RW_PIN, ENU_DIRECTION_OUTPUT);
			BREAK_ON_ERROR(sint32_retval);
			sint32_retval = gpio_set_pin_direction(ENU_PORT_B, LCD_E_PIN, ENU_DIRECTION_OUTPUT);
			BREAK_ON_ERROR(sint32_retval);

			_delay_ms(5);
			
			sint32_retval = gpio_set_pin_direction(ENU_PORT_A, LCD_D7_PIN, ENU_DIRECTION_OUTPUT);
			BREAK_ON_ERROR(sint32_retval);
			sint32_retval = gpio_set_pin_direction(ENU_PORT_A, LCD_D6_PIN, ENU_DIRECTION_OUTPUT);
			BREAK_ON_ERROR(sint32_retval);
			sint32_retval = gpio_set_pin_direction(ENU_PORT_A, LCD_D5_PIN, ENU_DIRECTION_OUTPUT);
			BREAK_ON_ERROR(sint32_retval);
			sint32_retval = gpio_set_pin_direction(ENU_PORT_A, LCD_D4_PIN, ENU_DIRECTION_OUTPUT);
			BREAK_ON_ERROR(sint32_retval);

			sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, LCD_FUNCTION_SET_1);
			BREAK_ON_ERROR(sint32_retval);
			_delay_ms(2);
			sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, LCD_FUNCTION_SET_2);
			BREAK_ON_ERROR(sint32_retval);
			_delay_ms(2);

#if defined (LCD_8_BITS)
			sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, LCD_8_BITS_MODE);
			BREAK_ON_ERROR(sint32_retval);
#else
			sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, LCD_4_BITS_MODE);
			BREAK_ON_ERROR(sint32_retval);
#endif

			sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, LCD_CLEAR_DISPLAY);
			BREAK_ON_ERROR(sint32_retval);
			sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, LCD_SHIFT_CURSOR_RIGHT_PER_READ_WRITE);
			BREAK_ON_ERROR(sint32_retval);
			sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, LCD_DISPLAY_ON_CURSOR_OFF);
			BREAK_ON_ERROR(sint32_retval);
		}while(0);
	}
	else
	{
		/*This means that we called the lcd_init before. So will not configure it again.*/
		sint32_retval = ERROR_MODULE_ALREADY_INITAILIZED;
	}
	return sint32_retval;
}

/**
    @fn: lcd_write_one_byte
    @brief: This function is used to write Insturction or Data on the LCD.
    @param[in] enu_lcd_operation:   Requested Write Operation
    @param[in] uint8_data:          Requested Write Data type or Insturction based on the @ref: enu_lcd_operation 
    @return: SUCCESS_RETVAL in case of Success, ERROR_MODULE_NOT_INITAILIZED in case of the LCD was NOT initialized before or ERROR_INVALID_ARGUMENTS in case of invalid arguments.
*/
sint32_type lcd_write_one_byte(enu_lcd_byte_operation_type enu_lcd_operation, uint8_type uint8_data)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	if(enu_lcd_operation < LCD_INVALID_OPERATION)
	{
		if(guint8_is_lcd_initialized == LCD_TRUE)
		{
			switch(enu_lcd_operation)
			{
				case LCD_INSTURCTION:
				{
					do
					{
						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_RS_PIN);
						BREAK_ON_ERROR(sint32_retval);
						
						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_RW_PIN);
						BREAK_ON_ERROR(sint32_retval);
						
						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);


						LCD_DATA_PORT = (uint8_data & 0xF0) | (LCD_DATA_PORT & 0x0F);


						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_SET, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);
					
						_delay_ms(1);

						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);


						LCD_DATA_PORT = (uint8_data << 4) | (LCD_DATA_PORT & 0x0F);


						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_SET, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);
					
						_delay_ms(1);

						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);

						_delay_ms(5);
						
					}while(0);
					
					break;
				}
				case LCD_CHARACTER_DATA:
				{
					do
					{
						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_SET, ENU_PORT_B, LCD_RS_PIN);
						BREAK_ON_ERROR(sint32_retval);
						
						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_RW_PIN);
						BREAK_ON_ERROR(sint32_retval);
						
						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);


						LCD_DATA_PORT = (uint8_data & 0xF0) | (LCD_DATA_PORT & 0x0F);


						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_SET, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);
					
						_delay_ms(1);

						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);


						LCD_DATA_PORT = (uint8_data << 4) | (LCD_DATA_PORT & 0x0F);


						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_SET, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);
					
						_delay_ms(1);

						sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_E_PIN);
						BREAK_ON_ERROR(sint32_retval);

						_delay_ms(5);
						
					}while(0);

					break;
				}
				default:
				{
					sint32_retval = ERROR_OUT_OF_BOUNDARY;
					break;
				}
			}
#if 0
			if(sint32_retval == SUCCESS_RETVAL)
			{
				sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_RW_PIN);
				BREAK_ON_ERROR(sint32_retval);

				sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_E_PIN);
				BREAK_ON_ERROR(sint32_retval);


				LCD_DATA_PORT = (uint8_data & 0xF0) | (LCD_DATA_PORT & 0x0F);


				sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_SET, ENU_PORT_B, LCD_E_PIN);
				BREAK_ON_ERROR(sint32_retval);

				_delay_ms(1);

				sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_E_PIN);
				BREAK_ON_ERROR(sint32_retval);


				LCD_DATA_PORT = (uint8_data << 4) | (LCD_DATA_PORT & 0x0F);


				sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_SET, ENU_PORT_B, LCD_E_PIN);
				BREAK_ON_ERROR(sint32_retval);

				_delay_ms(1);

				sint32_retval = gpio_mainpulate_pin(ENU_OPERATION_CLEAR, ENU_PORT_B, LCD_E_PIN);
				BREAK_ON_ERROR(sint32_retval);

				_delay_ms(5);
			}	
#endif
		}
		else
		{
			sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
		}
	}
	else
	{
		sint32_retval = ERROR_INVALID_ARGUMENTS;
	}
	return sint32_retval;
}

/**
    @fn: lcd_write_integral_data
    @brief: This function is used to write Integral Data on the LCD.
    @param[in] enu_integral_operation:      Requested Write Operation
    @param[in] sint32_integral_data:        Requested Integral Data to write 
    @return: SUCCESS_RETVAL in case of Success, ERROR_MODULE_NOT_INITAILIZED in case of the LCD was NOT initialized before or ERROR_INVALID_ARGUMENTS in case of invalid arguments.
*/
sint32_type lcd_write_integral_data(enu_integral_operation_type enu_integral_operation, sint32_type sint32_integral_data)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	if(guint8_is_lcd_initialized == LCD_TRUE)
	{
		if(enu_integral_operation < LCD_INTEGRAL_TOTAL)
		{
			switch(enu_integral_operation)
			{
				case LCD_INTEGRAL_DECIMAL:
				{
					uint32_type 	uint32_reversed_num = 0;
					uint8_type 		uint8_displayed_num = 0;
					uint32_type 	uint32_multiplier = 1;
					uint32_type 	uint32_iterator = 0;
					if(sint32_integral_data == 0)
					{
						sint32_retval = lcd_write_one_byte(LCD_CHARACTER_DATA, '0');
						if(sint32_retval != SUCCESS_RETVAL)
						{
							/*MR_TODO: Remove This after testing the driver. And Replace it with break*/
						}
					}
					else if(sint32_integral_data < 0)
					{
						sint32_retval = lcd_write_one_byte(LCD_CHARACTER_DATA, '-');
						if(sint32_retval != SUCCESS_RETVAL)
						{
							/*MR_TODO: Remove This after testing the driver. And Replace it with break*/
						}
						sint32_integral_data *= -1;
					}
					
					while (sint32_integral_data != 0)
					{
						uint32_reversed_num = (uint32_reversed_num * 10) + sint32_integral_data % 10;
						if (uint32_reversed_num == 0)	
						uint32_multiplier *= 10;
						sint32_integral_data /= 10;
					}	
					while (uint32_reversed_num != uint32_iterator)
					{
						uint8_displayed_num = uint32_reversed_num % 10;
						sint32_retval = lcd_write_one_byte(LCD_CHARACTER_DATA, (uint8_displayed_num + 0x30));
						if(sint32_retval != SUCCESS_RETVAL)
						{
							/*MR_TODO: Remove This after testing the driver. And Replace it with break*/
						}
						uint32_reversed_num/=10;
						if ((uint32_reversed_num == 0) && (uint32_multiplier >= 10))
						{
							uint32_iterator = 1;
							uint32_reversed_num = uint32_multiplier;
						}	
					}
					break;
				}
				case LCD_INTEGRAL_HEXADECIMAL:
				{

					break;
				}
				default:
				{

					break;
				}
			}
		}
		else
		{
			sint32_retval = ERROR_INVALID_ARGUMENTS;
		}
	}
	else
	{
		sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
	}
	return sint32_retval;
}

/**
    @fn: lcd_write_buffer
    @brief: This function is used to write a Buffer of data (Array) over the LCD.
    @param[in] psint8_buffer:           Pointer to the buffer to write on the LCD.
    @param[in] uint8_buffer_length:     Requested Buffer Length
    @return:        SUCCESS_RETVAL in case of Success, 
                or  ERROR_MODULE_NOT_INITAILIZED in case of the LCD was NOT initialized before or 
                or  ERROR_INVALID_ARGUMENTS in case of invalid arguments.
                or  ERROR_INVALD_LENGTH in case of buffer length is larger than the screen boundaries.
*/

sint32_type lcd_write_buffer(uint8_type* puint8_buffer, uint8_type uint8_buffer_length)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	uint8_type uint8_local_iterator;
	if(guint8_is_lcd_initialized == LCD_TRUE)
	{
		if((puint8_buffer != NULL_PTR) && (uint8_buffer_length > 0))
		{
			for(uint8_local_iterator = 0; uint8_local_iterator < uint8_buffer_length; uint8_local_iterator++)
			{
				sint32_retval = lcd_write_one_byte(LCD_CHARACTER_DATA, puint8_buffer[uint8_local_iterator]);
				if(sint32_retval != SUCCESS_RETVAL)
				{
					/*MR_TODO: Remove This after testing the driver. And Replace it with break*/
				}
			}
			
		}
		else
		{
			sint32_retval = ERROR_INVALID_ARGUMENTS;
		}
	}
	else
	{
		sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
	}
	return sint32_retval;
}

/**
    @fn:    lcd_goto_row_column
    @brief: This function is set the cursor to a specific row, column coordinates.
    @param[in] uint8_row:           Row Index.
    @param[in] uint8_column:        Column Index.
    @return:        SUCCESS_RETVAL in case of Success, 
                or  ERROR_MODULE_NOT_INITAILIZED in case of the LCD was NOT initialized before or 
                or  ERROR_INVALD_LENGTH in case of one or both the row or column larger than the screen boundaries.
*/
sint32_type lcd_goto_row_column(uint8_type uint8_row, uint8_type uint8_column)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	if(guint8_is_lcd_initialized == LCD_TRUE)
	{
		if((uint8_row >= 0) && (uint8_row < 2) && (uint8_column >= 0) && (uint8_column < 16))
		{
			if(uint8_row == 0)
			{
				sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, (LCD_CURSOR_BEGIN_1ST_LINE + uint8_column));
				if(sint32_retval != SUCCESS_RETVAL)
				{
					/*MR_TODO: Remove This after testing the driver. And Replace it with break*/
				}
			}
			else if(uint8_row == 1)
			{
				sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, (LCD_CURSOR_BEGIN_2ND_LINE + uint8_column));
				if(sint32_retval != SUCCESS_RETVAL)
				{
					/*MR_TODO: Remove This after testing the driver. And Replace it with break*/
				}
			}
		}
		else
		{
			sint32_retval = ERROR_INVALD_LENGTH;
		}
	}
	else
	{
		sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
	}
	return sint32_retval;
}

/**
    @fn:    lcd_clear_screen
    @brief: This function is used to clear all the data on the screen.
    @param[in] None
    @return:   None
*/
void lcd_clear_screen(void)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	sint32_retval = lcd_write_one_byte(LCD_INSTURCTION, LCD_CLEAR_DISPLAY);
	if(sint32_retval != SUCCESS_RETVAL)
	{
		/*MR_TODO: Remove This after testing the driver. And Replace it with break*/
	}
}