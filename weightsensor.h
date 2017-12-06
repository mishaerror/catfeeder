/* 
 * File:   weightsensor.h
 * Author: Milos
 * 
 * Read weight information
 *
 * Created on September 22, 2017, 8:37 PM
 */
#include "xcincludes.h"

void initHX711();

signed long getWeight();

void turnOnHX711();

void turnOffHX711();
