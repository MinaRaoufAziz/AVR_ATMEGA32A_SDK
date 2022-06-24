/*common.h*/

#ifndef __COMMON_H__ 
#define __COMMON_H__

#include "types.h"

/**
*@brief:
*/
#define BREAK_ON_ERROR(VAR)             if(VAR != SUCCESS_RETVAL)    \
                                        {                           \
                                            /*Todo: Add the compiler line here*/	\
                                            break;                  \
                                        }                                                    
/**
 *@fn: memory_set
 *@brief: This function is used to set a specific space in memory with a specific value.
 *@param [out]   ptr_char_mem:   pointer to the memory space you want to fill with the data.
 *@param [in]    sint_value:     value you want to fill inside the memory space
 *@param [in]    uint32_size:    size of memory space to fill with the value
 *@return       :: -1 in case of invalid arguments, 0 For Success
*/
sint32_type memory_set(char* ptr_char_mem, sint32_type sint_value, uint32_type uint32_size);

/**
 *@fn: memory_set_zero
 *@brief: This function is used to set a specific space in memory with zero.
 *@param [out]   ptr_char_mem:   pointer to the memory space you want to fill with the data.
 *@param [in]    uint32_size:    size of memory space to fill with 0
 *@return       :: -1 in case of invalid arguments, 0 For Success
*/
sint32_type memory_set_zero(char* ptr_char_mem, uint32_type uint32_size);

/**
 *@fn: memory_copy
 *@brief: This function is used to copy the content of a specific memory space inside another memory space.
 *@param [out]   ptr_char_dest_buff:    pointer to the memory space you want to fill with the data.
 *@param [in]    ptr_char_src_buff:     pointer to the memory space you want to take the data from.
 *@param [in]    uint32_size:           size of memory space to fill with the value
 *@return       :: -1 in case of invalid arguments, 0 For Success
*/
sint32_type memory_copy(char* ptr_char_dest_buff, char* ptr_char_src_buff, uint32_type uint32_buff_size);

/**
 *@fn: memory_compare
 *@brief: This function is used to set a specific space in memory with a specific value.
 *@param [out]   ptr_char_dest_buff:    pointer to the first memory space you want to compare.
 *@param [in]    ptr_char_src_buff:     pointer to the second memory space you want to compare.
 *@param [in]    uint32_size:           size of memory space to fill with the value
 *@return       :: -1 in case of invalid arguments, 0 For Success
*/
sint32_type memory_compare(char* ptr_char_dest_buff, char* ptr_char_src_buff, uint32_type uint32_buff_size);

#endif  /*__COMMON_H__*/
