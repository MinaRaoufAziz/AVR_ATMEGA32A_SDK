/**
 * @file:           timer.c
 * @author:         Mina Raouf
 * @brief:          This file contains the implementation of the timers driver for ATMEGA32A.
 * @version:        1.0
 * @date:           2022-07-09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "timer.h"
#include "errors.h"
#include "gpio.h"
#include "atmega32a_register_file.h"
#include "bit_manipulation.h"
#include "lcd.h"
#include <avr/interrupt.h>

#ifndef F_CPU
#define F_CPU                                   8000000UL
#endif


#define MUL_FACTOR                              1000000         /*Factor to convert the F_CPU from Million to a single digit*/
#define CONVERT_FROM_MILLI_TO_MICRO             1000
#define COUNTS_TILL_OVERFLOW_IN_8_BIT_TIMER     256
#define OVERFLOW_VALUE_IN_8_BIT_TIMER           255
#define COUNTS_TILL_OVERFLOW_IN_16_BIT_TIMER    65536
#define OVERFLOW_VALUE_IN_16_BIT_TIMER          65535
#define ONE_MS                                  1
#define NUMBER_OF_SUPPORTED_TIMERS              3
#define FIND_ARR_LENGTH(ARR_NAME)               ((sizeof(ARR_NAME)) / sizeof(ARR_NAME[0]))
#define MIN_DELAY_VALUE                         0
#define MIN_DUTY_CYCLE_VALUE                    0
#define MAX_DUTY_CYCLE_VALUE                    100
#define I_BIT                                   7
#define TOIE0_BIT                               0
#define OCIE0_BIT                               1
#define WGM01_BIT                               3
#define WGM00_BIT                               6


/**
 * @brief: Sturcture that contains all the parameters needed to track the Timers configuration database.
 * 
 */
typedef struct
{
    uint8_type                          uint8_is_timer_enabled_in_db;
    enum_timer_mode_type                enum_timer_mode_in_db;
    enum_timer_index_type               enum_timer_index_in_db;
    enum_timer_prescalar_value_type		enum_timer_prescalar_value_in_db;
    pointer_func_timer_callback_type    pointer_func_timer_callback_in_db;
}struct_timer_database_type;

#if (0)
static struct_timer_database_type global_arr_str_timer_database[NUMBER_OF_SUPPORTED_TIMERS] =   {
                                                                                                    {
                                                                                                        .enum_timer_index               = TIMER_INDEX_INVALID,
                                                                                                        .pointer_func_timer_callback    = NULL_PTR
                                                                                                    }

                                                                                                    {
                                                                                                        .enum_timer_index               = TIMER_INDEX_INVALID,
                                                                                                        .pointer_func_timer_callback    = NULL_PTR
                                                                                                    }

                                                                                                    {
                                                                                                        .enum_timer_index               = TIMER_INDEX_INVALID,
                                                                                                        .pointer_func_timer_callback    = NULL_PTR
                                                                                                    }
                                                                                                };
#endif

static struct_timer_database_type   global_arr_str_timer_database[NUMBER_OF_SUPPORTED_TIMERS];
static uint8_type                   global_bool_need_to_init_database   = TRUE;
static volatile uint32_type         global_timer_0_int_count_for_one_ms = 0;
static volatile uint32_type         global_timer_0_needed_interrupts    = 0;
static volatile uint32_type         global_timer_1_needed_interrupts    = 0;
static volatile uint32_type         global_timer_2_needed_interrupts    = 0;

static void init_timers_database(void);
static sint32_type delay_timer_0(uint32_type uint32_delay_in_ms);
static sint32_type delay_timer_1(uint32_type uint32_delay_in_ms);
static sint32_type delay_timer_2(uint32_type uint32_delay_in_ms);
static sint32_type generate_pwm_timer_0(uint8_type uint8_duty_cycle);
static sint32_type generate_pwm_timer_1(uint8_type uint8_duty_cycle);
static sint32_type generate_pwm_timer_2(uint8_type uint8_duty_cycle);

ISR(TIMER0_OVF_vect)
{
    static volatile uint32_type local_interrupt_counter = 0;
    local_interrupt_counter++;

    if(local_interrupt_counter == global_timer_0_needed_interrupts)
    {
        local_interrupt_counter     = 0;
        if(global_arr_str_timer_database[TIMER_INDEX_0].pointer_func_timer_callback_in_db != NULL_PTR)
        {
            global_arr_str_timer_database[TIMER_INDEX_0].pointer_func_timer_callback_in_db(TIMER_INDEX_0);
        }
    }
}

ISR(TIMER0_COMP_vect)
{
	static volatile uint32_type local_interrupt_counter = 0;
	local_interrupt_counter++;

	if(local_interrupt_counter == global_timer_0_needed_interrupts)
	{
		local_interrupt_counter     = 0;
		if(global_arr_str_timer_database[TIMER_INDEX_0].pointer_func_timer_callback_in_db != NULL_PTR)
		{
			global_arr_str_timer_database[TIMER_INDEX_0].pointer_func_timer_callback_in_db(TIMER_INDEX_0);
		}
	}
}

static void init_timers_database(void)
{
    uint8_type uint8_iterator;
    for(uint8_iterator = 0; uint8_iterator < FIND_ARR_LENGTH(global_arr_str_timer_database); uint8_iterator++)
    {
        global_arr_str_timer_database[uint8_iterator].enum_timer_index_in_db                = TIMER_INDEX_INVALID;
        global_arr_str_timer_database[uint8_iterator].enum_timer_mode_in_db                 = TIMER_MODE_INVALID;
        global_arr_str_timer_database[uint8_iterator].enum_timer_prescalar_value_in_db      = TIMER_INVALID_PRESCALAR;
        global_arr_str_timer_database[uint8_iterator].pointer_func_timer_callback_in_db     = NULL_PTR;
        global_arr_str_timer_database[uint8_iterator].uint8_is_timer_enabled_in_db          = FALSE;

    }
}

static sint32_type delay_timer_0(uint32_type uint32_delay_in_ms)
{

    sint32_type sint32_retval                               = SUCCESS_RETVAL;
    uint8_type  uint8_preload_value                         = 0;
    uint32_type uint32_number_of_interrupts                 = 0;
    uint32_type uint32_time_to_overflow                     = 0;
    fint32_type float_tick_time_in_ns                       = 0;
    uint32_type uint32_tick_time_in_ns                       = 0;
    fint32_type float_exact_number_of_interrupts            = 0;

    if  (
            ((global_arr_str_timer_database[TIMER_INDEX_0].enum_timer_mode_in_db) == TIMER_MODE_OVF) ||
            ((global_arr_str_timer_database[TIMER_INDEX_0].enum_timer_mode_in_db) == TIMER_MODE_CTC)
        )
    {
        /*Put Timer 0 Delay Logic Here*/
        uint32_tick_time_in_ns = ((global_arr_str_timer_database[TIMER_INDEX_0].enum_timer_prescalar_value_in_db) * MUL_FACTOR) / (F_CPU / CONVERT_FROM_MILLI_TO_MICRO);
        uint32_time_to_overflow = uint32_tick_time_in_ns * COUNTS_TILL_OVERFLOW_IN_8_BIT_TIMER / CONVERT_FROM_MILLI_TO_MICRO;
        float_exact_number_of_interrupts = (uint32_delay_in_ms * CONVERT_FROM_MILLI_TO_MICRO) / uint32_time_to_overflow;
        uint32_number_of_interrupts = (uint32_type) float_exact_number_of_interrupts + 1;
        float_exact_number_of_interrupts = float_exact_number_of_interrupts - uint32_number_of_interrupts;
        global_timer_0_needed_interrupts = uint32_number_of_interrupts;
        uint8_preload_value = OVERFLOW_VALUE_IN_8_BIT_TIMER - (float_exact_number_of_interrupts * COUNTS_TILL_OVERFLOW_IN_8_BIT_TIMER);
        if((global_arr_str_timer_database[TIMER_INDEX_0].enum_timer_mode_in_db) == TIMER_MODE_OVF)
        {
            REG_TCNT0           = uint8_preload_value;
        }
        else if((global_arr_str_timer_database[TIMER_INDEX_0].enum_timer_mode_in_db) == TIMER_MODE_CTC)
        {
            REG_OCR0           = uint8_preload_value;
        }

    }
    else
    {
        sint32_retval = ERROR_UNSUPPORTED_FEATURE;
    }
    return sint32_retval;
}

static sint32_type delay_timer_1(uint32_type uint32_delay_in_ms)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    uint16_type uint16_preload_value                        = 0;
    uint32_type uint32_number_of_interrupts                 = 0;
    uint32_type uint32_time_to_overflow                     = 0;
    fint32_type float_tick_time_in_us                       = 0;
    fint32_type float_exact_number_of_interrupts            = 0;

    if  (
            ((global_arr_str_timer_database[TIMER_INDEX_1].enum_timer_mode_in_db) == TIMER_MODE_OVF) ||
            ((global_arr_str_timer_database[TIMER_INDEX_1].enum_timer_mode_in_db) == TIMER_MODE_CTC)
        )
    {
        /*Put Timer 1 Delay Logic Here*/
        float_tick_time_in_us = ((global_arr_str_timer_database[TIMER_INDEX_0].enum_timer_prescalar_value_in_db) * MUL_FACTOR) / (F_CPU);
        uint32_time_to_overflow = float_tick_time_in_us * COUNTS_TILL_OVERFLOW_IN_16_BIT_TIMER;
        float_exact_number_of_interrupts = (CONVERT_FROM_MILLI_TO_MICRO * uint32_delay_in_ms) / COUNTS_TILL_OVERFLOW_IN_16_BIT_TIMER;
        uint32_number_of_interrupts = (uint32_type) float_exact_number_of_interrupts;
        float_exact_number_of_interrupts -= uint32_number_of_interrupts;
        if(float_exact_number_of_interrupts > (fint32_type) 0.0)
        {
            uint32_number_of_interrupts++;
        }
        global_timer_0_needed_interrupts = uint32_number_of_interrupts;
        uint16_preload_value = OVERFLOW_VALUE_IN_16_BIT_TIMER - (float_exact_number_of_interrupts * COUNTS_TILL_OVERFLOW_IN_16_BIT_TIMER);
        REG_TCNT1L           = uint16_preload_value;
    }
    else
    {
        sint32_retval = ERROR_UNSUPPORTED_FEATURE;
    }
    return sint32_retval;
}

static sint32_type delay_timer_2(uint32_type uint32_delay_in_ms)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    uint8_type  uint8_preload_value                         = 0;
    uint32_type uint32_number_of_interrupts                 = 0;
    uint32_type uint32_time_to_overflow                     = 0;
    fint32_type float_tick_time_in_us                       = 0;
    fint32_type float_exact_number_of_interrupts            = 0;

    if  (
            ((global_arr_str_timer_database[TIMER_INDEX_2].enum_timer_mode_in_db) == TIMER_MODE_OVF) ||
            ((global_arr_str_timer_database[TIMER_INDEX_2].enum_timer_mode_in_db) == TIMER_MODE_CTC)
        )
    {
        /*Put Timer 2 Delay Logic Here*/
        float_tick_time_in_us = ((global_arr_str_timer_database[TIMER_INDEX_0].enum_timer_prescalar_value_in_db) * MUL_FACTOR) / (F_CPU);
        uint32_time_to_overflow = float_tick_time_in_us * COUNTS_TILL_OVERFLOW_IN_8_BIT_TIMER;
        float_exact_number_of_interrupts = (CONVERT_FROM_MILLI_TO_MICRO * uint32_delay_in_ms) / COUNTS_TILL_OVERFLOW_IN_8_BIT_TIMER;
        uint32_number_of_interrupts = (uint32_type) float_exact_number_of_interrupts;
        float_exact_number_of_interrupts -= uint32_number_of_interrupts;
        if(float_exact_number_of_interrupts > (fint32_type) 0)
        {
            uint32_number_of_interrupts++;
        }
        global_timer_0_needed_interrupts = uint32_number_of_interrupts;
        uint8_preload_value = float_exact_number_of_interrupts * COUNTS_TILL_OVERFLOW_IN_8_BIT_TIMER;
        REG_TCNT2           = uint8_preload_value;
    }
    else
    {
        sint32_retval = ERROR_UNSUPPORTED_FEATURE;
    }
    return sint32_retval;
}

static sint32_type generate_pwm_timer_0(uint8_type uint8_duty_cycle)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if  (
            ((global_arr_str_timer_database[TIMER_INDEX_2].enum_timer_mode_in_db) == TIMER_MODE_PWM) ||
            ((global_arr_str_timer_database[TIMER_INDEX_2].enum_timer_mode_in_db) == TIMER_MODE_PHASE_PWM)
        )
    {
        /*Put Timer 0 PWM Generation Logic Here*/
    }
    else
    {
        sint32_retval = ERROR_UNSUPPORTED_FEATURE;
    }
    return sint32_retval;
}
static sint32_type generate_pwm_timer_1(uint8_type uint8_duty_cycle)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if  (
            ((global_arr_str_timer_database[TIMER_INDEX_2].enum_timer_mode_in_db) == TIMER_MODE_PWM) ||
            ((global_arr_str_timer_database[TIMER_INDEX_2].enum_timer_mode_in_db) == TIMER_MODE_PHASE_PWM)
        )
    {
        /*Put Timer 1 PWM Generation Logic Here*/
    }
    else
    {
        sint32_retval = ERROR_UNSUPPORTED_FEATURE;
    }
    return sint32_retval;
}
static sint32_type generate_pwm_timer_2(uint8_type uint8_duty_cycle)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if  (
            ((global_arr_str_timer_database[TIMER_INDEX_2].enum_timer_mode_in_db) == TIMER_MODE_PWM) ||
            ((global_arr_str_timer_database[TIMER_INDEX_2].enum_timer_mode_in_db) == TIMER_MODE_PHASE_PWM)
        )
    {
        /*Put Timer 2 PWM Generation Logic Here*/
    }
    else
    {
        sint32_retval = ERROR_UNSUPPORTED_FEATURE;
    }
    return sint32_retval;
}

/**
 * @brief: 		Function that is used to initialize each timer.
 * 
 * @param[in] 	pstr_timer_config: 	Constant Pointer to Constant Structure that contains the Required Timer Configuration
 * @return:		SUCCESS_RETVAL:	in case of success.
 * 				ERROR_INVALID_ARGUMENTS: In case of Invalid Arguments Passed to the function.
 * 				ERROR_MODULE_ALREADY_INITAILIZED: In case that this timer is already initialized previously. 
 */
sint32_type timer_init(const tstr_timer_config* const pstr_timer_config)
{
    sint32_type sint32_retval                           = SUCCESS_RETVAL;
    uint8_type  uint8_need_to_save_in_timer_database    = TRUE;
    if(pstr_timer_config != NULL_PTR)
    {
        /*  This is done to assure that the timer driver local database is initialized one time only. 
            To avoid corrupting the previous intialized data upon multiple calling to the timer_init function with different configurations.
        */
        if(global_bool_need_to_init_database == TRUE)
        {
            init_timers_database();
            global_bool_need_to_init_database = FALSE;
        }

        /*Input Parameters check. This to assure that all the parameters passed are right parameters to operate on.*/
        if  (
                ((pstr_timer_config->enum_timer_index) < TIMER_INDEX_INVALID) &&
                ((pstr_timer_config->enum_timer_interrupt_usage) < TIMER_USAGE_INVALID) &&
                ((pstr_timer_config->enum_timer_mode) < TIMER_MODE_INVALID) &&
                ((pstr_timer_config->pointer_func_timer_callback) != NULL_PTR)
            )
        {
            if  (
                    ((global_arr_str_timer_database[pstr_timer_config->enum_timer_index].uint8_is_timer_enabled_in_db) == FALSE)                        && 
                    ((global_arr_str_timer_database[pstr_timer_config->enum_timer_index].enum_timer_index_in_db) == TIMER_INDEX_INVALID)                && 
                    ((global_arr_str_timer_database[pstr_timer_config->enum_timer_index].enum_timer_mode_in_db) == TIMER_MODE_INVALID)                  && 
                    ((global_arr_str_timer_database[pstr_timer_config->enum_timer_index].enum_timer_prescalar_value_in_db) == TIMER_INVALID_PRESCALAR)  && 
                    ((global_arr_str_timer_database[pstr_timer_config->enum_timer_index].pointer_func_timer_callback_in_db) == NULL_PTR)
                )
            {
                /*This means that this timer has no saved data in the database. So it's not initialized previously.*/
                switch(pstr_timer_config->enum_timer_index)
                {
                    case TIMER_INDEX_0:
                    {
                        if((pstr_timer_config->enum_timer_interrupt_usage) == TIMER_USAGE_INTERRUPT)
                        {
                            /*Put Timer 0 Interrupt Configurations here.*/
                            if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_OVF)
                            {
                                /*Enable Global Interrupt*/
                                SET_BIT(REG_SREG, I_BIT);   
                                /*Enable Peripheral Interrupt*/
                                SET_BIT(REG_TIMSK, TOIE0_BIT);
                            }
                            else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_CTC)
                            {
                                /*Enable Global Interrupt*/
                                SET_BIT(REG_SREG, I_BIT);   
                                /*Enable Peripheral Interrupt*/
                                SET_BIT(REG_TIMSK, OCIE0_BIT);   
                            }
                        }
                        else if((pstr_timer_config->enum_timer_interrupt_usage) == TIMER_USAGE_POLLING)
                        {
                            /*Do Nothing.*/
                        }

                        if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_OVF)
                        {
                            /*Put Timer 0 Overflow configurations here.*/
                            CLEAR_BIT(REG_TCCR0, WGM00_BIT);
                            CLEAR_BIT(REG_TCCR0, WGM01_BIT);
                        }
                        else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_CTC)
                        {
                            /*Put Timer 0 Clear Timer on Compare Match configurations here.*/
                            CLEAR_BIT(REG_TCCR0, WGM00_BIT);
                            SET_BIT(REG_TCCR0, WGM01_BIT);
                        }
                        else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_PWM)
                        {
                            /*Put Timer 0 Pulse Width Modulation configurations here.*/
                            SET_BIT(REG_TCCR0, WGM00_BIT);
                            SET_BIT(REG_TCCR0, WGM01_BIT);
                        }
                        else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_PHASE_PWM)
                        {
                            /*Put Timer 0 Phase Correct Pulse Width Modulation configurations here.*/
                            SET_BIT(REG_TCCR0, WGM00_BIT);
                            CLEAR_BIT(REG_TCCR0, WGM01_BIT);
                        }

                        break;
                    }

                    case TIMER_INDEX_1:
                    {
                        if((pstr_timer_config->enum_timer_interrupt_usage) == TIMER_USAGE_INTERRUPT)
                        {
                            /*Put Timer 1 Interrupt Configurations here.*/
                        }
                        else if((pstr_timer_config->enum_timer_interrupt_usage) == TIMER_USAGE_POLLING)
                        {
                            /*Do Nothing.*/
                        }

                        if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_OVF)
                        {
                            /*Put Timer 1 Overflow configurations here.*/
                        }
                        else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_CTC)
                        {
                            /*Put Timer 1 Clear Timer on Compare Match configurations here.*/
                        }
                        else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_PWM)
                        {
                            /*Put Timer 1 Pulse Width Modulation configurations here.*/
                        }
                        else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_PHASE_PWM)
                        {
                            /*Put Timer 1 Fast Pulse Width Modulation configurations here.*/
                        }
                        break;
                    }

                    case TIMER_INDEX_2:
                    {
                        if((pstr_timer_config->enum_timer_interrupt_usage) == TIMER_USAGE_INTERRUPT)
                        {
                            /*Put Timer 2 Interrupt Configurations here.*/
                        }
                        else if((pstr_timer_config->enum_timer_interrupt_usage) == TIMER_USAGE_POLLING)
                        {
                            /*Do Nothing.*/
                        }

                        if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_OVF)
                        {
                            /*Put Timer 2 Overflow configurations here.*/
                        }
                        else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_CTC)
                        {
                            /*Put Timer 2 Clear Timer on Compare Match configurations here.*/
                        }
                        else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_PWM)
                        {
                            /*Put Timer 2 Pulse Width Modulation configurations here.*/
                        }
                        else if((pstr_timer_config->enum_timer_mode) == TIMER_MODE_PHASE_PWM)
                        {
                            /*Put Timer 2 Fast Pulse Width Modulation configurations here.*/
                        }
                        break;
                    }

                    default:
                    {
                        /*Unsupported Timer Index. We shall not save this parameters in the database.*/
                        uint8_need_to_save_in_timer_database = FALSE;
                        break;
                    }
                }

                if(uint8_need_to_save_in_timer_database == TRUE)
                {
                    /*Save the Passed Configurations in the Local Timer Database*/
                    global_arr_str_timer_database[pstr_timer_config->enum_timer_index].enum_timer_index_in_db               = pstr_timer_config->enum_timer_index;
                    global_arr_str_timer_database[pstr_timer_config->enum_timer_index].enum_timer_mode_in_db                = pstr_timer_config->enum_timer_mode;
                    global_arr_str_timer_database[pstr_timer_config->enum_timer_index].enum_timer_prescalar_value_in_db     = pstr_timer_config->enum_timer_prescalar_value;
                    global_arr_str_timer_database[pstr_timer_config->enum_timer_index].pointer_func_timer_callback_in_db    = pstr_timer_config->pointer_func_timer_callback;
                }
            }
            else
            {
                /*This means that this timer has a saved configuration parameters in the database. So it's initialized previously.*/
                sint32_retval = ERROR_MODULE_ALREADY_INITAILIZED;
            }
        }
        /*This shall be set to ERROR_INVALID_ARGUMENTS. But it's assigned to ERROR_UNSUPPORTED_FEATURE to differentiate the errors returned from this function.*/
        else
        {
            sint32_retval = ERROR_UNSUPPORTED_FEATURE;
        }
    }
    else
    {
        sint32_retval = ERROR_INVALID_ARGUMENTS;
    }
    return sint32_retval;
}

/**
 * @brief:		Function that is used to deinitialize a specific timer. 
 * 				This shall be used to re-initialize the timer with a new configurations by calling @fn: timer_init and pass the new configurations.
 * 				Or to erase the configurations of a specific timer and disable it.
 * 
 * @param[in] 	enum_timer_index: The timer index the user wants to uninitialize 
 * @return 		SUCCESS_RETVAL:	in case of success. 
 *				ERROR_INVALID_ARGUMENTS: In case of Invalid Arguments Passed to the function.
 * 				ERROR_MODULE_NOT_INITAILIZED: in case that this timer was not initialized previously.
 */
sint32_type timer_deinit(enum_timer_index_type enum_timer_index)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if(enum_timer_index < TIMER_INDEX_INVALID)
    {
        if  (                    
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_index_in_db) == TIMER_INDEX_INVALID)   && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_mode_in_db) == TIMER_MODE_INVALID)     && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_prescalar_value_in_db) == TIMER_INVALID_PRESCALAR)     && 
                ((global_arr_str_timer_database[enum_timer_index].pointer_func_timer_callback_in_db) == NULL_PTR)
            )
        {
            /*This means that this timer doesn't have a saved configuration parameters in the database. So it's not initialized previously.*/
            sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
        }
        else
        {
            global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db        = FALSE;
            global_arr_str_timer_database[enum_timer_index].enum_timer_index_in_db              = TIMER_INDEX_INVALID;
            global_arr_str_timer_database[enum_timer_index].enum_timer_mode_in_db               = TIMER_MODE_INVALID;
            global_arr_str_timer_database[enum_timer_index].enum_timer_prescalar_value_in_db    = TIMER_INVALID_PRESCALAR;
            global_arr_str_timer_database[enum_timer_index].pointer_func_timer_callback_in_db   = NULL_PTR;
        }
    }
    else
    {
        sint32_retval = ERROR_INVALID_ARGUMENTS;
    }
    return sint32_retval;
}

/**
 * @brief: 		Function that is used to start the timer by enabling the clock signal to it.
 * 
 * @param[in] 	enum_timer_index: 	Index of the Enumeration that indicates which timer to enable.
 * @return:		SUCCESS_RETVAL:	in case of success, or in case that the timer is already enabled. Ie, this API was called previously for this specific timer index.
 * 				ERROR_INVALID_ARGUMENTS: In case of Invalid Arguments Passed to the function.
 * 				ERROR_MODULE_NOT_INITAILIZED: In case that this timer is not configured yet. Ie, the initialization is not done yet for this specific index. 
 */
sint32_type timer_enable(enum_timer_index_type enum_timer_index)
{
    sint32_type sint32_retval                           = SUCCESS_RETVAL;
    uint8_type  uint8_need_to_save_in_timer_database    = TRUE;
    if(enum_timer_index < TIMER_INDEX_INVALID)
    {
        if  (                    
                ((global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db) == FALSE)                       && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_index_in_db) == TIMER_INDEX_INVALID)               && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_prescalar_value_in_db) == TIMER_INVALID_PRESCALAR) && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_mode_in_db) == TIMER_MODE_INVALID)                 && 
                ((global_arr_str_timer_database[enum_timer_index].pointer_func_timer_callback_in_db) == NULL_PTR)
            )
        {
            /*This means that this timer doesn't have a saved configuration parameters in the database. So it's not initialized previously.*/
            sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
        }
        else
        {
            if((global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db) == FALSE)
            {
                switch(enum_timer_index)
                {
                    case TIMER_INDEX_0:
                    {
                        /*Put the Sequence of Enabling Timer 0 Here*/
                        switch(global_arr_str_timer_database[enum_timer_index].enum_timer_prescalar_value_in_db)
                        {
                            case TIMER_NO_PRESCALAR:
                            {
                                REG_TCCR0 = REG_TCCR0 | 0x01;
                                break;
                            }
                            case TIMER_8_PRESCALAR:
                            {
                                REG_TCCR0 = REG_TCCR0 | 0x02;
                                break;
                            }
                            case TIMER_64_PRESCALAR:
                            {
                                REG_TCCR0 = REG_TCCR0 | 0x03;
                                break;
                            }
                            case TIMER_256_PRESCALAR:
                            {
                                REG_TCCR0 = REG_TCCR0 | 0x04;
                                break;
                            }
                            case TIMER_1024_PRESCALAR:
                            {
                                REG_TCCR0 = REG_TCCR0 | 0x05;
                                break;
                            }
                            default:
                            {
                                break;
                            }
                        }
                        
                        break;
                    }
                    case TIMER_INDEX_1:
                    {
                        /*Put the Sequence of Enabling Timer 1 Here*/
                        break;
                    }
                    case TIMER_INDEX_2:
                    {
                        /*Put the Sequence of Enabling Timer 2 Here*/
                        break;
                    }
                    default:
                    {
                        uint8_need_to_save_in_timer_database = FALSE;
                        /*This shall be set to ERROR_INVALID_ARGUMENTS. But it's assigned to ERROR_UNSUPPORTED_FEATURE to differentiate the errors returned from this function.*/
                        sint32_retval = ERROR_UNSUPPORTED_FEATURE;
                        break;
                    }
                }
                if(uint8_need_to_save_in_timer_database == TRUE)
                {
                    global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db = TRUE;
                }
            }
            else
            {
                /*Entering Here means that the database info is not saved right.*/
                sint32_retval = ERROR_INVALD_LENGTH;
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
 * @brief: 		Function that is used to stop the timer by disabling the clock signal to it.
 * 
 * @param[in]	enum_timer_index: 	Index of the Enumeration that indicates which timer to disable. 
 * @return 		SUCCESS_RETVAL: In case of success, or the timer is already disabled (This API was called previously for this specific timer index), or that the @fn: timer_enable was not called previously for this specific timer. 
 *  			ERROR_INVALID_ARGUMENTS: In case of Invalid Arguments Passed to the function.
 * 				ERROR_MODULE_NOT_INITAILIZED: In case that this timer is not configured yet. Ie, the initialization is not done yet for this specific index. 
 */
sint32_type timer_disable(enum_timer_index_type enum_timer_index)
{
    sint32_type sint32_retval                           = SUCCESS_RETVAL;
    uint8_type  uint8_need_to_save_in_timer_database    = TRUE;
    if(enum_timer_index < TIMER_INDEX_INVALID)
    {
        if  (
                ((global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db) == FALSE)           && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_index_in_db) == TIMER_INDEX_INVALID)   && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_mode_in_db) == TIMER_MODE_INVALID)     && 
                ((global_arr_str_timer_database[enum_timer_index].pointer_func_timer_callback_in_db) == NULL_PTR) 
            )
        {
            /*This means that this timer doesn't have a saved configuration parameters in the database. So it's not initialized previously.*/
            sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
        }
        else
        {
            if((global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db) == TRUE)
            {
                switch(enum_timer_index)
                {
                    case TIMER_INDEX_0:
                    {
                        /*Put the Sequence of Disabling Timer 0 Here*/
                        REG_TCCR0 = REG_TCCR0 & 0xF8;
                        break;
                    }
                    case TIMER_INDEX_1:
                    {
                        /*Put the Sequence of Disabling Timer 1 Here*/
                        break;
                    }
                    case TIMER_INDEX_2:
                    {
                        /*Put the Sequence of Disabling Timer 2 Here*/
                        break;
                    }
                    default:
                    {
                        uint8_need_to_save_in_timer_database = FALSE;
                        /*This shall be set to ERROR_INVALID_ARGUMENTS. But it's assigned to ERROR_UNSUPPORTED_FEATURE to differentiate the errors returned from this function.*/
                        sint32_retval = ERROR_UNSUPPORTED_FEATURE;
                        break;
                    }
                }
                if(uint8_need_to_save_in_timer_database == TRUE)
                {
                    global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db = FALSE;
                }
            }
            else
            {
                /*Entering Here means that the database info is not saved right.*/
                sint32_retval = ERROR_INVALD_LENGTH;
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
 * @brief:		Function that is used to set a specific delay value for a specific timer index.
 * @note:		The Delay will be done here is a single shot. 
 * 				This means that this delay will be done for one time only. After the timer finishes the delay value, it will execute the timer callback.
 * 
 * @param[in] 	enum_timer_index: 	Enumeration for the specific timer index the user wants to delay.
 * @param[in] 	uint32_delay_in_ms: The specific delay value in Milli Seconds.
 * @return 		SUCCESS_RETVAL: In case of success. 
 *  			ERROR_INVALID_ARGUMENTS: In case of Invalid Arguments Passed to the function.
 * 				ERROR_MODULE_NOT_INITAILIZED: In case that this timer is not configured yet. Ie, the initialization is not done yet for this specific index. 
 */
sint32_type timer_delay(enum_timer_index_type enum_timer_index, uint32_type uint32_delay_in_ms)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if((enum_timer_index < TIMER_INDEX_INVALID) && (uint32_delay_in_ms > MIN_DELAY_VALUE))
    {
        if  (                    
                ((global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db) == FALSE)           && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_index_in_db) == TIMER_INDEX_INVALID)   && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_mode_in_db) == TIMER_MODE_INVALID)     && 
                ((global_arr_str_timer_database[enum_timer_index].pointer_func_timer_callback_in_db) == NULL_PTR)
            )
        {
            /*This means that this timer doesn't have a saved configuration parameters in the database. So it's not initialized previously.*/
            sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
        }
        else
        {
            if((global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db) == FALSE)
            {
                switch(enum_timer_index)
                {
                    case TIMER_INDEX_0:
                    {
                        timer_enable(TIMER_INDEX_0);
                        // global_timer_0_needed_interrupts = uint32_delay_in_ms / CONVERT_FROM_MILLI_TO_MICRO;
                        sint32_retval = delay_timer_0(uint32_delay_in_ms);
                        break;
                    }
                    case TIMER_INDEX_1:
                    {
                        sint32_retval = delay_timer_1(uint32_delay_in_ms);
                        break;
                    }
                    case TIMER_INDEX_2:
                    {
                        sint32_retval = delay_timer_2(uint32_delay_in_ms);
                        break;
                    }
                    default:
                    {
                        sint32_retval = ERROR_OUT_OF_BOUNDARY;
                        break;
                    }
                }
            }
            else
            {
                sint32_retval = ERROR_UNSUPPORTED_FEATURE;
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
 * @brief:		Function that is used to generate a PWM (Pulse Width Modulation) for the timer
 * 
 * @param[in] 	enum_timer_index: 	Enumeration for the specific timer index the user wants to generate a PWM on it. 
 * @param[in] 	uint8_duty_cycle:	Timer Duty Cycle the user wants to generate on the timer. 
 * @return 		SUCCESS_RETVAL: In case of success. 
 *  			ERROR_MODULE_NOT_INITAILIZED: In case that this timer is not configured yet. Ie, the initialization is not done yet for this specific index. 
 * 				ERROR_UNSUPPORTED_FEATURE: In case that this timer was not configured in the @fn: timer_init in the @member: enum_timer_mode in the @struct: tstr_timer_config was not equal to TIMER_MODE_PWM | TIMER_MODE_PHASE_PWM
 */
sint32_type	timer_generate_pwm(enum_timer_index_type enum_timer_index, uint8_type uint8_duty_cycle)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if((enum_timer_index < TIMER_INDEX_INVALID) && (uint8_duty_cycle > MIN_DUTY_CYCLE_VALUE) && (uint8_duty_cycle < MAX_DUTY_CYCLE_VALUE))
    {
        if  (                    
                ((global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db) == FALSE)           && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_index_in_db) == TIMER_INDEX_INVALID)   && 
                ((global_arr_str_timer_database[enum_timer_index].enum_timer_mode_in_db) == TIMER_MODE_INVALID)     && 
                ((global_arr_str_timer_database[enum_timer_index].pointer_func_timer_callback_in_db) == NULL_PTR)
            )
        {
            /*This means that this timer doesn't have a saved configuration parameters in the database. So it's not initialized previously.*/
            sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
        }
        else
        {
            if((global_arr_str_timer_database[enum_timer_index].uint8_is_timer_enabled_in_db) == TRUE)
            {
                switch(enum_timer_index)
                {
                    case TIMER_INDEX_0:
                    {
                        sint32_retval = generate_pwm_timer_0(uint8_duty_cycle);
                        break;
                    }
                    case TIMER_INDEX_1:
                    {
                        sint32_retval = generate_pwm_timer_1(uint8_duty_cycle);
                        break;
                    }
                    case TIMER_INDEX_2:
                    {
                        sint32_retval = generate_pwm_timer_2(uint8_duty_cycle);
                        break;
                    }
                    default:
                    {
                        sint32_retval = ERROR_OUT_OF_BOUNDARY;
                        break;
                    }
                }
            }
            else
            {
                sint32_retval = ERROR_UNSUPPORTED_FEATURE;
            }
        }
    }
    else
    {
        sint32_retval = ERROR_INVALID_ARGUMENTS;
    }
    return sint32_retval;
}

void timer_dispatcher(void)
{

}