/*
 * functions.h
 *
 * Created: 4/7/2021 8:06:32 PM
 *  Author: Phillip Salowe
 
 This module contains the delay functions, the init for timer 1 and the io inits.
 
 HARDWARE:
	none
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <avr/io.h>


void delay_1ms(void);
void delay_ms(uint16_t time);
void timer_init(void);
void io_init(void);



#endif /* FUNCTIONS_H_ */