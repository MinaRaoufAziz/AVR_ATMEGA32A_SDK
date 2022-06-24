/**
*   @file:      gpio.h
*   @brief:     File contains the API(s) Prototypes and the needed types declaration.
*   @author:    Mina Raouf
*/

#ifndef __GPIO_H__
#define __GPIO_H__

#include "types.h"

#define DIRECTION_INPUT             0
#define DIRECTION_OUTPUT            1
#define PIN_STATE_LOW               0
#define PIN_STATE_HIGH              1
/**
*   @enum:  enum_supported_ports_t
*   @brief: Enumeration of the supported ports in ATMEGA32A Controller.
*   @note:  Any Supported Value shall be less than @val: ENU_PORT_TOTAL
*/
typedef enum
{
    ENU_PORT_A = 0,
    ENU_PORT_B,
    ENU_PORT_C,
    ENU_PORT_D,
    ENU_PORT_TOTAL
}enum_supported_ports_t;

/**
*   @enum:  enum_supported_directions_t
*   @brief: Enumeration of the supported directions in ATMEGA32A Controller.
*   @note:  Any Supported Value shall be less than @val: ENU_DIRECTION_TOTAL
*/
typedef enum
{
    ENU_DIRECTION_INPUT = 0,
    ENU_DIRECTION_OUTPUT,
    ENU_DIRECTION_TOTAL
}enum_supported_directions_t;

/**
*   @enum:  enum_supported_operations_t
*   @brief: Enumeration of the supported Operations in ATMEGA32A GPIO Driver.
*   @note:  Any Supported Value shall be less than @val: ENU_OPERATION_TOTAL
*/
typedef enum
{
    ENU_OPERATION_SET = 0,
    ENU_OPERATION_CLEAR,
    ENU_OPERATION_TOGGLE,
    ENU_OPERATION_TOTAL
}enum_supported_operations_t;

/**
*   @fn: gpio_set_pin_direction
*   @brief: This function is used to set the direction of a specific pin in one of the available ports.
*   @param[in]  enu_supported_ports:        Supported port
*   @param[in]  u8_pin_index:               Pin Index to set the direction.
*   @param[in]  enu_supported_directions:   Required Direction to set.
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_set_pin_direction(enum_supported_ports_t enu_supported_ports, uint8_type u8_pin_index, enum_supported_directions_t enu_supported_directions);

/**
*   @fn: gpio_get_pin_direction
*   @brief: This function is used to get the direction of a specific pin in one of the available ports.
*   @param[in]   enu_supported_ports:        Supported port
*   @param[in]   u8_pin_index:               Pin Index to set the direction.
*   @param[out]  pu8_pin_direction:         Pointer to the variable to assign the value of the pin direction. It shall be @ref: DIRECTION_INPUT or @ref: DIRECTION_OUTPUT
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_get_pin_direction(enum_supported_ports_t enu_supported_ports, uint8_type u8_pin_index, uint8_type*  pu8_pin_direction);

/**
*   @fn: gpio_mainpulate_pin
*   @brief: This function is used to change the value of a specific pin in one of the supported physical ports.
*   @param[in]  enu_supported_operation:        Operation to perform on the physical pin
*   @param[in]  enu_supported_ports:            Supported port
*   @param[in]  u8_pin_index:                   Pin Index to set the direction.
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_mainpulate_pin(enum_supported_operations_t enu_supported_operation, enum_supported_ports_t enu_supported_ports, uint8_type u8_pin_index);

/**
*   @fn: gpio_mainpulate_port
*   @brief: This function is used to change the value of one of the supported physical ports.
*   @param[in]  enu_supported_operation:        Operation to perform on the physical pin
*   @param[in]  enu_supported_ports:            Supported port
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_mainpulate_port(enum_supported_operations_t enu_supported_operation, enum_supported_ports_t enu_supported_ports);

/**
*   @fn: gpio_get_pin_status
*   @brief: This function is used to get the status of a specific physical pin in one of the available physical ports.
*   @param[in]   enu_supported_ports:        Supported port
*   @param[in]   u8_pin_index:               Pin Index to set the direction.
*   @param[out]  pu8_pin_status:             Pointer to the variable to assign the value of the pin status. It shall be @ref: PIN_STATE_LOW or @ref: PIN_STATE_HIGH
*   @return     SUCCESS_RETVAL in case of success, ERROR_INVALID_ARGUMENTS in case of wrong input parameters
*/
sint32_type gpio_get_pin_status(enum_supported_ports_t enu_supported_ports, uint8_type u8_pin_index, uint8_type*  pu8_pin_status);

#endif  /*__GPIO_H__*/