/* 
 * File:   stepper_motor.h
 * Author: Milos
 *
 * Created on October 23, 2017, 7:51 PM
 */

#ifndef STEPPER_MOTOR_H
#define	STEPPER_MOTOR_H

#include "xcincludes.h"

typedef enum MOTOR_SPEED {
    MOTOR_LOW, MOTOR_MIDDLE, MOTOR_HIGH
} motor_speed_t;

void stepperStep();

void stepperStart();

void stepperStop();

void stepperSpeed(motor_speed_t speed);
#endif	/* STEPPER_MOTOR_H */

