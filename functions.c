/*
 * functions.c
 *
 * Created: 4/7/2021 8:06:32 PM
 *  Author: Phillip Salowe
 
 This module contains the delay functions, the init for timer 1 and the io inits.
 
 HARDWARE:
	none
 */ 

#include <avr/io.h>
#include "functions.h"


void delay_1ms(void)
{
	TCCR0A = 0x00;					// set timer for "normal port operation"


	TCNT0 = 5;						// preload the timer/counter to count enough ticks for 1mS to elapse (250)
	TCCR0B = (1<<CS11)|(1<<CS10);					// select an appropriate clock source fclk64

	while( (TIFR0 & (1<<TOV0)) == 0)		// wait for the timer to overflow by monitoring the timer overflow flag
	{
	}
	
	TCCR0B = 0;						// stop counting

	TIFR0 = TIFR0 | 0x01;			// clear the timer overflow flag

}
void delay_ms(uint16_t time)
{
	uint16_t counter; //declare the variable counter
	for(counter=0;counter<time;counter++)  //while counter is less than input, call the delay_1ms function
	{
		delay_1ms(); //call the delay 1 ms function
	}
}


void timer_init(void)
{
	OCR1A = 0;           //timer 1 for duty cycle for motor
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
}

void io_init(void)
{
	DDRA = 0x00;
	PORTA = 0xFF;     //for PB's
	DDRC = 0xFF;
	PORTC = 0x00;     //for stepper motor
	PORTB = 0x00;
	DDRB |= (1<<PB5);     //for motor control
	PORTF = 0x00;   //for ADC inputs
	DDRF = 0x00;
	DDRE = 0xFF;	//OCR3A and B outputs
	DDRH = 0xFF;    //OCR4A timer setup
	PORTH = 0x00;
}