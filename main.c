/*
 * crane.c
 *
 * Created: 4/7/2021 7:39:00 PM
 * Authors : Phillip Salowe, David Sprouls, Markus Priest, Alex Neil
 
 This code controls the crane for our final project. There are two servo motors and one stepper motor.
 The stepper motor is controlled by buttons that decide if it should move left or right
 The servo motors are both controlled by a separate potentiometer that control where they are placed
 
 There is serial communication using USART for the password. When the right password is entered, the
 program will respond that the user is in configure mode. Before the correct password is typed, the controls cannot be used.
 
 Hardware
 PORTF:
 PF1: For ADC channel 1 input from pot
 PF2: For ADC channel 2 input from pot
 
 PORTH:
 PH3: outputs PWM wave that is controlled by ADC channel 2. It is timer 4A. Hooked up to servo motor
 
 PORTE:
 PE3: outputs PWM wave that is controlled by ADC channel 1. It is timer 3A. Hooked up to servo motor
 
 PORTA:
 PA0: Pushbutton to move stepper CCW
 PA1: Pushbutton to move stepper CW
 PA2: Pushbutton to move stepper to home position
 
 PORTC:
 PC0 - PC4: For outputs to the stepper motor (PC0 to pin 1 on motor, PC1 to pin 2 on motor, etc)
 
 */ 

#include <avr/io.h>
#include "functions.h"
#include "Interrupt.h"
#include "USART.h"
#include "servo.h"

#define delay 3

volatile uint8_t Wave[4] = {0x01, 0x02, 0x04, 0x08};            //for the servo motor pattern
volatile int32_t numSteps = 0;
volatile uint16_t steps = ((uint32_t)1 * 2048) / 360;
volatile uint8_t config_mode = 0; // when config mode is == to 1 password is ok and its time to enter config / setup mode

void goHome(void);                  //function header for the function that makes the crane go home
void password_checker(void);        //function header for the function that checks the password


int main(void)
{
	io_init();              //inits all the IO
	init_ADC();             //inits the ADC
	timer_init();
	uint8_t k = 0;
	
	initServoTimer();      //inits the timers used to control the servos
	
	Serial_Begin();//start serial
	Serial_Read(); // setup serial read interrupts

	goHome();              //makes the crane go to the home position before anything else happens
	
    while (1) 
    {
	
		while (config_mode != 1)           //when configure mode is 1, the password entered was correct
		{
			password_checker();            //keep waiting for the correct password before any other controls can be accessed
		}
		
		while((PINA & (1<< PA1)) == 2)              //when the button 2 is pressed, the servo will move clockwise
		{
			k = 0;
			
			for (uint16_t j = 0; j < steps; j++)          //will keep moving until button is released
			{
				numSteps++;                               //keep track of how many steps were taken
				k = j % 4;
				PORTC = Wave[k];                          //will constantly move the motor
				delay_ms(delay);				
			}				
		}
		//degrees = 360;
			
		while((PINA & (1<< PA0)) == 1)                    //when the button 1 is pressed, the servo will move counterclockwise
		{
			k = 0;

				for (uint16_t j = steps; j > 0; j--)
				{
					numSteps--;                               //keep track of how many steps were taken
					k = j % 4;
					PORTC = Wave[k];                          //will constantly move the motor counterclockwise				
					delay_ms(delay);				
				}

		}

		servoPotDrive();                //if the POT 1 is adjusted, the program will read the new ADC number and move the servo
		
		delay_ms(5);                    //delay is added for stability
		
		timer3_servo_driver();          //if the POT 2 is adjusted, the program will read the new ADC number and move the servo
		
		delay_ms(5);                    //delay is added for stability
				
    }
}

void goHome(void)
{
	uint8_t k = 0;
	uint16_t j = 0;
	uint8_t flag = 0;
	
	
	//rotate left until limit switch is pressed
	while(flag == 0)	//limit switch isn’t pressed
	{
		//numSteps--;
		k = j % 4;
		PORTC = Wave[k];           //will move the stepper motor clockwise until "home" pushbutton is reached
		delay_ms(delay);
		j++;
		
		if ((PINA & 0x04) == 4)
		{
			flag = 1;              //when home pushbutton is pressed, the crane is in the home position. 
								   //The code can now leave the loop
		}
	}
}

void password_checker(void)
{
	char password[32] = "123"; //password for config mode max 32 char
	char send_message_OK[32] = "Entering config mode\n";
	char send_message_NG[32] = "Password incorrect\n";
	while (check_password)
	{
		for (uint8_t i = 0 ; i < array_index-1; i++)
		{
			if (RX_buffer[i] != password[i]) // if any chhar is not correct then break
			{
				for (uint8_t j = 0 ; j < 20; j++)
				{
					Serial_Write(send_message_NG[j]); //print message ng, password not correct
				}
				for (uint8_t k = 0; k < 32; k++)
				{
					RX_buffer[i] = 32;
				}
				array_index = 0;
				check_password=0;
				break;
			}
		}
		if (check_password)
		{
			for (uint8_t i = 0 ; i < 22; i++)
			{
				Serial_Write(send_message_OK[i]); //print message ok, entering config mode
				
			}
			for (uint8_t i = 0 ; i < array_index; i++)
			{
				RX_buffer[i]=32; //clear array 32 = space or blank
			}
			check_password = 0;
			array_index = 0;	// reset array counter to 0 for next string
			config_mode = 1;
		}
	}
}


