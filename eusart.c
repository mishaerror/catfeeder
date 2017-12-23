#include "eusart.h"

void UART_Init(const long baudrate){
	
    TRISC7 = 1;
    TRISC6 = 1;


	SYNC = 1;//synchorous
	SPEN = 1;//serial
    CSRC = 1;//master

    CREN = 1;//continous reception
    RCIE = 1;//enable interrupt
}


unsigned char UART_Read(){
 
  while(!RCIF);
  return RCREG;
}