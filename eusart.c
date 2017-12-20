#include "eusart.h"

void UART_Init(const long baudrate){
	unsigned int x;
	x = (_XTAL_FREQ - baudrate*16)/(baudrate*16);
	SPBRG = x;//baud rate

    TRISC7 = 1;
    TRISC6 = 1;

    CREN = 1;//continous reception
    RCIE = 1;//enable interrupt

	SYNC = 1;//synchorous
	SPEN = 1;//serial
    CSRC = 1;//master
}


unsigned char UART_Read(){
 
  while(!RCIF);
  return RCREG;
}