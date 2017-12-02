/* 
 * File:   utils.h
 * Author: Milos
 *
 * Created on December 2, 2017, 4:52 PM
 */
#include <xc.h>

#ifndef UTILS_H
#define	UTILS_H

#ifdef	__cplusplus
extern "C" {
#endif

void write_eeprom(unsigned int addr, unsigned char byte);
unsigned char read_eeprom(unsigned int addr);

#ifdef	__cplusplus
}
#endif

#endif	/* UTILS_H */

