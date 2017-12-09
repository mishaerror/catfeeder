/* 
 * File:   i2c.h
 * Author: Milos
 * 
 * Created on December 1, 2017, 7:54 PM
 * 
 * 
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif


void i2cInit(const unsigned long c);

void i2cWait();

void i2cStart();

void i2cRepeatedStart();

void i2cStop();

void i2cWrite(unsigned d);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

