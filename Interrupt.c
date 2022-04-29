/*
 * Interrupt.c
*
* Created: 3/6/2021
* Author: David Sprouls
*
* Project name: Interrupt
* Description:
*    INT interrupt
*	
* 
* Hardware -- Any interrupt pin
*
*/
#include<avr/interrupt.h>
#include "Interrupt.h"


void int_interrupt(void)
{			
	EICRA=(1<<ISC01) | (1<<ISC11) | (1<<ISC21) | (1<<ISC31);   //failing edge on INT0
	EIMSK=(1<<INT0) | (1<<INT1) | (1<<INT2) | (1<<INT3);	// enable interupt on INT 0	
}