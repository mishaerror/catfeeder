/* 
 * File:   servo_motor.h
 * Author: Milos
 *
 * Created on February 7, 2018, 9:40 PM
 */

#ifndef SERVO_MOTOR_H
#define	SERVO_MOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif
    void servoInit();
    void servoStart();
    void servoStop();
    void servoPulse();


#ifdef	__cplusplus
}
#endif

#endif	/* SERVO_MOTOR_H */

