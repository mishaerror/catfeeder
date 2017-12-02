#include "realtimeclock.h"
#include "utils.h"
#define TIME_EEPROM_ADDR 12

void read_time_from_eeprom() {
    minutes = read_eeprom(TIME_EEPROM_ADDR);
    if(minutes > 59) {
        minutes = 0;
    }
    hours = read_eeprom(TIME_EEPROM_ADDR + 1);
    if(hours > 23) {
        hours = 0;
    }
}

void setupRealTimeClock() {
    TMR1 = TMR1_RESET_VALUE;//32k ticks per second, interrupt on overflow

    read_time_from_eeprom();
    
    T1CONbits.RD16 = 0; //16-bit read
    T1CONbits.T1CKPS = 0; //prescaler 1:1

    PIE1bits.TMR1IE = 1; //enable timer interrupt
    PIR1bits.TMR1IF = 0; //clear interrupt flag

    T1CONbits.TMR1CS = 1; //external clock
    T1CONbits.T1OSCEN = 1; //enable oscilator
    T1CONbits.TMR1ON = 1; //enable timer
}

void time_to_digit(unsigned char time, unsigned char* timestr) {

    if(time < 10){
        timestr[0] = '0';
    } else {
        timestr[0] = (unsigned char)(time /10 + 48);//ASCII numbers start at position 48
    }
    timestr[1] = (unsigned char)(time%10 + 48);
}

void addOneSecond() {
    seconds++;
    ClrWdt();
    
    if (seconds > 59) {
        seconds = 0;
        minutes++;
        write_eeprom(TIME_EEPROM_ADDR, minutes);
    }

    if (minutes > 59) {
        minutes = 0;
        hours++;
        write_eeprom(TIME_EEPROM_ADDR + 1, hours);
    }

    if (hours > 23) {
        hours = 0;
        write_eeprom(TIME_EEPROM_ADDR + 1, hours);
    }
    
}
