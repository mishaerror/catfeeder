/* 
 * File:   i2c.h
 * Author: Milos
 *
 * Created on December 1, 2017, 7:54 PM
 */

#ifndef I2C_H
#define	I2C_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif


void I2C_Master_Init(const unsigned long c)
{
  SSPCON1 = 0b00101000;
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*c))-1;
  SSPSTAT = 0;
  TRISC3 = 1;        //Setting as input as given in datasheet
  TRISC4 = 1;        //Setting as input as given in datasheet
}

void I2C_Master_Wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}

void I2C_Master_Start()
{
  I2C_Master_Wait();    
  SEN = 1;             //Initiate start condition
}

void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  RSEN = 1;           //Initiate repeated start condition
}

void I2C_Master_Stop()
{
  I2C_Master_Wait();
  PEN = 1;           //Initiate stop condition
}

void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  SSPBUF = d;         //Write data to SSPBUF
}

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

