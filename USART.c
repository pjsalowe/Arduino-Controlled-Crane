/*
 * USART.c
 *
 * Created: 2/21/2021
 * Author: David Sprouls
 *
 * Project name: Delay
 * Description:
 * INT USART Com for serial communication
 *
 *
 * Hardware Connection:
 *	TXD0
 *	RXD0
 */
#include "USART.h"

ISR (USART0_RX_vect) //Inturpt service routine for Serial comms RX
{
	RX_char=UDR0;
	if (RX_char == 0x0D) // null terminator
	{
		
		check_password = 1;		// set flag to enable checking password
	}
	else
	{
		RX_buffer[array_index] = RX_char;	//store rcvd char in array
		array_index = array_index + 1;		// increment index counter
	}
}

void Serial_Begin(void)
{

	UCSR0A = 0x00;			
	UCSR0B = 0x18;			// enable UART TX and RX
	UCSR0C = 0x06;			// set the UART for N, 8, 1
	UBRR0L = 103;			// set BAUD Rate for 9600 with 16MHz clock
	UBRR0H = 0;
	array_index = 0;
	check_password = 0;

}


// the following function sends a single character out of the serial port
void Serial_Write(uint8_t ch)
{

		while ( (UCSR0A & (1 << UDRE0)) == 0 )		// wait until there is room in the transmit buffer
		{
		}

		UDR0 = ch;

}

// the following function waits for a serial character to be received
void Serial_Read(void)
{
	UCSR0B = UCSR0B | (1 << RXCIE0); // setup interupt for RX
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
	
}