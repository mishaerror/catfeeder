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
#include "stepper_motor.h"
#include "utils.h"
#include "weightsensor.h"

#define BLANK_LINE "                "

enum KEY_PRESSED_t {
    KEY_ENTER,
    KEY_PLUS,
    KEY_MINUS
} key_pressed;

//states

typedef enum {
    ST_START_SCREEN,
    ST_EDIT_TIME_HOUR,
    ST_EDIT_TIME_MINUTE,
    ST_VIEW_FEED,
    ST_EDIT_FEED_HOUR,
    ST_EDIT_FEED_MINUTE,
    ST_EDIT_FEED_QTY,
    ST_LOADING_FOOD
} DISPLAY_STATE_t;

typedef struct {
    unsigned char hour;
    unsigned char minute;
    unsigned char quantity;
} FEEDING_t;

#define NOF_FEEDINGS 4;
#define FEEDINGS_EEPROM_ADDR 0;

FEEDING_t feedings[4] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0}
};
unsigned char feedIndex = 0;
unsigned char tmp_num; //hold value of currently edited number
char str_tmp_num[] = "  ";
unsigned int totalQty = 0;
char str_total_qty[] = " 0";
unsigned char loaded_qty = 0;
unsigned char totalFeedings = 0;

long weight_tare;

DISPLAY_STATE_t display_state;

unsigned wasSleeping = 0;

#define SLEEP_TIMEOUT 15
unsigned char sleepCounter = 0;

void goToSleep() {
    if (wasSleeping) {
        Sleep();
        return;
    }
    //turn off periferrals
    turnOffHX711();
    lcdOn(0);
    wasSleeping = 1;
    Sleep();
}

void wakeUp() {
    if (!wasSleeping) {
        return;
    }
    wasSleeping = 0;
    lcdOn(1);
    sleepCounter = 0;
}

void renderScreenTemplate(DISPLAY_STATE_t state) {
    lcdClear();
    lcdHome();
    switch (state) {
        case ST_START_SCREEN:
        case ST_EDIT_TIME_HOUR:
        case ST_EDIT_TIME_MINUTE:
            lcdWriteString("Time:   :       ");
            lcdSetCursor(1, 0);
            lcdWriteString("Qty:    g   /day");
            break;

        case ST_VIEW_FEED:
        case ST_EDIT_FEED_HOUR:
        case ST_EDIT_FEED_MINUTE:
        case ST_EDIT_FEED_QTY:
            lcdWriteString("Feed   :   :    ");
            lcdSetCursor(1, 0);
            lcdWriteString("Qty:       g    ");
            break;
        case ST_LOADING_FOOD:
            lcdWriteString("Loading feed    ");
            lcdSetCursor(1, 0);
            lcdWriteString("Qty:   /  g     ");
            break;
    }
}

unsigned char getTotalQty() {
    char result = 0;
    for (char i = 0; i < 4; i++) {
        result += feedings[i].quantity;
    }

    return result;
}

unsigned char getTotalFeedings() {
    char result = 0;
    for (char i = 0; i < 4; i++) {
        if (feedings[i].quantity > 0) {
            result++;
        }
    }

    return result;
}

void start_screen_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        display_state = ST_EDIT_TIME_HOUR;
        tmp_num = hours;
    } else if (key_pressed == KEY_PLUS) {
        display_state = ST_VIEW_FEED;
        feedIndex = 0;
        renderScreenTemplate(display_state);
    } else if (key_pressed == KEY_MINUS) {
        display_state = ST_VIEW_FEED;
        feedIndex = 3;
        renderScreenTemplate(display_state);
    }
}

void edit_hour_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        display_state = ST_EDIT_TIME_MINUTE;
        hours = tmp_num;
        writeTimeToEeprom();
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
        writeTimeToEeprom();
        seconds = 0;
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
            renderScreenTemplate(display_state);
        } else {
            feedIndex++;
            display_state = ST_VIEW_FEED;
        }
        renderScreenTemplate(display_state);
    } else if (key_pressed == KEY_MINUS) {
        display_state = ST_VIEW_FEED;
        if (feedIndex == 0) {
            display_state = ST_START_SCREEN;
            renderScreenTemplate(display_state);
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
        tmp_num = feedings[feedIndex].minute;
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
        if (tmp_num >= 59) {
            tmp_num = 0;
        } else {
            tmp_num += 10;
        }
        display_state = ST_EDIT_FEED_MINUTE;
    } else if (key_pressed == KEY_MINUS) {
        if (tmp_num == 0) {
            tmp_num = 50;
        } else {
            tmp_num -= 10;
        }
        display_state = ST_EDIT_FEED_MINUTE;
    }
}

void write_feed_to_eeprom(unsigned char feedIndex) {
    unsigned char feed_address = feedIndex * 3;
    eepromWrite(feed_address, feedings[feedIndex].hour);
    eepromWrite(feed_address + 1, feedings[feedIndex].minute);
    eepromWrite(feed_address + 2, feedings[feedIndex].quantity);
}

void read_feed_from_eeprom(char feedIndex) {
    unsigned char feed_address = feedIndex * 3;
    feedings[feedIndex].hour = eepromRead(feed_address);
    if (feedings[feedIndex].hour > 23) {
        feedings[feedIndex].hour = 0;
    }
    feedings[feedIndex].minute = eepromRead(feed_address + 1);
    if (feedings[feedIndex].minute > 59) {
        feedings[feedIndex].minute = 0;
    }
    feedings[feedIndex].quantity = eepromRead(feed_address + 2);
    if (feedings[feedIndex].quantity > 99) {
        feedings[feedIndex].quantity = 0;
    }
}

void edit_feed_qty_key_pressed() {
    if (key_pressed == KEY_ENTER) {
        feedings[feedIndex].quantity = tmp_num;
        display_state = ST_VIEW_FEED;
        write_feed_to_eeprom(feedIndex);
        totalQty = getTotalQty();
        totalFeedings = getTotalFeedings();
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
    INTEDG0 = 0; //falling edge
    INTEDG1 = 0;
    INTEDG2 = 0;

    ADCON1bits.PCFG = 0b0111; //RB ports are digital

    INTCONbits.PEIE = 1; //perferal interrupts
    INTCONbits.GIE = 1; //global interrupts
}

void setupPorts() {
    TRISA = 0;
    TRISCbits.RC3 = 0;
    TRISCbits.RC4 = 0;
    TRISCbits.RC5 = 0;
    LATC5 = 1; //signal lamp
}

void write_loading_screen(unsigned char feed, unsigned char qty) {
    /*
     ****************
        Loading feed 1
        Qty: 02/25g
     ****************
     * */
    char str_qty[] = "  ";

    if (weight_tare == 0) {
        weight_tare = getWeight();
    }
    long value = getWeight() - weight_tare;

    lcdSetCursor(0, 13);
    lcdWriteChar('1');
    lcdSetCursor(1, 5);
    //timeToDigit(qty, str_qty);
    //lcdWriteString(str_qty);
    if (value < 10) {
        lcdWriteChar('0');
    }
    lcdWriteString(str_qty);
    //lcdSetCursor(1, 8);
    //timeToDigit(feedings[0].quantity, str_qty);
    //lcdWriteString(str_qty);
}

void writeStartScreen(const char * hour, const char* minute, const char* qty, const char times) {
    char secs[] = "  ";
    /*
     ****************
     Time: 12:44     
     Qty:  40g  2/day
     ****************
     */
    lcdSetCursor(0, 6);
    lcdWriteString(hour);
    lcdWriteChar(':');
    lcdWriteString(minute);
    lcdWriteChar(':');
    timeToDigit(seconds, secs);
    lcdWriteString(secs);
    lcdSetCursor(1, 6);
    lcdWriteString(qty);
    lcdSetCursor(1, 11);
    lcdWriteChar(times);
}

void write_feeding_screen(char feedNo, const char* feedHour, const char* feedMinute, const char* feedQty) {
    /*
     ****************
     Feed 1: 05:30    
     Qty:    20g
     ****************
     */
    lcdSetCursor(0, 5);
    lcdWriteChar(feedNo);
    lcdSetCursor(0, 8);
    lcdWriteString(feedHour);
    lcdSetCursor(0, 11);
    lcdWriteString(feedMinute);
    lcdSetCursor(1, 9);
    lcdWriteString(feedQty);
}

void updateScreen() {
    char str_minutes[] = "00";
    char str_hours[] = "00";
    char feed_hour[] = "  ";
    char feed_minute[] = "  ";
    char feed_qty[] = "  ";
    timeToDigit(feedings[feedIndex].hour, feed_hour);
    timeToDigit(feedings[feedIndex].minute, feed_minute);
    timeToDigit(feedings[feedIndex].quantity, feed_qty);

    timeToDigit(hours, str_hours);
    timeToDigit(minutes, str_minutes);
    timeToDigit(totalQty, str_total_qty);
    timeToDigit(tmp_num, str_tmp_num);

    lcdHome();

    switch (display_state) {
        case ST_START_SCREEN:
            writeStartScreen(str_hours, str_minutes,
                    str_total_qty, digitToChar(totalFeedings));
            lcdCursor(0);
            break;
        case ST_EDIT_TIME_HOUR:
            writeStartScreen(str_tmp_num, str_minutes,
                    str_total_qty, digitToChar(totalFeedings));
            lcdSetCursor(0, 7);
            lcdCursor(1);
            break;
        case ST_EDIT_TIME_MINUTE:
            writeStartScreen(str_hours, str_tmp_num,
                    str_total_qty, digitToChar(totalFeedings));
            lcdSetCursor(0, 10);
            lcdCursor(1);
            break;

        case ST_VIEW_FEED:
            write_feeding_screen(digitToChar(feedIndex + 1), feed_hour, feed_minute, feed_qty);
            lcdCursor(0);
            break;

        case ST_EDIT_FEED_HOUR:
            write_feeding_screen(digitToChar(feedIndex + 1), str_tmp_num, feed_minute, feed_qty);
            lcdSetCursor(0, 9);
            lcdCursor(1);
            break;
        case ST_EDIT_FEED_MINUTE:
            write_feeding_screen(digitToChar(feedIndex + 1), feed_hour, str_tmp_num, feed_qty);
            lcdSetCursor(0, 12);
            lcdCursor(1);
            break;
        case ST_EDIT_FEED_QTY:
            write_feeding_screen(digitToChar(feedIndex + 1), feed_hour, feed_minute, str_tmp_num);
            lcdSetCursor(1, 10);
            lcdCursor(1);
            break;
        case ST_LOADING_FOOD:
            write_loading_screen(0, 0);
            break;

    }
}

void interrupt handleInterrupt() {

    if (TMR1IE && TMR1IF) { // any timer 1 interrupts?
        TMR1IF = 0;
        TMR1 = TMR1_RESET_VALUE;
        LATC5 ^= 1;
        addOneSecond();
        sleepCounter++;
        if (wasSleeping || sleepCounter >= SLEEP_TIMEOUT) {
            goToSleep();
        } else {
            updateScreen();
        }
        return;
    }

    //handle button press events. If waking up from sleep, don't process regular key event
    if (INT0IF) {
        INT0IF = 0;
        if (!PORTBbits.RB0) {
            if (wasSleeping) {
                wakeUp();
            } else {
                key_pressed = KEY_ENTER;
                sleepCounter = 0;
                process_key_input();
                updateScreen();
            }
        }
        return;
    }
    if (INT1IF) {
        INT1IF = 0;
        if (!PORTBbits.RB1) {
            if (wasSleeping) {
                wakeUp();
            } else {
                sleepCounter = 0;
                key_pressed = KEY_PLUS;
                process_key_input();
                updateScreen();
           }
        }
        return;
    }
    if (INT2IF) {
        INT2IF = 0;
        if (!PORTBbits.RB2) {
            if (wasSleeping) {//woken up, go back to sleep
                wakeUp();
            } else {
                sleepCounter = 0;
                key_pressed = KEY_MINUS;
                process_key_input();
                updateScreen();
            }
        }
        return;
    }

    if (TMR3IF) {
        TMR3IF = 0;

        motorStep();

        return;
    }

}

void reload_feedings() {
    unsigned char i;
    for (i = 0; i < 4; i++) {
        read_feed_from_eeprom(i);
    }
    totalQty = getTotalQty();
    totalFeedings = getTotalFeedings();
}

void main(void) {
    reload_feedings();
    setupPorts();

    //after ports are initiated, set up lcd and render initial screen
    lcdInit();
    lcdClear();

    display_state = ST_START_SCREEN;
    renderScreenTemplate(display_state);

    //enable weight sensor and clock
    initHX711();

    setupRealTimeClock();

    enableInterrupts();

    goToSleep(); //sleep by default, wake up on button press or on WDT
    while (1) {
    }
}
