#include "motor.h"

void setMotorSpeed(MOTOR_SPEED speed) {
    CCP1CONbits.DC1B = (speed >> 8) & 0x3;
    CCPR1L = speed & 0x00FF;
}

void setupPwm() {
    // 1. Set the PWM period by writing to the PR2 register.
    PR2 = PWM_PERIOD;
    // 2. Set the PWM duty cycle by writing to the CCPRxL register and CCPxCON<5:4> bits.
    setMotorSpeed(PWM_HALF);
    //3. Make the CCPx pin an output by clearing the appropriate TRIS bit.
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    //4. Set the TMR2 prescale value, then enable Timer2 by writing to T2CON.
    T2CONbits.T2CKPS = 0; //prescaler 1
    T2CONbits.TMR2ON = 1; //timer on

    // 5. Configure the CCPx module for PWM operation.  
    CCP1CONbits.CCP1M = 0b1100; //PWM mode
    
    MOTOR_ENABLE = 0;//disable by default
}

void motorOff() {
    MOTOR_ENABLE = 0;
}

void motorOn() {
    MOTOR_ENABLE = 1;
}

