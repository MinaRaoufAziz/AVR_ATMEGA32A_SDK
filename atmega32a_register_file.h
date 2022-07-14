/**
*   @file:      atmega32_register_file.h
*   @brief:     Definition of the ATMEGA32A Register Map Defined in Data Sheet.
*   @author:    Mina Raouf
*/

// #define ATMEGA_32A
#ifndef __ATMEGA32_REGISTER_FILE__
#define __ATMEGA32_REGISTER_FILE__

/**
    @note:
    This condition is added to assure that this file is included when ATMEGA_32A is defined only.
    If not, an #error message will appear to user to guard wrong Register file inclusion.
*/
#if defined (ATMEGA_32A)    
#include "types.h"

#define REG_TWBR            (*((volatile uint8_type*) 0x20)) /*Two-wire Serial Interface Bit Rate Register*/
#define REG_TWSR            (*((volatile uint8_type*) 0x21)) 
#define REG_TWAR           	(*((volatile uint8_type*) 0x22)) 
#define REG_TWDR           	(*((volatile uint8_type*) 0x23)) 
#define REG_ADC           	(*((volatile uint16_type*) 0x24)) 
#define REG_ADCSRA          (*((volatile uint8_type*) 0x26)) 
#define REG_ADMUX           (*((volatile uint8_type*) 0x27)) 
#define REG_ACSR           	(*((volatile uint8_type*) 0x28)) 
#define REG_UBRRL           (*((volatile uint8_type*) 0x29)) 
#define REG_UCSRB           (*((volatile uint8_type*) 0x2A)) 
#define REG_UCSRA           (*((volatile uint8_type*) 0x2B)) 
#define REG_UDR           	(*((volatile uint8_type*) 0x2C)) 
#define REG_SPCR           	(*((volatile uint8_type*) 0x2D)) 
#define REG_SPSR          	(*((volatile uint8_type*) 0x2E)) 
#define REG_SPDR           	(*((volatile uint8_type*) 0x2F)) 
#define REG_PIND           	(*((volatile uint8_type*) 0x30)) 
#define REG_DDRD           	(*((volatile uint8_type*) 0x31)) 
#define REG_PORTD           (*((volatile uint8_type*) 0x32)) 
#define REG_PINC           	(*((volatile uint8_type*) 0x33)) 
#define REG_DDRC           	(*((volatile uint8_type*) 0x34)) 
#define REG_PORTC           (*((volatile uint8_type*) 0x35)) 
#define REG_PINB           	(*((volatile uint8_type*) 0x36)) 
#define REG_DDRB           	(*((volatile uint8_type*) 0x37)) 
#define REG_PORTB           (*((volatile uint8_type*) 0x38)) 
#define REG_PINA           	(*((volatile uint8_type*) 0x39)) 
#define REG_DDRA           	(*((volatile uint8_type*) 0x3A)) 
#define REG_PORTA           (*((volatile uint8_type*) 0x3B)) 
#define REG_EECR           	(*((volatile uint8_type*) 0x3C)) 
#define REG_EEDR           	(*((volatile uint8_type*) 0x3D)) 
#define REG_EEARL           (*((volatile uint8_type*) 0x3E)) 
#define REG_EEARH           (*((volatile uint8_type*) 0x3F)) 
#define REG_UCSRC           (*((volatile uint8_type*) 0x40)) 
#define REG_UBRRH           (*((volatile uint8_type*) 0x40)) 
#define REG_WDTCR           (*((volatile uint8_type*) 0x41)) 
#define REG_ASSR           	(*((volatile uint8_type*) 0x42)) 
#define REG_OCR2           	(*((volatile uint8_type*) 0x43)) 
#define REG_TCNT2           (*((volatile uint8_type*) 0x44)) 
#define REG_TCCR2          	(*((volatile uint8_type*) 0x45)) 
#define REG_ICR1L           (*((volatile uint8_type*) 0x46)) 
#define REG_ICR1H           (*((volatile uint8_type*) 0x47)) 
#define REG_OCR1B           (*((volatile uint16_type*) 0x48)) 
#define REG_OCR1A           (*((volatile uint16_type*) 0x4A)) 
#define REG_TCNT1           (*((volatile uint16_type*) 0x4C)) 
#define REG_TCCR1B          (*((volatile uint8_type*) 0x4E)) 
#define REG_TCCR1A          (*((volatile uint8_type*) 0x4F)) 
#define REG_SFIOR           (*((volatile uint8_type*) 0x50)) 
#define REG_OCDR           	(*((volatile uint8_type*) 0x51)) 
#define REG_OSCCAL          (*((volatile uint8_type*) 0x51)) 
#define REG_TCNT0           (*((volatile uint8_type*) 0x52)) 
#define REG_TCCR0           (*((volatile uint8_type*) 0x53)) 
#define REG_MCUCSR          (*((volatile uint8_type*) 0x54)) 
#define REG_MCUCR           (*((volatile uint8_type*) 0x55)) 
#define REG_TWCR           	(*((volatile uint8_type*) 0x56)) 
#define REG_SPMCR           (*((volatile uint8_type*) 0x57)) 
#define REG_TIFR           	(*((volatile uint8_type*) 0x58)) 
#define REG_TIMSK           (*((volatile uint8_type*) 0x59)) 
#define REG_GIFR           	(*((volatile uint8_type*) 0x5A)) 
#define REG_GICR           	(*((volatile uint8_type*) 0x5B)) 
#define REG_OCR0           	(*((volatile uint8_type*) 0x5C)) 
#define REG_SPL           	(*((volatile uint8_type*) 0x5D)) 
#define REG_SPH           	(*((volatile uint8_type*) 0x5E)) 
#define REG_SREG           	(*((volatile uint8_type*) 0x5F)) 

#else 
	#error "Please check the included register file or define ATMEGA_32A in your project in case of ATMEGA32A."
#endif /*ATMEGA_32A*/
#endif  /*__ATMEGA32_REGISTER_FILE__*/
