#include "i2c.h"
#include "xcincludes.h"

void i2cInit(const unsigned long c)
{
  SSPCON1 = 0b00101000;
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*c))-1;
  SSPSTAT = 0;
  TRISC3 = 1;        //Setting as input as given in datasheet
  TRISC4 = 1;        //Setting as input as given in datasheet
}

void i2cWait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}

void I2cStart()
{
  i2cWait();    
  SEN = 1;             //Initiate start condition
}

void i2cRepeatedStart()
{
  i2cWait();
  RSEN = 1;           //Initiate repeated start condition
}

void i2cStop()
{
  i2cWait();
  PEN = 1;           //Initiate stop condition
}

void i2cWrite(unsigned d)
{
  i2cWait();
  SSPBUF = d;         //Write data to SSPBUF
}

