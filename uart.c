/**
 * @file:       uart.c
 * @author:     Mina Raouf
 * @brief:      File Contains the implementation of the Universal Synchronous Asynchronous Receiver Transmitter (USART) 
 * @version:    1.0
 * @date:       2022-07-13
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include"uart.h"
#include "errors.h"
#include "atmega32a_register_file.h"
#include "bit_manipulation.h"
#include <avr/interrupt.h>

#define BAUD_2400_FOR_16_MHZ_NORMAL_SPEED           416
#define BAUD_4800_FOR_16_MHZ_NORMAL_SPEED           207
#define BAUD_9600_FOR_16_MHZ_NORMAL_SPEED           103
#define BAUD_14400_FOR_16_MHZ_NORMAL_SPEED          68
#define BAUD_19200_FOR_16_MHZ_NORMAL_SPEED          51
#define BAUD_28800_FOR_16_MHZ_NORMAL_SPEED          34
#define BAUD_38400_FOR_16_MHZ_NORMAL_SPEED          25
#define BAUD_57600_FOR_16_MHZ_NORMAL_SPEED          16
#define BAUD_76800_FOR_16_MHZ_NORMAL_SPEED          12
#define BAUD_115200_FOR_16_MHZ_NORMAL_SPEED         8
#define BAUD_230400_FOR_16_MHZ_NORMAL_SPEED         3
#define BAUD_250000_FOR_16_MHZ_NORMAL_SPEED         3

#define BAUD_2400_FOR_16_MHZ_DOUBLE_SPEED           832
#define BAUD_4800_FOR_16_MHZ_DOUBLE_SPEED           416
#define BAUD_9600_FOR_16_MHZ_DOUBLE_SPEED           207
#define BAUD_14400_FOR_16_MHZ_DOUBLE_SPEED          138
#define BAUD_19200_FOR_16_MHZ_DOUBLE_SPEED          103
#define BAUD_28800_FOR_16_MHZ_DOUBLE_SPEED          68
#define BAUD_38400_FOR_16_MHZ_DOUBLE_SPEED          51
#define BAUD_57600_FOR_16_MHZ_DOUBLE_SPEED          34
#define BAUD_76800_FOR_16_MHZ_DOUBLE_SPEED          25
#define BAUD_115200_FOR_16_MHZ_DOUBLE_SPEED         16
#define BAUD_230400_FOR_16_MHZ_DOUBLE_SPEED         8
#define BAUD_250000_FOR_16_MHZ_DOUBLE_SPEED         7

/*Register UCSRA Bits*/
#define REG_UCSRA_RXC_BIT                                     7
#define REG_UCSRA_TXC_BIT                                     6
#define REG_UCSRA_UDRE_BIT                                    5
#define REG_UCSRA_FE_BIT                                      4
#define REG_UCSRA_DOR_BIT                                     3
#define REG_UCSRA_PE_BIT                                      2
#define REG_UCSRA_U2X_BIT                                     1

/*Register UCSRB Bits*/
#define REG_UCSRB_RXCIE_BIT                                   7
#define REG_UCSRB_TXCIE_BIT                                   6
#define REG_UCSRB_UDRIE_BIT                                   5
#define REG_UCSRB_RXEN_BIT                                    4
#define REG_UCSRB_TXEN_BIT                                    3
#define REG_UCSRB_UCSZ2_BIT                                   2

/*Register UCSRC Bits*/
#define REG_UCSRC_UMSEL_BIT                                   6
#define REG_UCSRC_UPM1_BIT                                    5
#define REG_UCSRC_UPM0_BIT                                    4
#define REG_UCSRC_USBS_BIT                                    3
#define REG_UCSRC_UCSZ1_BIT                                   2
#define REG_UCSRC_UCSZ0_BIT                                   1
#define REG_UCSRC_UCPOL_BIT                                   0


static uint8_type                           global_uint8_is_module_initialized          = FALSE;
static uint8_type                           global_uint8_is_send_byte_in_progress       = FALSE;
static uint8_type                           global_uint8_is_send_buffer_in_progress     = FALSE;
static uint16_type                          global_uint16_number_of_bytes_sent          = 0;
static struct_uart_configurations_type*     global_ptr_usart_database                   = NULL_PTR;


/**
 * @brief:      Function to initialize the USART Peripheral And Configure it
 * 
 * @param[in]   ptr_struct_uart_configurations:     Pointer to the Configurations Structure that holds all the configuration parameters 
 * @return      SUCCESS_RETVAL:                     In case of Success
 *              ERROR_INVALID_ARGUMENTS:            In case of Invalid Arguments Passed to this function.
 *              ERROR_MODULE_ALREADY_INITAILIZED:   In case that the module is already initialized previously.
 */
sint32_type uart_init(const struct_uart_configurations_type* const ptr_struct_uart_configurations)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if(global_uint8_is_module_initialized == FALSE)
    {
        if(ptr_struct_uart_configurations != NULL_PTR)
        {
            if  (
                    ((ptr_struct_uart_configurations->enum_baud_rate)               < BAUD_RATE_INVALID)                &&
                    ((ptr_struct_uart_configurations->enum_clock_polarity)          < INVALID_CLOCK_POLARITY)           &&
                    ((ptr_struct_uart_configurations->enum_usart_operating_mode)    < INVALID_USART_OPERATING_MODE)     &&
                    ((ptr_struct_uart_configurations->enum_interrupt_usage)         < INVALID_INTERRUPT_USAGE)          &&
                    ((ptr_struct_uart_configurations->enum_parity_mode)             < INVALID_PARITY_USED)              &&
                    ((ptr_struct_uart_configurations->enum_stop_bits)               < INVALID_STOP_BITS)                &&
                    ((ptr_struct_uart_configurations->enum_supported_role)          < INVALID_ROLE_SUPPORTED)           &&
                    ((ptr_struct_uart_configurations->enum_data_frame_length)       < DATA_LENGTH_INVALID)              &&
                    ((ptr_struct_uart_configurations->pointer_to_callback_func)     != NULL_PTR)
                )
            {
                switch (ptr_struct_uart_configurations->enum_usart_operating_mode)
                {
                    case UART_MODE:
                    {
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_UMSEL_BIT);
                        CLEAR_BIT(REG_UCSRA, REG_UCSRA_U2X_BIT);
                        break;
                    }
                    case UART_MODE_DOUBLE_SPEED:
                    {
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_UMSEL_BIT);
                        SET_BIT(REG_UCSRA, REG_UCSRA_U2X_BIT);
                        break;
                    }
                    case USART_MODE:
                    {
                        SET_BIT(REG_UCSRC, REG_UCSRC_UMSEL_BIT);
                        CLEAR_BIT(REG_UCSRA, REG_UCSRA_U2X_BIT);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                switch(ptr_struct_uart_configurations->enum_baud_rate)
                {
                    case BAUD_RATE_2400:
                    case BAUD_RATE_4800:
                    {
                        break;
                    }
                    case BAUD_RATE_9600:
                    {
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_9600_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_9600_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    case BAUD_RATE_14400:
                    {
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_14400_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_14400_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    case BAUD_RATE_19200:
                    {
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_19200_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_19200_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    case BAUD_RATE_28800:
                    {
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_28800_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_28800_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    case BAUD_RATE_38400:
                    {   
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_38400_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_38400_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    case BAUD_RATE_57600:
                    {
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_57600_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_57600_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    case BAUD_RATE_76800:
                    {
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_76800_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_76800_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    case BAUD_RATE_115200:
                    {
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_115200_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_115200_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    case BAUD_RATE_230400:
                    {
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_230400_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_230400_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    case BAUD_RATE_250000:
                    {
                        if((ptr_struct_uart_configurations->enum_usart_operating_mode) == UART_MODE_DOUBLE_SPEED)
                        {
                            REG_UBRRL = BAUD_250000_FOR_16_MHZ_DOUBLE_SPEED;
                        }   
                        else
                        {
                            REG_UBRRL = BAUD_250000_FOR_16_MHZ_NORMAL_SPEED;
                        }
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                switch(ptr_struct_uart_configurations->enum_clock_polarity)
                {
                    case NO_CLOCK_USED:
                    {
                        /*Do Nothing. As this is the default Configurations.*/
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_UCPOL_BIT);
                        break;
                    }
                    case INVERTED_CLOCK:
                    {
                        SET_BIT(REG_UCSRC, REG_UCSRC_UCPOL_BIT);
                        break;
                    }
                    case NON_INVERTED_CLOCK:
                    {
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_UCPOL_BIT);
                        break;
                    }
                    default:
                    {
                        /*Do Nothing. Because this HAL Will Not Support the USART. It will Support only the UART.*/
                        break;
                    }
                }

                switch(ptr_struct_uart_configurations->enum_interrupt_usage)
                {
                    case NO_INTERRUPT:
                    {
                        /*Do Nothing. As this is the default Configurations.*/
                        break;
                    }
                    case RX_DONE_INTERRUPT:
                    {
                        SET_BIT(REG_UCSRB, REG_UCSRB_RXCIE_BIT);
                        break;
                    }
                    case TX_DONE_INTERRUPT:
                    {
                        SET_BIT(REG_UCSRB, REG_UCSRB_TXCIE_BIT);
                        break;
                    }
                    case EMPTY_UDR_INTERRUPT:
                    {
                        SET_BIT(REG_UCSRB, REG_UCSRB_UDRIE_BIT);
                        break;
                    }
                    case BOTH_TX_RX_INTERRUPT:
                    {
                        SET_BIT(REG_UCSRB, REG_UCSRB_TXCIE_BIT);
                        SET_BIT(REG_UCSRB, REG_UCSRB_RXCIE_BIT);
                        break;
                    }
                    default:
                    {
                        /*Do Nothing. Because this HAL Will not support the Interrupt*/
                        break;
                    }
                }

                switch(ptr_struct_uart_configurations->enum_parity_mode)
                {
                    case NO_PARITY_USED:
                    {
                        break;
                    }
                    case PARITY_ODD:
                    {
                        SET_BIT(REG_UCSRC, REG_UCSRC_UPM0_BIT);
                        SET_BIT(REG_UCSRC, REG_UCSRC_UPM1_BIT);
                        break;
                    }
                    case PARITY_EVEN:
                    {
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_UPM0_BIT);
                        SET_BIT(REG_UCSRC, REG_UCSRC_UPM1_BIT);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                
                switch(ptr_struct_uart_configurations->enum_stop_bits)
                {
                    case STOP_BIT_1:
                    {
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_USBS_BIT);
                        break;
                    }
                    case STOP_BIT_2:
                    {
                        SET_BIT(REG_UCSRC, REG_UCSRC_USBS_BIT);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }

                switch (ptr_struct_uart_configurations->enum_supported_role)
                {
                    case TX_ROLE_ENABLED:
                    {
                        SET_BIT(REG_UCSRB, REG_UCSRB_RXEN_BIT);
                        break;
                    }
                    case RX_ROLE_ENABLED:
                    {
                        SET_BIT(REG_UCSRB, REG_UCSRB_TXEN_BIT);
                        break;
                    }
                    case BOTH_TX_RX_ROLES_ENABLED:
                    {
                        SET_BIT(REG_UCSRB, REG_UCSRB_RXEN_BIT);
                        SET_BIT(REG_UCSRB, REG_UCSRB_TXEN_BIT);
                        break;
                    }
                    
                    default:
                    {
                        break;
                    }

                }

                switch(ptr_struct_uart_configurations->enum_data_frame_length)
                {
                    case DATA_LENGTH_5_BITS:
                    {
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_UCSZ0_BIT);
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_UCSZ1_BIT);
                        CLEAR_BIT(REG_UCSRB, REG_UCSRB_UCSZ2_BIT);
                        break;
                    }
                    case DATA_LENGTH_6_BITS:
                    {
                        SET_BIT(REG_UCSRC, REG_UCSRC_UCSZ0_BIT);
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_UCSZ1_BIT);
                        CLEAR_BIT(REG_UCSRB, REG_UCSRB_UCSZ2_BIT);
                        break;
                    }   
                    case DATA_LENGTH_7_BITS:
                    {
                        CLEAR_BIT(REG_UCSRC, REG_UCSRC_UCSZ0_BIT);
                        SET_BIT(REG_UCSRC, REG_UCSRC_UCSZ1_BIT);
                        CLEAR_BIT(REG_UCSRB, REG_UCSRB_UCSZ2_BIT);
                        break;
                    }
                    case DATA_LENGTH_8_BITS:
                    {
                        SET_BIT(REG_UCSRC, REG_UCSRC_UCSZ0_BIT);
                        SET_BIT(REG_UCSRC, REG_UCSRC_UCSZ1_BIT);
                        CLEAR_BIT(REG_UCSRB, REG_UCSRB_UCSZ2_BIT);
                        break;
                    }
                    case DATA_LENGTH_9_BITS:
                    {
                        SET_BIT(REG_UCSRC, REG_UCSRC_UCSZ0_BIT);
                        SET_BIT(REG_UCSRC, REG_UCSRC_UCSZ1_BIT);
                        SET_BIT(REG_UCSRB, REG_UCSRB_UCSZ2_BIT);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
                global_ptr_usart_database           = ptr_struct_uart_configurations;
                global_uint8_is_module_initialized  = TRUE;
            }
            else
            {
                /*This shall be assigned to ERROR_INVALID_ARGUMENTS. But it's assigned to another error to differentiate between the errors.*/
                sint32_retval = ERROR_OUT_OF_BOUNDARY;
            }
        }
        else
        {
            sint32_retval = ERROR_INVALID_ARGUMENTS;
        }
    }
    else
    {
        sint32_retval = ERROR_MODULE_ALREADY_INITAILIZED;
    }
    return sint32_retval;
}

/**
 * @brief:      Function to un-initialize the USART Peripheral And Clear its Configurations
 * 
 * @return      SUCCESS_RETVAL:                     In case of Success
 *              ERROR_MODULE_NOT_INITAILIZED:       In case that the module is not initialized previously.
 */
sint32_type uart_deinit(void)
{

}

/**
 * @brief:      Function that is used to send a Single Byte over the USART
 * 
 * @param[in]   sint8_data:                     The data that the user wants to send in ASCII Representation
 * @return      SUCCESS_RETVAL:                 In case of Success
 *              ERROR_MODULE_NOT_INITAILIZED:   In case that the module is not initialized.
 *              ERROR_MODULE_BUSY:              In case that there is another send operation in progress.
 */
sint32_type uart_send_byte(sint8_type sint8_data)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if(global_uint8_is_module_initialized == TRUE)
    {
        if(global_uint8_is_send_byte_in_progress == FALSE)
        {
            global_uint8_is_send_byte_in_progress = TRUE;
            REG_UDR = sint8_data;
            /*Wait until the Bit Is cleared. Ie, It's written to 1*/
            while(GET_BIT(REG_UCSRA, REG_UCSRA_TXC_BIT) == 0);
            global_uint8_is_send_byte_in_progress = FALSE;
        }
        else
        {
            sint32_retval = ERROR_MODULE_BUSY;
        }
    }
    else
    {
        sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
    }
    return sint32_retval;
}

/**
 * @brief:      Function that is used to Receive a Single Byte over the USART
 * 
 * @param[out]  psint8_data:                    Pointer to a variable at which The received data will be stored. @note: The received data will be in ASCII Representation
 * @return      SUCCESS_RETVAL:                 In case of Success
 *              ERROR_MODULE_NOT_INITAILIZED:   In case that the module is not initialized.
 *              ERROR_INVALID_ARGUMENTS:        In case that the passed arguments are not valid.
 */
sint32_type uart_receive_byte(sint8_type* psint8_data)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if(global_uint8_is_module_initialized == TRUE)
    {
        if(psint8_data != NULL_PTR)
        {
            if(global_uint8_is_send_byte_in_progress == FALSE)
            {
                while(GET_BIT(REG_UCSRA, REG_UCSRA_RXC_BIT) == 0);
                *psint8_data = REG_UDR;
            }
            else
            {
                sint32_retval = ERROR_MODULE_BUSY;
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
 * @brief:      Function that is used to send A Buffer over the USART.
 * 
 * @param[in]   psint8_buffer:                  Pointer to the buffer. The Data Representation shall be ASCII Representation. 
 * @param[in]   uint16_buffer_length:           The Buffer Length that will be sent over the USART. 
 * @return      SUCCESS_RETVAL:                 In case of Success
 *              ERROR_INVALID_ARGUMENTS:        In case that the passed arguments are not valid.
 *              ERROR_MODULE_NOT_INITAILIZED:   In case that the module is not initialized.
 *              ERROR_MODULE_BUSY:              In case that there is another send operation in progress.
 */
sint32_type uart_send_buffer(sint8_type* psint8_buffer, uint16_type uint16_buffer_length)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    uint16_type uint16_counter;
    if(global_uint8_is_module_initialized == TRUE)
    {
        if((psint8_buffer != NULL_PTR) && (uint16_buffer_length > 0))
        {
            if((global_uint8_is_send_buffer_in_progress == TRUE) || (global_uint8_is_send_byte_in_progress == TRUE))
            {
                sint32_retval = ERROR_MODULE_BUSY;
            }
            else
            {
                global_uint8_is_send_buffer_in_progress = TRUE;
                for(uint16_counter = 0; uint16_counter < uint16_buffer_length; uint16_counter++)
                {
                    sint32_retval = uart_send_byte(psint8_buffer[uint16_counter]);
                    if(sint32_retval != SUCCESS_RETVAL)
                    {
                        break;
                    }
                }
                global_uint8_is_send_buffer_in_progress = FALSE;
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
