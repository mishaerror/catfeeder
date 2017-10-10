/*
 * File:   main.c
 * Author: Milos
 *
 * Created on September 18, 2017, 9:41 PM
 */
#include "bitconfig.h"
#include "xcincludes.h"
#include "realtimeclock.h"
#include "lcd.h"
#include <stdio.h>

typedef struct FEEDING_TIME_t {
    unsigned short hour;
    unsigned short minute;
    unsigned short quantity;
} FEEDING_TIME;

unsigned short PRESSED_KEY = 0;

FEEDING_TIME FEEDING1 = {0, 0, 0};
FEEDING_TIME FEEDING2 = {0, 0, 0};
FEEDING_TIME FEEDING3 = {0, 0, 0};
FEEDING_TIME FEEDING4 = {0, 0, 0};

int getDailyWeight() {
    int sum = FEEDING1.quantity + FEEDING2.quantity + FEEDING3.quantity + FEEDING4.quantity;

    return sum;
}; //sum of all feedings

short totalFeedings() {
    return FEEDING1.quantity^0 + FEEDING2.quantity^0 + FEEDING3.quantity^0 + FEEDING4.quantity^0;
}

void enableInterrupts() {
    INTCONbits.GIE = 1; //global interrupts
    INTCONbits.PEIE = 1; //perferal interrupts

    INTCONbits.INT0IE = 1; //enter button
    INTCON3bits.INT1IE = 1; //plus button
    INTCON3bits.INT2IE = 1; //minus button
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;
}

void setupPorts() {
    TRISA = 0;
    TRISCbits.RC4 = 0;

    PORTCbits.RC4 = 1;

}

void time_to_digit(char time) {
    if(time < 10){
        Lcd_Write_Char('0');
    } else {
        Lcd_Write_Char(time /10 + 48);
    }
    Lcd_Write_Char(time%10 + 48);
        
}

void interrupt handleInterrupt() {

    if (TMR1IE && TMR1IF) { // any timer 1 interrupts?
        TMR1IF = 0;
        TMR1 = 0x8000;

        PORTCbits.RC4 ^= 1;
        addOneSecond();
        
        Lcd_Home();
        Lcd_Write_String("Time: ");
        time_to_digit(hours);
        Lcd_Write_Char(':');
        time_to_digit(minutes);
        Lcd_Write_Char(':');
        time_to_digit(seconds);
    
    }

    //handle button press events
    if (INT0IF) {
        INT0IF = 0;
        __delay_ms(100);
        if (!PORTBbits.RB0) {
            PRESSED_KEY |= 0b0000;
        }
        return;
    }
    if (INT1IF) {
        INT1IF = 0;
        __delay_ms(100);
        if (!PORTBbits.RB1) {
            PRESSED_KEY |= 0b0010;
        }
        return;
    }
    if (INT2IF) {
        INT1IF = 0;
        __delay_ms(100);
        if (!PORTBbits.RB2) {
            PRESSED_KEY |= 0b0100;
        }
        return;
    }


}

void checkKeyPress() {
    if (PRESSED_KEY) {//any recorded key

    }
}

void renderScreen() {

}


void main(void) {
    int i;
    setupPorts();
    setupRealTimeClock();
    enableInterrupts();
    unsigned int a;

    Lcd_Init();
    Lcd_Clear();
    Lcd_Home();
    
    //
    while (1) {
    
    }
}
