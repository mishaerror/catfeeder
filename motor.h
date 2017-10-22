/* 
 * File:   motor.h
 * Author: Milos
 *
 * Created on October 22, 2017, 3:01 PM
 */
#include <xc.h>

#ifndef MOTOR_H
#define	MOTOR_H


#define PWM_PERIOD 0xFF
#define MOTOR_ENABLE LATC3

typedef enum {
    PWM_FULL = 0b1111111111,
    PWM_3QTR = 0b1101111111,
    PWM_HALF = 0b0111111111,
    PWM_QTR = 0b0011111111
} MOTOR_SPEED;

void setMotorSpeed(MOTOR_SPEED speed);
void setupPwm();
void motorOff();
void motorOn();

#endif	/* MOTOR_H */

