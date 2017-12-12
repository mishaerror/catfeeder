/* 
 * File:   realtimeclock.h
 * Author: Milos
 *
 * Created on September 22, 2017, 8:36 PM
 */

#define TMR1_RESET_VALUE 0x8000

unsigned char seconds = 0;
unsigned char minutes = 0;
unsigned char hours = 0;

void setupRealTimeClock();

void addOneSecond();

void writeTimeToEeprom();

void readTimeFromEeprom();

void addOneMinute();

void addOneHour();

void addSeconds(char nofSeconds);