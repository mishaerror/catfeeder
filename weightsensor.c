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
#include "weightsensor.h"
#include "xcincludes.h"

#define SCALE_FACTOR 2100
#define NOF_SAMPLES 8

#define HX_SCK LATCbits.LATC6
#define HX_DT PORTCbits.RC7

void initHX711() {
    TRISCbits.RC6 = 0; //clock for serial data
    TRISCbits.RC7 = 1; //input for serial data
    HX_SCK = 1;
}

void turnOnHX711() {
    HX_SCK = 0;
}

void turnOffHX711() {
    HX_SCK = 1;
}

long getWeight() {
    long data = 0;
    long dataSum = 0x00;

    turnOnHX711();
    
    while(HX_DT == 1);  // wait HX711 ready
    // dummy conversion next : channel A@128
    for(char i=0; i<25; i++){
        HX_SCK = 1;
        HX_SCK = 0;
    }

    for(char averaging  = 0; averaging < NOF_SAMPLES; averaging++) {
        while(HX_DT == 1);  // wait HX711 ready
        data = 0;
        for(char i=0; i<24; i++){
            data = data << 1;
            HX_SCK = 1;
            HX_SCK = 0;
            if (HX_DT) data += 1;
        }
        HX_SCK = 1;
        HX_SCK = 0;         // next conversion channel A:128
        //pad with sign
        if(data & 0x800000) {
            data |= 0xFF000000;
        }
        data = data ^ 0x80000000;
        
        dataSum += data;
    }
    
    turnOffHX711();

    return (dataSum / NOF_SAMPLES)/SCALE_FACTOR;
}

