
#include "stepper_motor.h"

#define TMR3_FULL_SPEED 0xFFE0
#define TMR3_HALF_SPEED 0xFF70
#define TMR3_QTR_SPEED  0xFF30

#define MOTOR_IN1 LATB3
#define MOTOR_IN2 LATB4
#define MOTOR_IN3 LATB5
#define MOTOR_IN4 LATB6

unsigned char _motor_step = 0;
unsigned char _motor_on = 0;
unsigned short _motor_speed;

void motorStart() {
    
    TRISBbits.RB3 = 0;
    TRISBbits.RB4 = 0;
    TRISBbits.RB5 = 0;
    TRISBbits.RB6 = 0;
    
    TMR3CS = 1;// sync on timer1 (real time clock)
    T3SYNC = 1;
    RD163 = 1; //16bit read of timer 3
    T3CKPS0 = 0;
    T3CKPS1 = 0;//prescaler 1:1
    _motor_speed = TMR3_FULL_SPEED;
    
    TMR3 = _motor_speed;
    TMR3IE = 1;// enable timer 3 interrupt
    TMR3IF = 0;
    TMR3ON = 1;
}

void motorStop() {
    TMR3ON = 0;
    TMR3IE = 0;
    _motor_on = 0;
}

void motorStep() {
    if(!_motor_on) {
        return;
    }
    TMR3 = _motor_speed;
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
