/* 
 * File:   realtimeclock.h
 * Author: Milos
 *
 * Created on September 22, 2017, 8:36 PM
 */
#include "xcincludes.h"

unsigned char seconds = 0;
unsigned char minutes = 0;
unsigned char hours = 0;

void setupRealTimeClock();

void addOneSecond();
