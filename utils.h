/* 
 * File:   utils.h
 * Author: Milos
 *
 * Created on December 2, 2017, 4:52 PM
 */

#ifndef UTILS_H
#define	UTILS_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

void eepromWrite(unsigned int addr, unsigned char byte);
unsigned char eepromRead(unsigned int addr);
void timeToDigit(char time, char* timestr);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

