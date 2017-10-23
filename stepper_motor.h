/* 
 * File:   stepper_motor.h
 * Author: Milos
 *
 * Created on October 23, 2017, 7:51 PM
 */

#ifndef STEPPER_MOTOR_H
#define	STEPPER_MOTOR_H

unsigned char _motor_step = 0;
unsigned char _motor_on = 0;

#define MOTOR_IN1 LATC2
#define MOTOR_IN2 LATC3
#define MOTOR_IN3 LATC4
#define MOTOR_IN4 LATC5
//mask
unsigned char steps[] = {};

void motor_step();

void motor_setup();

#endif	/* STEPPER_MOTOR_H */

