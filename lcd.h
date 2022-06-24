/**
*   @file:      lcd.h
*   @brief:     File contains the API(s) Prototypes and the needed types declaration.
*   @author:    Mina Raouf
*/

#ifndef __LCD_H__
#define __LCD_H__

#include "types.h"
#include "atmega32a_register_file.h"

#ifndef F_CPU
#define F_CPU	16000000
#endif

/**
    @defgroup:  LCD_INSTRUCTIONS_OPCODE
    @brief:     This is the supported insturctions for the CFAH1602A-AGB-JP LCD
*/
#define LCD_CLEAR_DISPLAY                           0x01
#define LCD_RETURN_HOME                             0x02
#define LCD_SHIFT_CURSOR_LEFT_PER_READ_WRITE        0x04
#define LCD_SHIFT_CURSOR_RIGHT_PER_READ_WRITE       0x06
#define LCD_SHIFT_DISPLAY_RIGHT_PER_READ_WRITE      0x05
#define LCD_SHIFT_DISPLAY_LEFT_PER_READ_WRITE       0x07
#define LCD_DISPLAY_CURSOR_BLINK_OFF                0x08
#define LCD_DISPLAY_OFF_CURSOR_ON                   0x0A
#define LCD_DISPLAY_ON_CURSOR_OFF                   0x0C
#define LCD_DISPLAY_ON_CURSOR_BLINK                 0x0E
#define LCD_SHIFT_CURSOR_POSITION_LEFT              0x10
#define LCD_SHIFT_CURSOR_POSITION_RIGHT             0x14
#define LCD_SHIFT_ENTIRE_DISPLAY_LEFT               0x18
#define LCD_SHIFT_ENTIRE_DISPLAY_RIGHT              0x1C
#define LCD_CURSOR_BEGIN_1ST_LINE                   0x80
#define LCD_CURSOR_BEGIN_2ND_LINE                   0xC0
#define LCD_4_BITS_MODE                             0x28
#if defined (LCD_8_BITS)
#define LCD_8_BITS_MODE                             0x38
#endif
#define LCD_FUNCTION_SET_1                          0x33
#define LCD_FUNCTION_SET_2                          0x32

/**
    @endgroup: LCD_INSTRUCTIONS_OPCODE
*/

#define LCD_RS_PIN          1
#define LCD_RS_PORT         REG_PORTB

#define LCD_E_PIN           3
#define LCD_E_PORT          REG_PORTB

#define LCD_RW_PIN          2
#define LCD_RW_PORT         REG_PORTB

#define LCD_D7_PIN          7
#define LCD_D6_PIN          6
#define LCD_D5_PIN          5
#define LCD_D4_PIN          4
#define LCD_DATA_PORT       REG_PORTA

#if defined (LCD_8_BITS)    /*Note: Kindly define this pins properly.*/
#define LCD_D3_PIN          0xFF
#define LCD_D2_PIN          0xFF
#define LCD_D1_PIN          0xFF
#define LCD_D0_PIN          0xFF
#endif

typedef enum
{
    LCD_INSTURCTION = 0,
    LCD_CHARACTER_DATA,
    LCD_INVALID_OPERATION,
}enu_lcd_byte_operation_type;

typedef enum
{
    LCD_INTEGRAL_DECIMAL = 0,
    LCD_INTEGRAL_HEXADECIMAL,
    LCD_INTEGRAL_TOTAL,
}enu_integral_operation_type;

/**
    @fn: lcd_init
    @brief: This function is used to configure the LCD operation parameters based on datasheet.
    @return: SUCCESS_RETVAL in case of Success, ERROR_MODULE_ALREADY_INITAILIZED in case of the LCD was initialized before.
*/
sint32_type lcd_init(void);

/**
    @fn: lcd_write_one_byte
    @brief: This function is used to write One byte. Whether it's an Insturction or a single byte of Data on the LCD.
    @param[in] enu_lcd_operation:   Requested Write Operation
    @param[in] uint8_data:          Requested Write Data type or Insturction based on the @ref: enu_lcd_operation 
    @return: SUCCESS_RETVAL in case of Success, ERROR_MODULE_NOT_INITAILIZED in case of the LCD was NOT initialized before or ERROR_INVALID_ARGUMENTS in case of invalid arguments.
*/
sint32_type lcd_write_one_byte(enu_lcd_byte_operation_type enu_lcd_operation, uint8_type uint8_data);

/**
    @fn: lcd_write_integral_data
    @brief: This function is used to write Integral Data on the LCD.
    @param[in] enu_integral_operation:      Requested Write Operation
    @param[in] sint32_integral_data:        Requested Integral Data to write 
    @return: SUCCESS_RETVAL in case of Success, ERROR_MODULE_NOT_INITAILIZED in case of the LCD was NOT initialized before or ERROR_INVALID_ARGUMENTS in case of invalid arguments.
*/
sint32_type lcd_write_integral_data(enu_integral_operation_type enu_integral_operation, sint32_type sint32_integral_data);

/**
    @fn: lcd_write_buffer
    @brief: This function is used to write a Buffer of data (Array) over the LCD.
    @param[in] puint8_buffer:           Pointer to the buffer to write on the LCD.
    @param[in] uint8_buffer_length:     Requested Buffer Length
    @return:        SUCCESS_RETVAL in case of Success, 
                or  ERROR_MODULE_NOT_INITAILIZED in case of the LCD was NOT initialized before or 
                or  ERROR_INVALID_ARGUMENTS in case of invalid arguments.
                or  ERROR_INVALD_LENGTH in case of buffer length is larger than the screen boundaries.
*/

sint32_type lcd_write_buffer(uint8_type* puint8_buffer, uint8_type uint8_buffer_length);

/**
    @fn:    lcd_goto_row_column
    @brief: This function is set the cursor to a specific row, column coordinates.
    @param[in] uint8_row:           Row Index.
    @param[in] uint8_column:        Column Index.
    @return:        SUCCESS_RETVAL in case of Success, 
                or  ERROR_MODULE_NOT_INITAILIZED in case of the LCD was NOT initialized before or 
                or  ERROR_INVALD_LENGTH in case of one or both the row or column larger than the screen boundaries.
*/
sint32_type lcd_goto_row_column(uint8_type uint8_row, uint8_type uint8_column);

/**
    @fn:    lcd_clear_screen
    @brief: This function is used to clear all the data on the screen.
    @param[in] None
    @return:   None
*/
void lcd_clear_screen(void);



#endif