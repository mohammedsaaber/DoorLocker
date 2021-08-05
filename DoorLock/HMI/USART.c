/*
 * USART.c
 *
 *  Created on: Aug 3, 2021
 *      Author: MohamedSaber
 */

#include "USART.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8 g_UART_Transmitted_Data_Flag = 0;
volatile uint8 g_UART_Recieved_Data_Flag = 0;
volatile uint8 g_UART_Recieved_Data = 0;

ISR (USART_TXC_vect)
{
	g_UART_Transmitted_Data_Flag = 1;
}

ISR (USART_RXC_vect)
{
	g_UART_Recieved_Data_Flag = 1;
	g_UART_Recieved_Data = UDR;
}


void UART_init(uint16 baudRate)
{

	SREG |= (1 << 7);

	/*
	 * Enable Receiver
	 * Enable Transmitter
	 * Enable interrupts for RX & TX
	 * */
	UCSRB |= (1 << RXEN) | (1 << TXEN) | (1 << TXCIE) | (1 << RXCIE);
	/*
	 * Asynchronous mode
	 * 8 bits character size
	 * 1 stop bit
	 * No Parity
	 * */
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	/*
	 * UBRR Register calculations
	 * */

	UBRRL = (F_CPU / (16 * baudRate)) - 1;

	DDRD |= (1 << PD1);
	DDRD &= (~(1 << PD0));


}

void UART_sendByte(uint8 Data)
{
	UDR = Data;
	while (g_UART_Transmitted_Data_Flag == 0);
	g_UART_Transmitted_Data_Flag = 0;
}

uint8 UART_recieveByte(void)
{
	while (g_UART_Recieved_Data_Flag == 0);
	g_UART_Recieved_Data_Flag = 0;
	return g_UART_Recieved_Data;
}

void UART_sendString(uint8 * str)
{
	uint8 i = 0;

	while (str[i] != '\0')
	{
		UART_sendByte(str[i]);
		i++;
	}
}

void UART_recieveString(uint8 * str)
{
	uint8 i = 0;
	str[i] = UART_recieveByte();

	while (str[i] != '#')
	{
		i++;
		str[i] = UART_recieveByte();
	}

	str[i] = '\0';

}
