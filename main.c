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

#define debounce_delay() __delay_ms(50)

#define BLANK_LINE "                "

enum KEY_PRESSED_t {
    KEY_ENTER,
    KEY_PLUS,
    KEY_MINUS
} key_pressed;

//states

enum DISPLAY_STATE_t {
    ST_START_SCREEN,
    ST_EDIT_TIME_HOUR,
    ST_EDIT_TIME_MINUTE,
    ST_VIEW_FEED,
    ST_EDIT_FEED_HOUR,
    ST_EDIT_FEED_MINUTE,
    ST_EDIT_FEED_QTY
} display_state = ST_START_SCREEN;

typedef struct {
    char hour;
    char minute;
    char quantity;
} FEEDING_t;

FEEDING_t feedings[4] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};
unsigned char feedIndex = 0;
unsigned char tmp_num; //hold value of currently edited number
unsigned int totalQty = 0;
unsigned char totalFeedings = 0;
char lcd_line_1[] = "****************";
char lcd_line_2[] = "****************";

void start_screen_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        display_state = ST_EDIT_TIME_HOUR;
        tmp_num = hours;
    } else if (key_pressed == KEY_PLUS) {
        display_state = ST_VIEW_FEED;
        feedIndex = 0;
    } else if (key_pressed == KEY_MINUS) {
        display_state = ST_VIEW_FEED;
        feedIndex = 3;
    }
}

void edit_hour_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        display_state = ST_EDIT_TIME_MINUTE;
        hours = tmp_num;
        tmp_num = minutes;
    } else if (key_pressed == KEY_PLUS) {
        display_state = ST_EDIT_TIME_HOUR;
        if (tmp_num == 23) {
            tmp_num = 0;
        } else {
            tmp_num++;
        }
    } else if (key_pressed == KEY_MINUS) {
        display_state = ST_EDIT_TIME_HOUR;
        if (tmp_num == 0) {
            tmp_num = 23;
        } else {
            tmp_num--;
        }
    }
}

void edit_minute_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        display_state = ST_START_SCREEN;
        minutes = tmp_num;
    } else if (key_pressed == KEY_PLUS) {
        display_state = ST_EDIT_TIME_MINUTE;
        if (tmp_num == 59) {
            tmp_num = 0;
        } else {
            tmp_num++;
        }
    } else if (key_pressed == KEY_MINUS) {
        display_state = ST_EDIT_TIME_MINUTE;
        if (tmp_num == 0) {
            tmp_num = 59;
        } else {
            tmp_num--;
        }
    }
}

void view_feed_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        display_state = ST_EDIT_FEED_HOUR;
        tmp_num = feedings[feedIndex].hour;
    } else if (key_pressed == KEY_PLUS) {
        if (feedIndex == 3) {
            display_state = ST_START_SCREEN;
        } else {
            feedIndex++;
            display_state = ST_VIEW_FEED;
        }
    } else if (key_pressed == KEY_MINUS) {
        display_state = ST_VIEW_FEED;
        if (feedIndex == 0) {
            display_state = ST_START_SCREEN;
        } else {
            feedIndex--;
            display_state = ST_VIEW_FEED;
        }
    }
}

void edit_feed_hour_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        feedings[feedIndex].hour = tmp_num;
        display_state = ST_EDIT_FEED_MINUTE;
    } else if (key_pressed == KEY_PLUS) {
        if (tmp_num == 23) {
            tmp_num = 0;
        } else {
            tmp_num++;
        }
        display_state = ST_EDIT_FEED_HOUR;
    } else if (key_pressed == KEY_MINUS) {
        if (tmp_num == 0) {
            tmp_num = 23;
        } else {
            tmp_num--;
        }
        display_state = ST_EDIT_FEED_HOUR;
    }
}

void edit_feed_minute_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        feedings[feedIndex].minute = tmp_num;
        display_state = ST_EDIT_FEED_QTY;
        tmp_num = feedings[feedIndex].quantity;
    } else if (key_pressed == KEY_PLUS) {
        if (tmp_num == 59) {
            tmp_num = 0;
        } else {
            tmp_num++;
        }
        display_state = ST_EDIT_FEED_MINUTE;
    } else if (key_pressed == KEY_MINUS) {
        if (tmp_num == 0) {
            tmp_num = 59;
        } else {
            tmp_num--;
        }
        display_state = ST_EDIT_FEED_MINUTE;
    }
}

void edit_feed_qty_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        feedings[feedIndex].quantity = tmp_num;
        display_state = ST_VIEW_FEED;
    } else if (key_pressed == KEY_PLUS) {
        if (tmp_num == 99) {
            tmp_num = 0;
        } else {
            tmp_num++;
        }
        display_state = ST_EDIT_FEED_QTY;
    } else if (key_pressed == KEY_MINUS) {
        if (tmp_num == 0) {
            tmp_num = 99;
        } else {
            tmp_num--;
        }
        display_state = ST_EDIT_FEED_QTY;
    }
}

void process_key_input() {
    switch (display_state) {
        case ST_START_SCREEN:
            start_screen_key_pressed();
            break;
        case ST_EDIT_TIME_HOUR:
            edit_hour_key_pressed();
            break;
        case ST_EDIT_TIME_MINUTE:
            edit_minute_key_pressed();
            break;

        case ST_VIEW_FEED:
            view_feed_key_pressed();
            break;

        case ST_EDIT_FEED_HOUR:
            edit_feed_hour_key_pressed();
            break;
        case ST_EDIT_FEED_MINUTE:
            edit_feed_minute_key_pressed();
            break;
        case ST_EDIT_FEED_QTY:
            edit_feed_qty_key_pressed();
            break;
    }
}

void enableInterrupts() {

    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;
    RBPU = 1; // activate pullups

    INTCONbits.INT0IE = 1; //enter button
    INTCON3bits.INT1IE = 1; //plus button
    INTCON3bits.INT2IE = 1; //minus button
    INTEDG0 = 1; //rising edge
    INTEDG1 = 0;
    INTEDG2 = 0;

    ADCON1bits.PCFG = 0b0111; //RB ports are digital

    INTCONbits.GIE = 1; //global interrupts
    INTCONbits.PEIE = 1; //perferal interrupts
}

void setupPorts() {
    TRISA = 0;
    TRISCbits.RC4 = 0;
    PORTCbits.RC4 = 1; //signal lamp
}

void writeTimeToLcd(int row, int col) {
    Lcd_Set_Cursor(row, col);
    Lcd_Write_String("Time:");
    Lcd_Write_String(str_hours);
    if (seconds & 1) {
        Lcd_Write_Char(':');
    } else {
        Lcd_Write_Char(' ');
    }
    Lcd_Write_String(str_minutes);
}

void interrupt handleInterrupt() {

    if (TMR1IE && TMR1IF) { // any timer 1 interrupts?
        TMR1IF = 0;
        TMR1 = TMR1_RESET_VALUE;
        
        tick++;
        if(tick == 1) {
            addOneSecond();
            tick = 0;
        }

        writeTimeToLcd(0, 0);
    }

    //handle button press events
    if (INT0IF) {
        INT0IF = 0;
        debounce_delay();
        if (PORTBbits.RB0) {
            key_pressed = KEY_ENTER;
            PORTCbits.RC4 ^= 1;
        }

        return;
    }
    if (INT1IF) {
        INT1IF = 0;
        debounce_delay();
        if (!PORTBbits.RB1) {
            key_pressed = KEY_PLUS;
            PORTCbits.RC4 ^= 1;
        }
        return;
    }
    if (INT2IF) {
        INT1IF = 0;
        debounce_delay();
        if (!PORTBbits.RB2) {
            key_pressed = KEY_MINUS;
            PORTCbits.RC4 ^= 1;
        }
        return;
    }


}

void renderScreen() {
    char tick_mark = tick ? ' ' : ':';

    switch (display_state) {
        case ST_START_SCREEN:
            sprintf(lcd_line_1, "Time: %d%s%d     ", hours, tick_mark, minutes);
            sprintf(lcd_line_2, "Qty:  %dg %d/day", totalQty, totalFeedings);
            /*
             ****************
            Time: 12:44     
            Qty:  40g  2/day
             ****************
             */
            break;
        case ST_EDIT_TIME_HOUR:
            if(tick){
                sprintf(lcd_line_1, "Time: %d:%d     ", tmp_num, minutes);
            } else {
                sprintf(lcd_line_1, "Time: __:%d     ", minutes);
            }
            sprintf(lcd_line_2, BLANK_LINE, totalQty, totalFeedings);
            /*
             ****************	
            Time: __:43

             ****************             
             */
            break;
        case ST_EDIT_TIME_MINUTE:
            if(tick){
                sprintf(lcd_line_1, "Time: %d:%d     ", hours, tmp_num);
            } else {
                sprintf(lcd_line_1, "Time: %d:__     ", hours);
            }
            sprintf(lcd_line_2, BLANK_LINE, totalQty, totalFeedings);
            /*
             ****************	
            Time: 12:__

             ****************             
             */
            break;

        case ST_VIEW_FEED:
            sprintf(lcd_line_1, "Feed %d: %02d:%02d   ", feedIndex + 1, feedings[feedIndex].hour, feedings[feedIndex].minute);
            sprintf(lcd_line_2, "Qty:    %dg", totalQty, totalFeedings);
            /*
             ****************
             Feed 1: 05:30    
             Qty:    20g
             ****************
             */
            break;

        case ST_EDIT_FEED_HOUR:
            if(tick){
              sprintf(lcd_line_1, "Feed %d: %02d:%02d   ", feedIndex + 1, tmp_num, feedings[feedIndex].minute);
            } else {
              sprintf(lcd_line_1, "Feed %d: __:%02d   ", feedIndex + 1, feedings[feedIndex].minute);
            }
            sprintf(lcd_line_2, "Qty:    %dg", totalQty, totalFeedings);
            /*
             ****************
             Feed 1: __:30
             Qty:    20g
             ****************
             */
            break;
        case ST_EDIT_FEED_MINUTE:
            if(tick){
              sprintf(lcd_line_1, "Feed %d: %02d:%02d   ", feedIndex + 1, feedings[feedIndex].hour, tmp_num);
            } else {
              sprintf(lcd_line_1, "Feed %d: %02d:__   ", feedIndex + 1, feedings[feedIndex].hour);
            }
            sprintf(lcd_line_2, "Qty:    %dg", totalQty, totalFeedings);
            /*
             ****************
             Feed 1: 05:__
             Qty:    20g
             ****************
             */
            break;
        case ST_EDIT_FEED_QTY:
            sprintf(lcd_line_1, "Feed %d: %02d:%02d   ", feedIndex + 1, feedings[feedIndex].hour, feedings[feedIndex].minute);
            if(tick){
              sprintf(lcd_line_2, "Qty:    %02dg", totalQty, totalFeedings);
            } else {
              sprintf(lcd_line_2, "Qty:    __g", totalQty, totalFeedings);
          }
            /*
             ****************
             Feed 1: 05:30
             Qty:    __g
             ****************
             */
            break;
    }

    Lcd_Home();
    Lcd_Clear();
    Lcd_Write_String(lcd_line_1);
    Lcd_Set_Cursor(1, 0);
    Lcd_Write_String(lcd_line_2);

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
