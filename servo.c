/*
 * servo
*
* Created: 3/6/2021
* Author: Alex Neil and Phillip Salowe
*
* Project name: servo
* Description:
*    Has the timer inits for the servo and two modules that use ADC to control two different servos
	 It also has the ADC init and ADC_read functions so that the ADC can be used
*	
* 
* Hardware
   PORTF:
   PF1: For ADC channel 1 input from pot
   PF2: For ADC channel 2 input from pot
   
   PORTH:
   PH3: outputs PWM wave that is controlled by ADC channel 2. It is timer 4A
   
   PORTE:
   PE3: outputs PWM wave that is controlled by ADC channel 1. It is timer 3A
*
*/

#include "servo.h"


void initServoTimer(void)
{
	
	
	/////////////////////////////////////////////// make this neater with | (1<<xx)
	TCCR3B = 0b00011000;	//pwm mode 14
	TCCR3A = 0b00000010;
	
	TCCR3A = TCCR3A | 0x80;	//clear on match
	TCCR3B = TCCR3B | 0x02;	//clk / 8
	///////////////////////////////////////////////
	
	ICR3 = 40000;	//sets frequency to 50 Hz (20 ms period)
	OCR3A = 3000;	//width of 1500 us. 0.5 us/tick
	
	TCCR4B = 0b00011000;	//pwm mode 14
	TCCR4A = 0b00000010;
	
	TCCR4A = TCCR4A | 0x80;	//clear on match
	TCCR4B = TCCR4B | 0x02;	//clk / 8
	///////////////////////////////////////////////
	
	ICR4 = 40000;	//sets frequency to 50 Hz (20 ms period)
	OCR4A = 3000;	//width of 1500 us. 0.5 us/tick
}

void timer3_servo_driver(void)
{
	
	uint16_t number;
	uint16_t width;
	
	number = ADC_read(1);	            //read the adc value from channel 1 (PF1)
	width = number * 1.178 + 900;       //scale the ADC value to a pulse width in us
	
	OCR3A = width * 2;                  //multiply by 2, since 1 tick = 0.5 us
}

void servoPotDrive(void)
{
	
	uint16_t number2;
	
	uint16_t width2;

	number2 = ADC_read(2);              //read the adc value from channel 2 (PF2) 
	width2 = number2 * 1.178 + 900;     //scale the ADC value to a pulse width in us

	OCR4A = width2 * 2;                 //multiply by 2, since 1 tick = 0.5 us
}

void init_ADC(void)
{
	ADMUX = (1<<REFS0);    //MUX4 is ADC0 or pin F0 or pin 97
	ADCSRA = (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);    //128 prescaler need to set up prescaler
	ADCSRB = 0;
}

uint16_t ADC_read(uint16_t channel)
{
	uint16_t adc_num = 0;
	uint16_t adc_num_upper = 0;

	ADMUX =  (1<<REFS0) | channel;
	ADCSRA = (1<<ADSC) | (1<<ADEN) | (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2);    //128 prescaler
	while ((ADCSRA & (1<<ADSC)) == 1);                                        //wait for conversion to be done
	
	adc_num = ADCL;                                    //read low bits
	adc_num_upper = ADCH;                              //read high bits
	adc_num= adc_num | (adc_num_upper << 8);

	return adc_num;       //ADC code by phillip salowe
}

