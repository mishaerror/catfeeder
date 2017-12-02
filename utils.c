#include "utils.h"

void write_eeprom(unsigned int addr, unsigned char byte) {
    EECON1bits.WREN = 1; //write enable
    EEADR = addr;
    EEDATA = byte;
    EECON1bits.EEPGD = 0;//data memory
    EECON1bits.CFGS = 0;//access EEPROM
    
    //disable interrupts
    INTCONbits.GIE = 0;
    
    EECON2 = 0x55;
    EECON2 = 0xAA;    
    EECON1bits.WR = 1;
    
    //re-enable interrupts
    INTCONbits.GIE = 0;
    while (EECON1bits.WR==1);
    EECON1bits.WREN = 0; //write disabled
}
unsigned char read_eeprom(unsigned int addr) {
    EEADR = addr;//memory address
    EECON1bits.EEPGD = 0;//data memory
    EECON1bits.CFGS = 0;//access EEPROM
    EECON1bits.RD = 1; //read operation
    
    return EEDATA;
}
