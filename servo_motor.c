#include "xcincludes.h"

/**
 * CCP PWM sounded nice, but unfortunately, we can't get period that is 20ms.
 * Therefore we simulate PWM using TIMER0 module with pulse output on A4
 */
void servoInit() {
  
  TRISA4 = 0; //pulse output on A4
  LATA4 = 0;
  
  T2CONbits.T2OUTPS = 0b0000;
  T2CONbits.T2CKPS = 0b00;//prescaler 1:1
  PR2 = 199;          // Interupt every 0.1 ms
  TMR2IF = 0;
  TMR2IE = 1;	     // Enable interrupt on TMR0 overflow
}

void servoStart() {
    TMR2ON = 1;
}

void servoStop() {
    TMR2ON = 0;
}

const char periodThreshold = 100;//20 ms
char dutyCycleThreshold = 5;
char pulsePeriod = 0;
char pulseDutyCycle = 0;
char angle = 0;

void servoPulse() {
    pulsePeriod++;
    pulseDutyCycle++;
    if(pulsePeriod >= periodThreshold) {//new period
        pulseDutyCycle = 0;
        pulsePeriod = 0;
        LATA4 = 1;//set output high
    }
    if(pulseDutyCycle >= dutyCycleThreshold) {
        LATA4 = 0;
    } else {
        LATA4 = 1;
    }
}
