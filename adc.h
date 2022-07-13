#ifndef __ADC_H__
#define __ADC_H__

#include "types.h"

/**
 * @brief: Function used to initialize the adc peripheral. 
 * 
 * @return sint32_type: SUCCESS_RETVAL in case of First time initialization.
 *                       ERROR_MODULE_ALREADY_INITAILIZED in case of initialized before.
 */
sint32_type adc_init(void);

/**
 * @brief: Function that is used to enable the ADC Peripheral.
 * 
 * @return sint32_type: SUCCESS_RETVAL in case of success.
 *                      ERROR_MODULE_NOT_INITAILIZED in case of not call the @ref: adc_init before.
 *  
 */
sint32_type adc_enable(uint8_type uint8_channel);

/**
 * @brief: Function that is used to disable the ADC Peripheral.
 * 
 * @return sint32_type: SUCCESS_RETVAL in case of success.
 *                      ERROR_MODULE_NOT_INITAILIZED in case of not call the @ref: adc_init before.
 *  
 */
sint32_type adc_disable(void);

/**
 * @brief: Function that is used to read the value of a conversion.
 * 
 * @param[in] pu16_adc_value:   Address of a variable in which the value will be stored.
 * @return sint32_type:         SUCCESS_RETVAL in case of success.
 *                      ERROR_MODULE_NOT_INITAILIZED in case of not call the @ref: adc_init before.
 *                      ERROR_INVALID_ARGUMENTS in case of that the @param: pu16_adc_value is NULL_POINTER 
 */
sint32_type adc_read_value(uint16_type* pu16_adc_value);

#endif  /*__ADC_H__*/