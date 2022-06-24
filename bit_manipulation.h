/*bit_manipulation.h*/

#ifndef __BIT_MANIPULATION_H__
#define __BIT_MANIPULATION_H__

#define SET_BIT(VAR, BIT_IDX)				(VAR |= (1<<BIT_IDX))
#define CLEAR_BIT(VAR, BIT_IDX)				(VAR &= (~(1<<BIT_IDX)))
#define GET_BIT(VAR, BIT_IDX)				(((VAR & (1<<BIT_IDX)) >>BIT_IDX))
#define TOGGLE_BIT(VAR, BIT_IDX)			(VAR ^= (1<<BIT_IDX))
#define SET_PORT(VAR)          				(VAR |= 0xFF)
#define CLEAR_PORT(VAR)          			(VAR &= 0x00)
#define TOGGLE_PORT(VAR)          			(VAR ^= 0xFF)

#endif  /*__BIT_MANIPULATION_H__*/
