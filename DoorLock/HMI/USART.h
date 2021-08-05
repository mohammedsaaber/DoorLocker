/*
 * USART.h
 *
 *  Created on: Aug 3, 2021
 *      Author: MohamedSaber
 */

#ifndef USART_H_
#define USART_H_

#include "std_types.h"

void UART_init(uint16 baudRate);

void UART_sendByte(uint8 Data);

uint8 UART_recieveByte(void);

void UART_sendString(uint8 * str);

void UART_recieveString(uint8 * str);

#endif /* USART_H_ */
