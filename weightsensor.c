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
#include "stepper_motor.h"

#define SCALE_FACTOR 2100
#define NOF_SAMPLES 3

#define HX_SCK LATCbits.LATC6
#define HX_DT PORTCbits.RC7

typedef enum {
    WAIT_HX_READY,
    HX_READ_BITS,
    HX_GET_AVERAGE        
} HX_READ_STATE_t;

HX_READ_STATE_t hxReadState = WAIT_HX_READY;

long hxTmpData;
char hxDataCounter;
char hxSampleIndex = 0;
long hxSamples = 0;
long hxWeight;

char isHxReadInProgress() {
    return hxReadState == HX_READ_BITS;
}

void hxResetRead() {
    hxReadState = WAIT_HX_READY;
}

char hxCheckRead() {
    switch(hxReadState) {
        case WAIT_HX_READY:
            if(HX_DT == 0) {
                hxReadState = HX_READ_BITS;
                hxTmpData = 0;
                hxDataCounter = 0;
            }
            break;
        case HX_READ_BITS:
            hxTmpData = hxTmpData << 1;
            HX_SCK = 1;
            HX_SCK = 0;
            if (HX_DT) {
                hxTmpData += 1;
            }
            hxDataCounter++;
            if(hxDataCounter>23){
                HX_SCK = 1;
                HX_SCK = 0;         // next conversion channel A:128
                //pad with sign
                if(hxTmpData & 0x800000) {
                    hxTmpData |= 0xFF000000;
                }
                hxTmpData = hxTmpData ^ 0x80000000;
                hxSamples += (hxTmpData/SCALE_FACTOR);
                hxSampleIndex++;
                if(hxSampleIndex>= NOF_SAMPLES) {
                    hxSampleIndex = 0;
                    hxWeight = hxSamples/NOF_SAMPLES;
                    hxSamples = 0;
                }
                hxReadState = WAIT_HX_READY;
                return 1;
            }
            break;
    }
    return 0;
}

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
//    long data = 0;
//    long dataSum = 0x00;
//    
//    while(HX_DT);  // wait HX711 ready
//
//    for(char averaging  = 0; averaging < NOF_SAMPLES; averaging++) {
//        while(HX_DT == 1);  // wait HX711 ready
//        data = 0;
//        for(char i=0; i<24; i++){
//            data = data << 1;
//            HX_SCK = 1;
//            HX_SCK = 0;
//            if (HX_DT) data += 1;
//        }
//        HX_SCK = 1;
//        HX_SCK = 0;         // next conversion channel A:128
//        //pad with sign
//        if(data & 0x800000) {
//            data |= 0xFF000000;
//        }
//        data = data ^ 0x80000000;
//        
//        dataSum += data;
//    }
//    for(int i = 0; i < NOF_SAMPLES; i++) {
//        dataSum+= hxSamples[i];
//    }
    return (hxWeight);
}

