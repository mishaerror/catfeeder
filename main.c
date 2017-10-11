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

unsigned char PRESSED_KEY = 0;

#define KEY0_PRESSED 0x1
#define KEY1_PRESSED 0x2
#define KEY2_PRESSED 0x4

FEEDING_TIME FEEDING1 = {0, 0, 0};
FEEDING_TIME FEEDING2 = {0, 0, 0};
FEEDING_TIME FEEDING3 = {0, 0, 0};
FEEDING_TIME FEEDING4 = {0, 0, 0};

#define debounce_delay() __delay_ms(50)

int getDailyWeight() {
    int sum = FEEDING1.quantity + FEEDING2.quantity + FEEDING3.quantity + FEEDING4.quantity;

    return sum;
}; //sum of all feedings

short totalFeedings() {
    return FEEDING1.quantity^0 + FEEDING2.quantity^0 + FEEDING3.quantity^0 + FEEDING4.quantity^0;
}

void enableInterrupts() {

    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;
    RBPU = 0;// activate pullups

    INTCONbits.INT0IE = 1; //enter button
    INTCON3bits.INT1IE = 1; //plus button
    INTCON3bits.INT2IE = 1; //minus button
    INTEDG0 = 0;//falling edge
    INTEDG1 = 0;
    INTEDG2 = 0;

    INTCONbits.GIE = 1; //global interrupts
    INTCONbits.PEIE = 1; //perferal interrupts
}

void setupPorts() {
    TRISA = 0;
    TRISCbits.RC4 = 0;
    PORTCbits.RC4 = 1;//signal lamp
}

void writeTimeToLcd(int row, int col) {
    Lcd_Set_Cursor(row, col);
    Lcd_Write_String("Time:");
    Lcd_Write_String(str_hours);
    Lcd_Write_Char(':');
    Lcd_Write_String(str_minutes);
    Lcd_Write_Char(':');
    Lcd_Write_String(str_seconds);

}

void interrupt handleInterrupt() {

    if (TMR1IE && TMR1IF) { // any timer 1 interrupts?
        TMR1IF = 0;
        TMR1 = TMR1_RESET_VALUE;

        PORTCbits.RC4 ^= 1;
        addOneSecond();

        writeTimeToLcd(0, 0);
    }

    //handle button press events
    if (INT0IF) {
        INT0IF = 0;
        debounce_delay();
        if (!PORTBbits.RB0) {
            PRESSED_KEY |= KEY0_PRESSED;
        }
        return;
    }
    if (INT1IF) {
        INT1IF = 0;
        debounce_delay();
        if (!PORTBbits.RB1) {
            PRESSED_KEY |= KEY1_PRESSED;
        }
        return;
    }
    if (INT2IF) {
        INT1IF = 0;
        debounce_delay();
        if (!PORTBbits.RB2) {
            PRESSED_KEY |= KEY2_PRESSED;
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
    setupPorts();
    setupRealTimeClock();
    enableInterrupts();

    Lcd_Init();
    Lcd_Clear();
    Lcd_Home();

    while (1) {

    }
}
