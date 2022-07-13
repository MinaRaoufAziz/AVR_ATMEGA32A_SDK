/**
 * @file:           timer.h
 * @author:         Mina Raouf
 * @brief:          This file contains the API(s) Declaration and the needed types for the upper layers in order to use the TIMER HAL
 */

#ifndef __TIMERS_H__
#define __TIMERS_H__

#include "types.h"

/**
 * 
 * @enum: 	enum_timer_index_type
 * @brief: 	Enumeration that points to each timer index.
 * 			This will be used in the initialization sequence to check if this timer is initialized previously or not.
 * 
 */
typedef enum
{
	TIMER_INDEX_0	= 0,
	TIMER_INDEX_1,
	TIMER_INDEX_2,
	TIMER_INDEX_INVALID
}enum_timer_index_type;

/**
 * 
 * @enum: 	enum_timer_interrupt_usage_type
 * @brief: 	Enumeration that Illustrates each timer interrupt status
 * 
 */
typedef enum
{
	TIMER_USAGE_INTERRUPT = 0,
	TIMER_USAGE_POLLING,
	TIMER_USAGE_INVALID
}enum_timer_interrupt_usage_type;

/**
 * 
 * @enum: 	enum_timer_mode_type
 * @brief: 	Enumeration that Indicates each timer mode of operation.
 * 
 */
typedef enum
{
	TIMER_MODE_OVF = 0,
	TIMER_MODE_CTC,
	TIMER_MODE_PWM,
	TIMER_MODE_PHASE_PWM,
	TIMER_MODE_INVALID
}enum_timer_mode_type;

/**
 * @enum:	enum_timer_prescalar_value_type
 * @brief:	Enumeration Describe the supported values of Prescalar for internal clock usage.
 * 
 */
typedef enum
{
	TIMER_NO_PRESCALAR		= 1,
	TIMER_8_PRESCALAR		= 8,
	TIMER_64_PRESCALAR		= 64,
	TIMER_256_PRESCALAR		= 256,
	TIMER_1024_PRESCALAR	= 1024,
	TIMER_INVALID_PRESCALAR
}enum_timer_prescalar_value_type;
/**
 * 
 * @typedef: 	pointer_func_timer_callback_type
 * @brief: 		Defining of the type of Generic Timers callback.
 * 
 */
typedef void (*pointer_func_timer_callback_type)(enum_timer_index_type enum_timer_index);

/**
 * 
 * @struct: 	tstr_timer_config
 * @brief: 		Structure that contains all the needed parameters to configure each timer.
 * 
 */

typedef struct
{
	enum_timer_index_type				enum_timer_index;
	enum_timer_interrupt_usage_type		enum_timer_interrupt_usage;
	enum_timer_mode_type				enum_timer_mode;
	enum_timer_prescalar_value_type		enum_timer_prescalar_value;
	pointer_func_timer_callback_type	pointer_func_timer_callback;
}tstr_timer_config;

/**
 * @brief: 		Function that is used to initialize each timer.
 * 
 * @param[in] 	pstr_timer_config: 	Constant Pointer to Constant Structure that contains the Required Timer Configuration
 * @return:		SUCCESS_RETVAL:	in case of success.
 * 				ERROR_INVALID_ARGUMENTS: In case of Invalid Arguments Passed to the function.
 * 				ERROR_MODULE_ALREADY_INITAILIZED: In case that this timer is already initialized previously. 
 */
sint32_type timer_init(const tstr_timer_config* const pstr_timer_config);

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
sint32_type timer_deinit(enum_timer_index_type enum_timer_index);

/**
 * @brief: 		Function that is used to start the timer by enabling the clock signal to it.
 * 
 * @param[in] 	enum_timer_index: 	Index of the Enumeration that indicates which timer to enable.
 * @return:		SUCCESS_RETVAL:	in case of success, or in case that the timer is already enabled. Ie, this API was called previously for this specific timer index.
 * 				ERROR_INVALID_ARGUMENTS: In case of Invalid Arguments Passed to the function.
 * 				ERROR_MODULE_NOT_INITAILIZED: In case that this timer is not configured yet. Ie, the initialization is not done yet for this specific index. 
 */
sint32_type timer_enable(enum_timer_index_type enum_timer_index);

/**
 * @brief: 		Function that is used to stop the timer by disabling the clock signal to it.
 * 
 * @param[in]	enum_timer_index: 	Index of the Enumeration that indicates which timer to disable. 
 * @return 		SUCCESS_RETVAL: In case of success, or the timer is already disabled (This API was called previously for this specific timer index), or that the @fn: timer_enable was not called previously for this specific timer. 
 *  			ERROR_INVALID_ARGUMENTS: In case of Invalid Arguments Passed to the function.
 * 				ERROR_MODULE_NOT_INITAILIZED: In case that this timer is not configured yet. Ie, the initialization is not done yet for this specific index. 
 */
sint32_type timer_disable(enum_timer_index_type enum_timer_index);

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
sint32_type timer_delay(enum_timer_index_type enum_timer_index, uint32_type uint32_delay_in_ms);

/**
 * @brief:		Function that is used to generate a PWM (Pulse Width Modulation) for the timer
 * 
 * @param[in] 	enum_timer_index: 	Enumeration for the specific timer index the user wants to generate a PWM on it. 
 * @param[in] 	uint8_duty_cycle:	Timer Duty Cycle the user wants to generate on the timer. 
 * @return 		SUCCESS_RETVAL: In case of success. 
 *  			ERROR_MODULE_NOT_INITAILIZED: In case that this timer is not configured yet. Ie, the initialization is not done yet for this specific index. 
 * 				ERROR_UNSUPPORTED_FEATURE: In case that this timer was not configured in the @fn: timer_init in the @member: enum_timer_mode_type in the @struct: tstr_timer_config was not equal to TIMER_MODE_PWM | TIMER_MODE_PHASE_PWM
 */
sint32_type	timer_generate_pwm(enum_timer_index_type enum_timer_index, uint8_type uint8_duty_cycle);

/**
 * @brief:		Function that is used to Handle all the timer events in the main loop
 * @return 		None
 */
void timer_dispatcher(void);

#endif 	/*__TIMERS_H__*/
