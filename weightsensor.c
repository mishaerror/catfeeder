/* 
 * File:   weightsensor.c
 * Author: Milos
 *
 * Created on September 23, 2017, 8:36 PM
 */
/* 
 * File:   weightsensor.h
 * Author: Milos
 * 
 * Read weight information
 *
 * Created on September 22, 2017, 8:37 PM
 */
#include "xcincludes.h"
#include "weightsensor.h"

long weightOffset = 0;

void initHX711() {
    TRISCbits.RC6 = 0; //clock for serial data
    TRISCbits.RC7 = 1; //input for serial data
    weightOffset = 0;
}

long readSerialData() {
    unsigned long serialData = 0x0;

    SERIAL_DATA_IN = 1;
    SERIAL_CLK_OUT = 0;
    serialData = 0;
    while (SERIAL_DATA_IN);
    for (unsigned char i = 0; i < 24; i++) {
        SERIAL_CLK_OUT = 1;
        serialData = serialData << 1;
        SERIAL_CLK_OUT = 0;
        if (SERIAL_DATA_IN) serialData++;
    }

    for (unsigned short i = 0; i < GAIN; i++) {
        SERIAL_CLK_OUT = 1;
        SERIAL_CLK_OUT = 0;
    }

    SERIAL_CLK_OUT = 1;
    serialData = serialData ^ 0x800000;
    SERIAL_CLK_OUT = 0;
    return (serialData);
}

long read_average(int times) {
    long sum = 0;
    for (int i = 0; i < times; i++) {
        sum += readSerialData();
    }
    return sum / times;
}

long get_value(int times) {
	return read_average(times) - weightOffset;
}

void tare(int times) {
	long sum = read_average(times);
	weightOffset = sum;
}

void power_down() {
    SERIAL_CLK_OUT = 0;
    SERIAL_CLK_OUT = 1;
}

void power_up() {
    SERIAL_CLK_OUT = 0;
}

