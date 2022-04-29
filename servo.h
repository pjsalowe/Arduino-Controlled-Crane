/*
* servo
*
* Created: 3/6/2021
* Author: Alex Neil
*
* Project name: Interrupt
* Description:
*    Has the timer inits for the servo and two modules that use ADC to control two different servos
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

#ifndef SERVO_H_
#define SERVO_H_

#include <avr/io.h>
#include "functions.h"                         //for io inits

//function prototypes
void initServoTimer(void);
void servoPotDrive(void);
void timer3_servo_driver(void);
uint16_t ADC_read(uint16_t channel);
void init_ADC(void);

#endif /* SERVO_H_ */