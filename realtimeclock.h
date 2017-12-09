/* 
 * File:   realtimeclock.h
 * Author: Milos
 *
 * Created on September 22, 2017, 8:36 PM
 */

#define TMR1_RESET_VALUE 0xC000

unsigned char seconds = 0;
unsigned char minutes = 0;
unsigned char hours = 0;

//tick is updated on half-second
char tick = 0;

void setupRealTimeClock();

void addOneSecond();

void writeTimeToEeprom();

void readTimeFromEeprom();