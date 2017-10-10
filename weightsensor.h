/* 
 * File:   weightsensor.h
 * Author: Milos
 * 
 * Read weight information
 *
 * Created on September 22, 2017, 8:37 PM
 */
#include "xcincludes.h"

#define SERIAL_CLK_OUT PORTCbits.RC6
#define SERIAL_DATA_IN PORTCbits.RC7
#define GAIN 1 //x128

void initHX711();

long readSerialData();

long read_average(int times);

long get_value(int times);

void tare(int times);

void power_down();

void power_up();