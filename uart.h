/**
 * @file:   uart.h
 * @brief:  File Contains all the needed types and API(s) Prototypes
 * @author: Mina Raouf
 * 
 */

#ifndef __UART_H__
#define __UART_H__

#include "types.h"
#include "errors.h"

/*This Error Code will be used to indicate an Error In Either. Parity Error, Data Overrun Error, Frame Error.*/
#define USART_INTERNAL_FRAME_ERROR              (UART_ERROR_BASE - 1)   

/**
 * @enum:   enum_operation_done_type
 * @brief:  Enumeration that contains the type Operation Done by Either interrupt/ Polling.
 */

typedef enum
{
	RX_DONE	= 0,                /** @brief: Receive Operation Done*/
	TX_DONE,                    /** @brief: Transmission Operation Done*/
	INVALID_OPERATION_DONE      
}enum_operation_done_type;

/**
 * @enum:   enum_baud_rate_type
 * @brief:  Enumeration that contains the Types of Supported Baud Rates
 */
typedef enum
{
	BAUD_RATE_2400	= 0,
	BAUD_RATE_4800,
	BAUD_RATE_9600,
	BAUD_RATE_14400,
	BAUD_RATE_19200,
	BAUD_RATE_28800,
	BAUD_RATE_38400,
	BAUD_RATE_57600,
	BAUD_RATE_76800,
	BAUD_RATE_115200,
	BAUD_RATE_230400,
	BAUD_RATE_250000,
	BAUD_RATE_INVALID
}enum_baud_rate_type;

/**
 * @enum:   enum_interrupt_usage_type
 * @brief:  Enumeration that contains the Types of Interrupt Usage Configurations.
 */
typedef enum
{
    NO_INTERRUPT        = 0,
	RX_DONE_INTERRUPT,
	TX_DONE_INTERRUPT,
    BOTH_TX_RX_INTERRUPT,
	EMPTY_UDR_INTERRUPT,
	INVALID_INTERRUPT_USAGE
}enum_interrupt_usage_type;

/**
 * @enum:   enum_supported_role_type
 * @brief:  Enumeration that contains the Supported Operating Modes.
 */
typedef enum
{
	TX_ROLE_ENABLED	= 0,
	RX_ROLE_ENABLED,
	BOTH_TX_RX_ROLES_ENABLED,
	INVALID_ROLE_SUPPORTED
}enum_supported_role_type;

/**
 * @enum:   enum_usart_operating_mode_type
 * @brief:  Enumeration that contains the Supported Operating Modes for the USART Module.
 */
typedef enum
{
	UART_MODE	= 0,
	UART_MODE_DOUBLE_SPEED,
	USART_MODE,
	INVALID_USART_OPERATING_MODE
}enum_usart_operating_mode_type;

/**
 * @enum:   enum_parity_mode_type
 * @brief:  Enumeration that contains the Supported Parity Modes.
 */
typedef enum
{
	NO_PARITY_USED	= 0,
	PARITY_ODD,
	PARITY_EVEN,
	INVALID_PARITY_USED
}enum_parity_mode_type;

/**
 * @enum:   enum_stop_bits_type
 * @brief:  Enumeration that contains the Supported Stop Bits Number.
 */
typedef enum
{
	STOP_BIT_1	= 0,
	STOP_BIT_2,
	INVALID_STOP_BITS
}enum_stop_bits_type;

/**
 * @enum:   enum_clock_polarity_type
 * @brief:  Enumeration that contains the Supported Clock Polarity.
 */
typedef enum
{
	NO_CLOCK_USED	= 0,
	INVERTED_CLOCK,
	NON_INVERTED_CLOCK,
	INVALID_CLOCK_POLARITY
}enum_clock_polarity_type;

/**
 * @enum:   enum_data_frame_length_type
 * @brief:  Enumeration that contains the Supported Frame Data Length.
 */
typedef enum
{
    DATA_LENGTH_5_BITS  = 0,
    DATA_LENGTH_6_BITS,
    DATA_LENGTH_7_BITS,
    DATA_LENGTH_8_BITS,
    DATA_LENGTH_9_BITS,
    DATA_LENGTH_INVALID
}enum_data_frame_length_type;

/**
 * @typedef:    pointer_to_callback_func_type
 * @brief:      Definition of the Callback Function Type that will be propagated to the upper layers.
 * @param[in]   enum_operation_done:    The Operation Done
 * @param[in]   psint8_data:             Pointer to the Received Data. This can be assigned to NULL_PTR in case of @event: TX_DONE
 * 
 */
typedef void (*pointer_to_callback_func_type)(enum_operation_done_type enum_operation_done, sint8_type* psint8_data);

#if (0)
typedef void (*pointer_to_callback_func_type)(enum_operation_done_type enum_operation_done, uint8_type* puint8_buffer, uint16_type uint16_buffer_length);
#endif

typedef struct
{
	enum_baud_rate_type				enum_baud_rate;
	enum_interrupt_usage_type		enum_interrupt_usage;
	enum_supported_role_type		enum_supported_role;
	enum_usart_operating_mode_type	enum_usart_operating_mode;	
	enum_parity_mode_type			enum_parity_mode;
	enum_stop_bits_type				enum_stop_bits;
	enum_clock_polarity_type		enum_clock_polarity;
    enum_data_frame_length_type     enum_data_frame_length;
	pointer_to_callback_func_type	pointer_to_callback_func;
}struct_uart_configurations_type;

/**
 * @brief:      Function to initialize the USART Peripheral And Configure it
 * 
 * @param[in]   ptr_struct_uart_configurations:     Pointer to the Configurations Structure that holds all the configuration parameters 
 * @return      SUCCESS_RETVAL:                     In case of Success
 *              ERROR_INVALID_ARGUMENTS:            In case of Invalid Arguments Passed to this function.
 *              ERROR_MODULE_ALREADY_INITAILIZED:   In case that the module is already initialized previously.
 */
sint32_type uart_init(const struct_uart_configurations_type* const ptr_struct_uart_configurations);

/**
 * @brief:      Function to un-initialize the USART Peripheral And Clear its Configurations
 * 
 * @return      SUCCESS_RETVAL:                     In case of Success
 *              ERROR_MODULE_NOT_INITAILIZED:       In case that the module is not initialized previously.
 */
sint32_type uart_deinit(void);

/**
 * @brief:      Function that is used to send a Single Byte over the USART
 * 
 * @param[in]   sint8_data:                     The data that the user wants to send in ASCII Representation
 * @return      SUCCESS_RETVAL:                 In case of Success
 *              ERROR_MODULE_NOT_INITAILIZED:   In case that the module is not initialized.
 *              ERROR_MODULE_BUSY:              In case that there is another send operation in progress.
 */
sint32_type uart_send_byte(sint8_type sint8_data);

/**
 * @brief:      Function that is used to Receive a Single Byte over the USART
 * 
 * @param[out]  psint8_data:                    Pointer to a variable at which The received data will be stored. @note: The received data will be in ASCII Representation
 * @return      SUCCESS_RETVAL:                 In case of Success
 *              ERROR_MODULE_NOT_INITAILIZED:   In case that the module is not initialized.
 *              ERROR_INVALID_ARGUMENTS:        In case that the passed arguments are not valid.
 */
sint32_type uart_receive_byte(sint8_type* psint8_data);

/**
 * @brief:      Function that is used to send A Buffer over the USART.
 * 
 * @param[in]   psint8_buffer:                  Pointer to the buffer. The Data Representation shall be ASCII Representation. 
 * @param[in]   uint16_buffer_length:           The Buffer Length that will be sent over the USART. 
 * @return      SUCCESS_RETVAL:                 In case of Success
 *              ERROR_MODULE_NOT_INITAILIZED:   In case that the module is not initialized.
 *              ERROR_MODULE_BUSY:              In case that there is another send operation in progress.
 */
sint32_type uart_send_buffer(sint8_type* psint8_buffer, uint16_type uint16_buffer_length);

#endif /*__UART_H__*/