/* 
 * File:   stepper_motor.h
 * Author: Milos
 *
 * Created on October 23, 2017, 7:51 PM
 */

#ifndef STEPPER_MOTOR_H
#define	STEPPER_MOTOR_H

#include <xc.h>

unsigned char _motor_step = 0;
unsigned char _motor_on = 0;

#define MOTOR_IN1 LATB3
#define MOTOR_IN2 LATB4
#define MOTOR_IN3 LATB5
#define MOTOR_IN4 LATB6

unsigned short motor_speed;

void motor_step();

void motor_setup();


#endif	/* STEPPER_MOTOR_H */

