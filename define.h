#ifndef _DEFINE_H
#define _DEFINE_H


//#define F_CPU 14745600


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
//#include <avr/eeprom.h>
#include <stdlib.h>
#include <math.h>

//#define UART_BAUD_RATE 9600//19200

/***************************** définition pour RTC ***************************/
#define HOUR_24 0
#define HOUR_12 1

#define I2C_START 0
#define I2C_DATA 1
#define I2C_DATA_ACK 2
#define I2C_STOP 3
char ds1307_addr[7];
char hour_mode, ampm_mode;

// ************************ DEFINITIONS des SORTIES ***********************
#define Relais0_on()  PORTB |= _BV(0)	// DEL sur PB0 = 1	
#define Relais0_off()  PORTB &= ~_BV(0)	// DEL sur PB6 = 0
#define Relais1_on()  PORTB |= _BV(1)	// DEL sur PB1 = 1	
#define Relais1_off()  PORTB &= ~_BV(1)	// DEL sur PB1 = 0
#define Relais2_on()  PORTB |= _BV(2) 	// DEL sur PB2 = 1
#define Relais2_off()  PORTB &= ~_BV(2)	// DEL sur PB2 = 0
#define Relais3_on()  PORTB |= _BV(3)	// DEL sur PB3 = 1	
#define Relais3_off()  PORTB &= ~_BV(3)	// DEL sur PB3 = 0
// #define Relais4_on()  PORTB |= _BV(4)	// DEL sur PB4 = 1	
// #define Relais4_off()  PORTB &= ~_BV(4)	// DEL sur PB5 = 0
// #define Relais5_on()  PORTB |= _BV(5)	// DEL sur PB5 = 1	
// #define Relais5_off()  PORTB &= ~_BV(5)	// DEL sur PB5 = 0
// #define Relais6_on()  PORTB |= _BV(6)	// DEL sur PB6 = 1	
// #define Relais6_off()  PORTB &= ~_BV(6)	// DEL sur PB6 = 0
// #define Relais7_on()  PORTB |= _BV(7) 	// DEL sur PB7 = 1
// #define Relais7_off()  PORTB &= ~_BV(7)	// DEL sur PB7 = 0




#endif




