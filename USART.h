/*
* Project name: Delay
* Description:
* INT USART Com for serial communication
*
*
* Hardware Connection:
*	NONE
*
*/



#ifndef USART_H_
#define USART_H_

#include <avr/io.h>

#include <stdio.h> //Standard IO Library

#include <stdlib.h>  //Standard function Library

#include <avr/interrupt.h>

void Serial_Begin(void);
void Serial_Write(uint8_t);
void Serial_Read(void);

volatile uint8_t RX_char;
volatile char RX_buffer[32];
volatile uint8_t LCD_update ;
volatile uint8_t array_index;
volatile uint8_t check_password; // when check password == 1 a new password has been enterened and needs to be checked, this keeps inturupt code short

#endif /* USART_H_ */