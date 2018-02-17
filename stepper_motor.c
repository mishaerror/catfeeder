
#include "stepper_motor.h"

#define TMR3_FULL_SPEED 0xFFC0
#define TMR3_HALF_SPEED 0xFF70
#define TMR3_QTR_SPEED  0xFF30

#define MOTOR_IN1 LATA0
#define MOTOR_IN2 LATA1
#define MOTOR_IN3 LATA2
#define MOTOR_IN4 LATA3

#define STEPS_PER_REV 1000 //some nice people on the web calculated it takes 4076 for full turn using half step method
#define STEPS_REVERSE 250 //approx 1/4 of forward move

//grains can get stuck so after approx. 1/4 turn forward we step back 1/16
char reversing = 0;

unsigned char _motor_step = 0;
unsigned short _motor_speed;

int stepCounter = 0; 

void stepperStart() {
    
    TRISAbits.RA0 = 0;
    TRISAbits.RA1 = 0;
    TRISAbits.RA2 = 0;
    TRISAbits.RA3 = 0;
    
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
    MOTOR_IN3 = 0;
    MOTOR_IN4 = 0;
    
    TMR3CS = 1;// sync on timer1 (real time clock)
    T3SYNC = 1;
    RD163 = 1; //16bit read of timer 3
    T3CKPS0 = 0;
    T3CKPS1 = 0;//prescaler 1:1
    _motor_speed = TMR3_FULL_SPEED;
    
    stepCounter = 0;
    reversing = 1;
    
    TMR3 = _motor_speed;
    TMR3IE = 1;// enable timer 3 interrupt
    TMR3IF = 0;
    TMR3ON = 1;
}

void stepperSpeed(motor_speed_t speed) {
    switch(speed) {
        case MOTOR_LOW:
            _motor_speed = TMR3_QTR_SPEED;
            break;
        case MOTOR_MIDDLE:
            _motor_speed = TMR3_HALF_SPEED;
            break;
        case MOTOR_HIGH:
            _motor_speed = TMR3_FULL_SPEED;
            break;
    }
}

void stepperStop() {
    TMR3ON = 0;
    TMR3IE = 0;
    MOTOR_IN1 = 0;
    MOTOR_IN2 = 0;
    MOTOR_IN3 = 0;
    MOTOR_IN4 = 0;
}

void stepperStep() {
    TMR3 = _motor_speed;
    if(reversing) {
        if(_motor_step == 0) {
            _motor_step = 3;
        } else {
            _motor_step--;
        }
    } else {
        if(_motor_step == 3) {
            _motor_step = 0;
        } else {
            _motor_step++;
        }
    }

    stepCounter++;
    if(reversing && stepCounter > STEPS_REVERSE) {
        stepCounter = 0;
        reversing = 0;
        _motor_step = 0;
    } 
    if(!reversing && stepCounter > STEPS_PER_REV) {
        stepCounter = 0;
        reversing = 1;
        _motor_step = 0;
    }
    
    switch (_motor_step) {
        case 0:
            MOTOR_IN1 = 1;
            MOTOR_IN2 = 1;
            MOTOR_IN3 = 0;
            MOTOR_IN4 = 0;
            break;
        case 1:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 1;
            MOTOR_IN3 = 1;
            MOTOR_IN4 = 0;
            break;
        case 2:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 0;
            MOTOR_IN3 = 1;
            MOTOR_IN4 = 1;
            break;
        case 3:
            MOTOR_IN1 = 1;
            MOTOR_IN2 = 0;
            MOTOR_IN3 = 0;
            MOTOR_IN4 = 1;
            break;
        /*
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
         * */
        default:
            MOTOR_IN1 = 0;
            MOTOR_IN2 = 0;
            MOTOR_IN3 = 0;
            MOTOR_IN4 = 0;
            break;
    }
}
