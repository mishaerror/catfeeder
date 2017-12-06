//LCD Functions Developed by electroSome
#include "i2c.h"

#include <xc.h>

#define LCD_D4 LATAbits.LA0
#define LCD_D5 LATAbits.LA1
#define LCD_D6 LATAbits.LA2
#define LCD_D7 LATAbits.LA3
#define LCD_RS LATAbits.LA4 
#define LCD_RW LATAbits.LA5
#define LCD_EN LATAbits.LA6

// commands
#define LCD_CLEARDISPLAY 0x01
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

#define LCD_I2C 1
#define LCD_I2C_ADDRESS 0x27
void Lcd_Port(unsigned char a) {
    LATA &= 0xF0;
    LATA |= (unsigned char)(a & 0x0F); 
    __delay_us(2);
    
    LATA |= 0b00100000;//e=1
    __delay_us(5);
    LATA &= 0b11011111; //e=0
    __delay_us(1);
}

void Lcd_I2C_Transmit(unsigned char i2cData) {
    I2C_Master_Start();
    I2C_Master_Write((LCD_I2C_ADDRESS << 1)&0xFE);//7-bit address + 0 = write
    I2C_Master_Write(i2cData);
    I2C_Master_Stop();
}

unsigned char lcd_display_control = 0;
unsigned char lcd_display_function = 0;
unsigned char lcd_display_mode = 0;

void Lcd_Write_Byte(const unsigned char a, unsigned char rs) {
    unsigned char low, high, i2cmd = 0x00;
    const unsigned char lcd_en_on = 0b00000100;
    const unsigned char lcd_en_off = 0b11111011;

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

    i2cmd |= rs;//PO
                //P1 is already 0, always write
    i2cmd |= 0b0000;// P2, EN = 0
    i2cmd |= 0b1000; // P3 - LED on
    
    
    
    //set high nibble on d4-d7
    i2cmd |= (a & 0xF0);
    Lcd_I2C_Transmit(i2cmd);
    __delay_us(2);  
    i2cmd |= lcd_en_on;//EN = 1
    Lcd_I2C_Transmit(i2cmd);
    __delay_us(5);
    i2cmd &= lcd_en_off;//EN = 0
    Lcd_I2C_Transmit(i2cmd);
    __delay_us(1);

    //set low data nibble
    i2cmd &= 0x0F;
    i2cmd |= ((a << 4) & 0xF0);
    

    Lcd_I2C_Transmit(i2cmd);
    __delay_us(2);  
    i2cmd |= lcd_en_on;//EN = 1
    Lcd_I2C_Transmit(i2cmd);
    __delay_us(5);
    i2cmd &= lcd_en_off;//EN = 0
    Lcd_I2C_Transmit(i2cmd);
    __delay_us(1);

#else
    
    if(rs == 0) {
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
void Lcd_Cmd(const unsigned char a) {
    Lcd_Write_Byte(a, 0);
}

void Lcd_Write_Char(const unsigned char a) {
    Lcd_Write_Byte(a, 1);
}

void Lcd_Write_String(const unsigned char *a) {
    for (int i = 0; a[i] != 0; i++){
        char c = a[i];
        Lcd_Write_Char(a[i]);
    }
}

void Lcd_Clear() {
    Lcd_Cmd(LCD_CLEARDISPLAY);
    __delay_ms(2);
}

void Lcd_Home() {
    Lcd_Cmd(LCD_RETURNHOME);
    __delay_ms(2);
}

void Lcd_Set_Cursor(char row, char col) {
    Lcd_Cmd((unsigned char)(LCD_SETDDRAMADDR + row*LCD_ROW_WIDTH + col));
    __delay_us(50);
}

void Lcd_Init() {
    
#ifdef LCD_I2C
    I2C_Master_Init(100000);
#endif

    
    LATA = 0;

    LCD_RW = 0;

    __delay_ms(100);

    Lcd_Cmd(0x03);
    __delay_ms(5);
    Lcd_Cmd(0x03);
    __delay_ms(5);
    Lcd_Cmd(0x03);
    __delay_us(150);
    Lcd_Cmd(0x02);
    __delay_us(150);
    /////////////////////////////////////////////////////

    lcd_display_function = LCD_2LINE | LCD_5x8DOTS;
    Lcd_Cmd(LCD_FUNCTIONSET | lcd_display_function);
    __delay_ms(5);

    lcd_display_control = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    Lcd_Cmd(LCD_DISPLAYCONTROL | lcd_display_control);
    __delay_ms(5);

    lcd_display_mode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    Lcd_Cmd(LCD_ENTRYMODESET | lcd_display_mode);
    __delay_ms(5);

}

