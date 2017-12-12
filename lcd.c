//LCD Functions Developed by electroSome
#include "xcincludes.h"
#include "i2c.h"
#include "lcd.h"

#ifndef LCD_I2C
    #define LCD_D4 LATAbits.LA0
    #define LCD_D5 LATAbits.LA1
    #define LCD_D6 LATAbits.LA2
    #define LCD_D7 LATAbits.LA3
    #define LCD_RS LATAbits.LA4 
    #define LCD_RW LATAbits.LA5
    #define LCD_EN LATAbits.LA6
#endif

// commands
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80
#define LCD_ROW_WIDTH 0x40

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define LCD_I2C_ADDRESS 0x27
#define LCD_I2C_EN_ON 0b00000100;
#define LCD_I2C_EN_OFF 0b11111011;

char lcd_display_control = 0;

char lcd_display_function = 0;

char lcd_display_mode = 0;

unsigned _lcd_on = 1;

#ifndef LCD_I2C 

void Lcd_Port(unsigned char a) {
    LATA &= 0xF0;
    LATA |= (unsigned char) (a & 0x0F);
    __delay_us(2);

    LATA |= 0b00100000; //e=1
    __delay_us(5);
    LATA &= 0b11011111; //e=0
    __delay_us(1);
}
#else

void lcdI2cTransmit(unsigned char i2cData) {
    i2cStart();
    i2cWrite((LCD_I2C_ADDRESS << 1)&0xFE); //7-bit address + 0 = write
    i2cWrite(i2cData);
    i2cStop();
}

void lcdI2cCommand(unsigned char cmd) {
    lcdI2cTransmit(cmd);
    __delay_us(2);
    cmd |= LCD_I2C_EN_ON; //EN = 1
    lcdI2cTransmit(cmd);
    __delay_us(5);
    cmd &= LCD_I2C_EN_OFF; //EN = 0
    lcdI2cTransmit(cmd);
    __delay_us(1);
}
#endif

void lcdWriteByte(const unsigned char a, unsigned char rs) {
    unsigned char low, high, i2cmd = 0x00;

    low = a & 0x0F;
    high = (a & 0xF0) >> 4;

#ifdef LCD_I2C 
    //    P0 -------- RS
    //    P1 -------- R/W
    //    P2 -------- En
    //    P3 -------- LED on/off
    //    P4 -------- Data pin 4
    //    P5 -------- Data pin 5
    //    P6 -------- Data pin 6
    //    P7 -------- Data pin 7

    i2cmd |= rs; //PO
    //P1 is already 0, always write
    i2cmd |= 0b0000; // P2, EN = 0
    i2cmd |= (_lcd_on << 3); // P3 - LED on

    //set high nibble on d4-d7
    i2cmd |= (a & 0xF0);
    
    lcdI2cCommand(i2cmd);

    //set low data nibble
    i2cmd &= 0x0F;
    i2cmd |= ((a << 4) & 0xF0);

    lcdI2cCommand(i2cmd);


#else

    if (rs == 0) {
        LATA &= 0b11101111;
    } else {
        LATA |= 0b00010000;
    }
    __delay_us(1);
    Lcd_Port(high);
    __delay_us(1);
    Lcd_Port(low);
#endif

    __delay_us(50); // commands need > 37us to settle
}

void lcdCmd(const unsigned char a) {
    lcdWriteByte(a, 0);
}

void lcdWriteChar(const unsigned char a) {
    lcdWriteByte(a, 1);
}

void lcdWriteString(const unsigned char *a) {
    for (int i = 0; a[i] != 0; i++) {
        char c = a[i];
        lcdWriteChar(a[i]);
    }
}

void lcdClear() {
    lcdCmd(LCD_CLEAR_DISPLAY);
    __delay_ms(2);
}

void lcdHome() {
    lcdCmd(LCD_RETURNHOME);
    __delay_ms(2);
}

void lcdSetCursor(char row, char col) {
    lcdCmd((unsigned char) (LCD_SETDDRAMADDR + row * LCD_ROW_WIDTH + col));
    __delay_us(50);
}

void lcdInit() {

#ifdef LCD_I2C
    i2cInit(100000);
#else
    LATA = 0;
    LCD_RW = 0;
#endif

    __delay_ms(100);

    lcdCmd(0x03);
    __delay_ms(5);
    lcdCmd(0x03);
    __delay_ms(5);
    lcdCmd(0x03);
    __delay_us(150);
    lcdCmd(0x02);
    __delay_us(150);
    /////////////////////////////////////////////////////

    lcd_display_function = LCD_2LINE | LCD_5x8DOTS;
    lcdCmd(LCD_FUNCTIONSET | lcd_display_function);
    __delay_ms(5);

    lcd_display_control = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    lcdCmd(LCD_DISPLAYCONTROL | lcd_display_control);
    __delay_ms(5);

    lcd_display_mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    lcdCmd(LCD_ENTRYMODESET | lcd_display_mode);
    __delay_ms(5);

}

void lcdOn(unsigned on) {
    _lcd_on = on;
    if(on) {
        lcd_display_control |= LCD_DISPLAYON;
    } else {
        lcd_display_control |= LCD_DISPLAYOFF;
    }
    lcdCmd(LCD_DISPLAYCONTROL | lcd_display_control);
    __delay_ms(5);
}
void lcdCursor(unsigned on) {
    if(on) {
      lcdCmd(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF);
    } else {
      lcdCmd(LCD_DISPLAYCONTROL | LCD_DISPLAYON |LCD_CURSOROFF | LCD_BLINKOFF);
    }
}
