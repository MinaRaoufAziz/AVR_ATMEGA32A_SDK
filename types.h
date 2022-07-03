/**
*   @file:      types.h
*   @brief:     Wrapping the normal types to specific Definition to be Hardware Target Independent.
*   @author:    Mina Raouf
*/

#ifndef __TYPES_H__
#define __TYPES_H__

#define NULL_PTR            (void*) 0
#define FALSE               0
#define TRUE                1


typedef char                sint8_type;
typedef unsigned char       uint8_type;
typedef short               sint16_type;
typedef unsigned short      uint16_type;
typedef long int            sint32_type;
typedef unsigned long int   uint32_type;
typedef float               fint32_type;
typedef double              fint64_type;

#endif /*__TYPES_H__*/
