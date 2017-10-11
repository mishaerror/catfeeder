#include "realtimeclock.h"

void setupRealTimeClock() {
    TMR1 = TMR1_RESET_VALUE;//32k ticks per second, interrupt on overflow

    T1CONbits.RD16 = 0; //16-bit read
    T1CONbits.T1CKPS = 0; //prescaler 1:1

    PIE1bits.TMR1IE = 1; //enable timer interrupt
    PIR1bits.TMR1IF = 0; //clear interrupt flag

    T1CONbits.TMR1CS = 1; //external clock
    T1CONbits.T1OSCEN = 1; //enable oscilator
    T1CONbits.TMR1ON = 1; //enable timer
}

void time_to_digit(char time, char* timestr) {

    if(time < 10){
        timestr[0] = '0';
    } else {
        timestr[0] = (time /10 + 48);//ASCII numbers start at position 48
    }
    timestr[1] = (time%10 + 48);
}

void addOneSecond() {
    seconds++;
    if (seconds > 59) {
        seconds = 0;
        minutes++;
    }
    time_to_digit(seconds, str_seconds);

    if (minutes > 59) {
        minutes = 0;
        hours++;
    }
    time_to_digit(minutes, str_minutes);

    if (hours > 23) {
        hours = 0;
    }
    
    time_to_digit(hours, str_hours);

}
