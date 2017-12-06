#include "eusart.h"

void UART_Init(const long int baudrate){
	unsigned int x;
	x = (_XTAL_FREQ - baudrate*16)/(baudrate*16);
	BRGH = 1;
	SPBRG = x;
	SYNC = 0;
	SPEN = 1;
    TRISC7 = 1;
    TRISC6 = 1;
    CREN = 1;
    TXEN = 1;
}


unsigned char UART_Read(){
 
  while(!RCIF);
  return RCREG;
}