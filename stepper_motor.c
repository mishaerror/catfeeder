
#include <pic18f2420.h>

#include "stepper_motor.h"

#define TMR3_FULL_SPEED 0xF800
#define TMR3_HALF_SPEED 0xF000
#define TMR3_QTR_SPEED  0xE000
void motor_setup() {
    TMR3CS = 1;// sync on timer1 (real time clock)
    RD163 = 1; //16bit read of timer 3
    T3CKPS0 = 0;
    T3CKPS1 = 0;//prescaler 1:1
    
    TMR3 = TMR3_HALF_SPEED;
    TMR3IE = 1;// enable timer 3 
}

void motor_step() {
    _motor_step++;
    if(_motor_step > 7) {
        _motor_step = 0;
    }
    switch (_motor_step) {
        case 0:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 0;
            MOTOR_IN3 = 0;
            MOTOR_IN4 = 1;
            break;
        case 1:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 0;
            MOTOR_IN3 = 1;
            MOTOR_IN4 = 1;
            break;
        case 2:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 0;
            MOTOR_IN3 = 1;
            MOTOR_IN4 = 0;
            break;
        case 3:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 1;
            MOTOR_IN3 = 1;
            MOTOR_IN4 = 0;
            break;
        case 4:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 1;
            MOTOR_IN3 = 0;
            MOTOR_IN4 = 0;
            break;
        case 5:
            MOTOR_IN1 = 1;
            MOTOR_IN2 = 1;
            MOTOR_IN3 = 0;
            MOTOR_IN4 = 0;
            break;
        case 6:
            MOTOR_IN1 = 1;
            MOTOR_IN2 = 0;
            MOTOR_IN3 = 0;
            MOTOR_IN4 = 0;
            break;
        case 7:
            MOTOR_IN1 = 1;
            MOTOR_IN2 = 0;
            MOTOR_IN3 = 0;
            MOTOR_IN4 = 1;
            break;
        default:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 0;
            MOTOR_IN3 = 0;
            MOTOR_IN4 = 0;
            break;
    }
}
