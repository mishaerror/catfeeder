#include "utils.h"

inline void numberToLcdStr(unsigned char time, unsigned char* timestr) {

    if (time < 10) {
        timestr[0] = '0';
    } else {
        timestr[0] = (unsigned char) (time / 10 + 48); //ASCII numbers start at position 48
    }
    timestr[1] = (unsigned char) (time % 10 + 48);
}

void eepromWrite(unsigned int addr, unsigned char byte) {
    EECON1bits.WREN = 1; //write enable
    EEADR = addr;
    EEDATA = byte;
    EECON1bits.EEPGD = 0; //data memory
    EECON1bits.CFGS = 0; //access EEPROM

    //disable interrupts
    INTCONbits.GIE = 0;

    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;

    //re-enable interrupts
    INTCONbits.GIE = 0;
    while (EECON1bits.WR == 1);
    EECON1bits.WREN = 0; //write disabled
}

unsigned char eepromRead(unsigned int addr) {
    EEADR = addr; //memory address
    EECON1bits.EEPGD = 0; //data memory
    EECON1bits.CFGS = 0; //access EEPROM
    EECON1bits.RD = 1; //read operation

    return EEDATA;
}

inline char digitToLcdChar(char number) {
    return number + 48;
}

inline char timeAfter(char h1, char m1, char h2, char m2) {
    if (h1>h2 || (h1==h2 && m1>m2)){
        return 1;
    }
    
    return 0;
}