
#include "gpio.h"
#include "errors.h"
#include "bit_manipulation.h"
#include "atmega32a_register_file.h"
#include "types.h"
#define MAX_PINS_PER_PORT		8
/**
*   @fn: gpio_set_pin_direction
*   @brief: This function is used to set the direction of a specific pin in one of the available ports.
*   @param[in]  enu_supported_ports:        Supported port
*   @param[in]  u8_pin_index:               Pin Index to set the direction.
*   @param[in]  enu_supported_directions:   Required Direction to set.
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_set_pin_direction(enum_supported_ports_t enu_supported_ports, uint8_type u8_pin_index, enum_supported_directions_t enu_supported_directions)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	if((enu_supported_ports < ENU_PORT_TOTAL) && (u8_pin_index < MAX_PINS_PER_PORT) && (enu_supported_directions < ENU_DIRECTION_TOTAL))
	{
		switch(enu_supported_ports)
		{
			case ENU_PORT_A:
			{
				if(ENU_DIRECTION_INPUT == enu_supported_directions)
				{
					CLEAR_BIT(REG_DDRA, u8_pin_index);
				}
				else if(ENU_DIRECTION_OUTPUT == enu_supported_directions)
				{
					SET_BIT(REG_DDRA, u8_pin_index);
				}
				break;
			}
			
			case ENU_PORT_B:
			{
				if(ENU_DIRECTION_INPUT == enu_supported_directions)
				{
					CLEAR_BIT(REG_DDRB, u8_pin_index);
				}
				else if(ENU_DIRECTION_OUTPUT == enu_supported_directions)
				{
					SET_BIT(REG_DDRB, u8_pin_index);
				}
				break;
			}
			
			case ENU_PORT_C:
			{
				if(ENU_DIRECTION_INPUT == enu_supported_directions)
				{
					CLEAR_BIT(REG_DDRC, u8_pin_index);
				}
				else if(ENU_DIRECTION_OUTPUT == enu_supported_directions)
				{
					SET_BIT(REG_DDRC, u8_pin_index);
				}
				break;
			}
			
			case ENU_PORT_D:
			{
				if(ENU_DIRECTION_INPUT == enu_supported_directions)
				{
					CLEAR_BIT(REG_DDRD, u8_pin_index);
				}
				else if(ENU_DIRECTION_OUTPUT == enu_supported_directions)
				{
					SET_BIT(REG_DDRD, u8_pin_index);
				}
				break;
			}
		}
	}
	else
	{
		sint32_retval = ERROR_INVALID_ARGUMENTS;
	}
	return sint32_retval;
}

/**
*   @fn: gpio_get_pin_direction
*   @brief: This function is used to get the direction of a specific pin in one of the available ports.
*   @param[in]   enu_supported_ports:        Supported port
*   @param[in]   u8_pin_index:               Pin Index to set the direction.
*   @param[out]  pu8_pin_direction:         Pointer to the variable to assign the value of the pin direction. It shall be @ref: DIRECTION_INPUT or @ref: DIRECTION_OUTPUT
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_get_pin_direction(enum_supported_ports_t enu_supported_ports, uint8_type u8_pin_index, uint8_type*  pu8_pin_direction)
{
	
}

/**
*   @fn: gpio_mainpulate_pin
*   @brief: This function is used to change the value of a specific pin in one of the supported physical ports.
*   @param[in]  enu_supported_operation:        Operation to perform on the physical pin
*   @param[in]  enu_supported_ports:            Supported port
*   @param[in]  u8_pin_index:                   Pin Index to set the direction.
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_mainpulate_pin(enum_supported_operations_t enu_supported_operation, enum_supported_ports_t enu_supported_ports, uint8_type u8_pin_index)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	if((enu_supported_operation < ENU_OPERATION_TOTAL) && (enu_supported_ports < ENU_PORT_TOTAL) && (u8_pin_index < MAX_PINS_PER_PORT))
	{
		switch(enu_supported_operation)
		{
			case ENU_OPERATION_SET:
			{
				switch(enu_supported_ports)
				{
					case ENU_PORT_A:
					{
						SET_BIT(REG_PORTA, u8_pin_index);
						break;
					}
					case ENU_PORT_B:
					{
						SET_BIT(REG_PORTB, u8_pin_index);
						break;
					}
					case ENU_PORT_C:
					{
						SET_BIT(REG_PORTC, u8_pin_index);
						break;
					}
					case ENU_PORT_D:
					{
						SET_BIT(REG_PORTD, u8_pin_index);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			
			case ENU_OPERATION_CLEAR:
			{
				switch(enu_supported_ports)
				{
					case ENU_PORT_A:
					{
						CLEAR_BIT(REG_PORTA, u8_pin_index);
						break;
					}
					case ENU_PORT_B:
					{
						CLEAR_BIT(REG_PORTB, u8_pin_index);
						break;
					}
					case ENU_PORT_C:
					{
						CLEAR_BIT(REG_PORTC, u8_pin_index);
						break;
					}
					case ENU_PORT_D:
					{
						CLEAR_BIT(REG_PORTD, u8_pin_index);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			
			case ENU_OPERATION_TOGGLE:
			{
				switch(enu_supported_ports)
				{
					case ENU_PORT_A:
					{
						TOGGLE_BIT(REG_PORTA, u8_pin_index);
						break;
					}
					case ENU_PORT_B:
					{
						TOGGLE_BIT(REG_PORTB, u8_pin_index);
						break;
					}
					case ENU_PORT_C:
					{
						TOGGLE_BIT(REG_PORTC, u8_pin_index);
						break;
					}
					case ENU_PORT_D:
					{
						TOGGLE_BIT(REG_PORTD, u8_pin_index);
						break;
					}
					default:
					{
						break;
					}
				}
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
	return sint32_retval;
}

/**
*   @fn: gpio_mainpulate_port
*   @brief: This function is used to change the value of one of the supported physical ports.
*   @param[in]  enu_supported_operation:        Operation to perform on the physical pin
*   @param[in]  enu_supported_ports:            Supported port
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_mainpulate_port(enum_supported_operations_t enu_supported_operation, enum_supported_ports_t enu_supported_ports)
{
	sint32_type sint32_retval = SUCCESS_RETVAL;
	if((enu_supported_operation < ENU_OPERATION_TOTAL) && (enu_supported_ports < ENU_PORT_TOTAL))
	{
		switch(enu_supported_operation)
		{
			case ENU_OPERATION_SET:
			{
				switch(enu_supported_ports)
				{
					case ENU_PORT_A:
					{
						SET_PORT(REG_PORTA);
						break;
					}
					case ENU_PORT_B:
					{
						SET_PORT(REG_PORTB);
						break;
					}
					case ENU_PORT_C:
					{
						SET_PORT(REG_PORTC);
						break;
					}
					case ENU_PORT_D:
					{
						SET_PORT(REG_PORTD);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			
			case ENU_OPERATION_CLEAR:
			{
				switch(enu_supported_ports)
				{
					case ENU_PORT_A:
					{
						CLEAR_PORT(REG_PORTA);
						break;
					}
					case ENU_PORT_B:
					{
						CLEAR_PORT(REG_PORTB);
						break;
					}
					case ENU_PORT_C:
					{
						CLEAR_PORT(REG_PORTC);
						break;
					}
					case ENU_PORT_D:
					{
						CLEAR_PORT(REG_PORTD);
						break;
					}
					default:
					{
						break;
					}
				}
				break;
			}
			
			case ENU_OPERATION_TOGGLE:
			{
				switch(enu_supported_ports)
				{
					case ENU_PORT_A:
					{
						TOGGLE_PORT(REG_PORTA);
						break;
					}
					case ENU_PORT_B:
					{
						TOGGLE_PORT(REG_PORTB);
						break;
					}
					case ENU_PORT_C:
					{
						TOGGLE_PORT(REG_PORTC);
						break;
					}
					case ENU_PORT_D:
					{
						TOGGLE_PORT(REG_PORTD);
						break;
					}
					default:
					{
						break;
					}
				}
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
	return sint32_retval;
}

/**
*   @fn: gpio_get_pin_status
*   @brief: This function is used to get the status of a specific physical pin in one of the available physical ports.
*   @param[in]   enu_supported_ports:        Supported port
*   @param[in]   u8_pin_index:               Pin Index to set the direction.
*   @param[out]  pu8_pin_status:             Pointer to the variable to assign the value of the pin status. It shall be @ref: PIN_STATE_LOW or @ref: PIN_STATE_HIGH
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_get_pin_status(enum_supported_ports_t enu_supported_ports, uint8_type u8_pin_index, uint8_type*  pu8_pin_status)
{
	
}
