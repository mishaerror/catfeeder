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
inline void numberToLcdStr(unsigned char time, unsigned char* timestr);
inline char digitToLcdChar(char number);
inline char timeAfter(char h1, char m1, char h2, char m2);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

