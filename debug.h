#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "types.h"

#ifndef LCD_NUM_OF_ROW
#define LCD_NUM_OF_ROW      2
#endif

#ifndef LCD_NUM_OF_COL
#define LCD_NUM_OF_COL      13
#endif

#define LED_DEBUG           led_debug
#define LCD_DEBUG           lcd_debug

typedef enum
{
    LED_INDEX_0 = 0,
    LED_INDEX_1,
    LED_INDEX_2,
    LED_INDEX_INVALID,
}enum_led_index_type;

/**
 * @brief:  Function that is used to toggle LED knowing its index, upon matching the @var: sint32_retval with @val:sint32_expected_value for specific duration. 
 * 
 * @note:   This function will NOT operate at all upon passing an invalid parameter.
 * 
 * @param:  enum_led_index:               LED Index. @note: This shall be less than @val: LED_INDEX_INVALID
 * @param:  sint32_retval:                The variable the user want to check its value.
 * @param:  sint32_expected_error_value:  The Expected Error Value the user doubts that this variable equals to it.
 * @param:  uint16_action_duration_in_ms: The duration the user wants the LED to toggle. @note: This shall be greater than 0
 * @param:  uint8_repeat_action_times:    The Number of times the user want the LED to toggle. @note: Set to 0 For Continuous Toggling.
 * @return: None
 * 
 */
void led_debug(enum_led_index_type enum_led_index, sint32_type sint32_retval, sint32_type sint32_expected_error_value, uint16_type uint16_action_duration_in_ms, uint8_type uint8_repeat_action_times);

/**
 * @brief:  Function that is used to display the error value on the LCD in a specific row, col
 * 
 * @note:   This function will NOT operate at all upon passing an invalid parameter.
 * 
 * @param:  uint8_row_index:      Row index in which the user wants to display the error value. @note: This shall be less than @value: LCD_NUM_OF_ROW.
 * @param:  uint8_col_index:      Column Index in which the user wants to display the error value. @note: This shall be less than @value: LCD_NUM_OF_COL.
 * @param:  sint32_retval:        The variable the user wants to display on the LCD.
 * @return: None 
 */
void lcd_debug(uint8_type uint8_row_index, uint8_type uint8_col_index, sint32_type sint32_retval);
#endif /*__DEBUG_H__*/