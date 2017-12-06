/* 
 * File:   eusart.h
 * Author: Milos
 *
 * Created on December 5, 2017, 8:09 PM
 */

#ifndef EUSART_H
#define	EUSART_H

#include <xc.h>
#include "xcincludes.h"

#ifdef	__cplusplus
extern "C" {
#endif


void UART_Init(const long int baudrate);

unsigned char UART_Read();

#ifdef	__cplusplus
}
#endif

#endif	/* EUSART_H */

