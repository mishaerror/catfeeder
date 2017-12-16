#include "xcincludes.h"

#include "realtimeclock.h"
#include "utils.h"

#define TIME_EEPROM_ADDR 30

void readTimeFromEeprom() {
    minutes = eepromRead(TIME_EEPROM_ADDR);
    if(minutes > 59) {
        minutes = 0;
    }
    hours = eepromRead(TIME_EEPROM_ADDR + 1);
    if(hours > 23) {
        hours = 0;
    }
}

void writeTimeToEeprom() {
    eepromWrite(TIME_EEPROM_ADDR, minutes);
    eepromWrite(TIME_EEPROM_ADDR + 1, hours);
}

void setupRealTimeClock() {
    readTimeFromEeprom();
        
    TMR1 = TMR1_RESET_VALUE;//interrupt on overflow of this value, with 32kHz ticks
    
    T1CONbits.RD16 = 0; //16-bit read
    T1CONbits.T1CKPS = 0; //prescaler 1:1

    PIE1bits.TMR1IE = 1; //enable timer interrupt
    PIR1bits.TMR1IF = 0; //clear interrupt flag

    T1CONbits.TMR1CS = 1; //external clock
    T1CONbits.T1OSCEN = 1; //enable oscilator
    T1CONbits.T1SYNC = 1; //async
    T1CONbits.TMR1ON = 1; //enable timer
}

void addOneHour() {
    hours++;
    
    if (hours > 23) {
        hours = 0;
    }
    eepromWrite(TIME_EEPROM_ADDR + 1, hours);
}

void addOneMinute() {
    minutes++;
    
    if (minutes > 59) {
        minutes = 0;
        addOneHour();
    }
    eepromWrite(TIME_EEPROM_ADDR, minutes);
}

void addOneSecond() {
    seconds++;
    
    if (seconds > 59) {
        seconds = 0;
        addOneMinute();
    }
    
}

void addSeconds(char nofSeconds) {
    for(char i = 0; i < nofSeconds; i++) {
        addOneSecond();
    }
}
