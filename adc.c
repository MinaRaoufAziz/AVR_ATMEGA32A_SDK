#include "adc.h"
#include "errors.h"
#include "atmega32a_register_file.h"
#include "bit_manipulation.h"
#include "gpio.h"

#define ADC_FALSE				0
#define ADC_TRUE				1
#define REFS0                   6   /*Bit Index inside the REG_ADMUX*/
#define ADLAR                   5   /*Bit Index inside the REG_ADMUX*/
#define ADATE                   5   /*Bit Index inside the REG_ADDCSRA*/
#define ADEN                    7   /*Bit Index inside the REG_ADDCSRA*/
#define ADSC                    6   /*Bit Index inside the REG_ADDCSRA*/
#define ADIF                    4   /*Bit Index inside the REG_ADDCSRA*/

/**
 * @brief : Description of the @value: Assigned in @ref: REG_ADMUX_6_BITS_MASK
 * 
|---------------------------------------------------------------|
| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| N/A   | N/A   | 1     | 1     | 1     | 1     |  1    | 1     |
|---------------------------------------------------------------|
*/
#define REG_ADMUX_6_BITS_MASK   0x3F

/**
 * @brief : Description of the @value: Assigned in @ref: REG_ADCSRA_3_BITS_MASK
 * 
|---------------------------------------------------------------|
| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| 0     | 0     | 0     | 0     | 0     | 1     |  1    | 1     |
|---------------------------------------------------------------|
*/
#define REG_ADCSRA_3_BITS_MASK   0x07

/**
 * @brief : Description of the @value: Assigned in @ref: REG_ADCSRA_3_BITS_MASK
 * 
|---------------------------------------------------------------|
| Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 |
| 0     | 0     | 0     | 1     | 1     | 1     |  1    | 1     |
|---------------------------------------------------------------|
*/
#define REG_ADCSRA_5_BITS_MASK   0x1F

static uint8_type guint8_is_module_initialized = ADC_FALSE;
/**
 * @brief: Function used to initialize the adc peripheral. 
 * 
 * @return sint32_type: SUCCESS_RETVAL in case of First time initialization.
 *                       ERROR_MODULE_ALREADY_INITAILIZED in case of initialized before.
 */
sint32_type adc_init(void)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if(guint8_is_module_initialized == ADC_FALSE)
    {
        /*Select Voltage Reference*/
        /*Note: You can use SETBIT(REG_ADMUX, REFS0) will do the same result.*/
        REG_ADMUX = (1<<REFS0) | (REG_ADMUX & 0x3F);
        
        /*Select Adjustment (Right Adjustment)*/
        CLEAR_BIT(REG_ADMUX, ADLAR);    /*Default Value assigned in the register.*/
        
        /*Select Prescalar (128 Prescalar)*/
        REG_ADCSRA = REG_ADCSRA_3_BITS_MASK;

        /*Start of ADC Auto Trigger Enable*/
        SET_BIT(REG_ADCSRA, ADATE);

        /*Conversion Trigger Source*/
        REG_SFIOR &=REG_ADCSRA_5_BITS_MASK;
        
        guint8_is_module_initialized = ADC_TRUE;
    }
    else
    {
        sint32_retval = ERROR_MODULE_ALREADY_INITAILIZED;
    }
    return sint32_retval;
}

/**
 * @brief: Function that is used to enable the ADC Peripheral.
 * 
 * @return sint32_type: SUCCESS_RETVAL in case of success.
 *                      ERROR_MODULE_NOT_INITAILIZED in case of not call the @ref: adc_init before.
 *  
 */
sint32_type adc_enable(uint8_type uint8_channel)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if(guint8_is_module_initialized == ADC_TRUE)
    {
        /*Enable ADC*/
        SET_BIT(REG_ADCSRA, ADEN);
        /*Start a conversion in ADC*/
        SET_BIT(REG_ADCSRA, ADSC);

        REG_ADMUX = (uint8_channel) | (REG_ADMUX & 0xE0);
    }
    else
    {
        sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
    }
    return sint32_retval;
}

/**
 * @brief: Function that is used to disable the ADC Peripheral.
 * 
 * @return sint32_type: SUCCESS_RETVAL in case of success.
 *                      ERROR_MODULE_NOT_INITAILIZED in case of not call the @ref: adc_init before.
 *  
 */
sint32_type adc_disable(void)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if(guint8_is_module_initialized == ADC_TRUE)
    {
        /*Disable ADC*/
        CLEAR_BIT(REG_ADCSRA, ADEN);
    }
    else
    {
        sint32_retval = ERROR_MODULE_NOT_INITAILIZED;
    }
    return sint32_retval;
}

/**
 * @brief: Function that is used to read the value of a conversion.
 * 
 * @param[in] pu16_adc_value:   Address of a variable in which the value will be stored.
 * @return sint32_type:         SUCCESS_RETVAL in case of success.
 *                      ERROR_MODULE_NOT_INITAILIZED in case of not call the @ref: adc_init before.
 *                      ERROR_INVALID_ARGUMENTS in case of that the @param: pu16_adc_value is NULL_POINTER 
 */
sint32_type adc_read_value(uint16_type* pu16_adc_value)
{
    sint32_type sint32_retval = SUCCESS_RETVAL;
    if(guint8_is_module_initialized == ADC_TRUE)
    {
        if(pu16_adc_value != NULL_PTR)
        {

            /*Start a conversion in ADC*/
            SET_BIT(REG_ADCSRA, ADSC);

            /*Wait until the conversion is done*/
            while(GET_BIT(REG_ADCSRA, ADIF) == 0);

            /*Store The Value inside the passed variable.*/
            *pu16_adc_value = REG_ADCL;
			
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