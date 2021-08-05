/*
 * Control.c
 *
 *  Created on: Aug 5, 2021
 *      Author: MohamedSaber
 */

#include "micro_config.h"
#include "external_eeprom.h"
#include "USART.h"

/********************************** Prototypes ****************************************/

void Recieve_Password(void);

void OpenDoor(void);

void ChangePassword(void);

void PasswordChecking(void);

/*************************************************************************************/

/*********************************** Macros ******************************************/

#define EEPROM_ADDRESS 0x0311
#define MAX_PASSWORD 8
#define RECIEVER_IS_READY 10
#define WRONG_PASSWORD 1
#define RIGHT_PASSWORD 0

/*************************************************************************************/

/********************************* Global Variables **********************************/

uint8 counter = 0;
uint8 Password_Checking = 0;
uint8 Password_Checking_Flag = RIGHT_PASSWORD;
uint8 key = 0;
uint16 Ptr2EEPROM = EEPROM_ADDRESS;

/*************************************************************************************/


int main(void)
{
	DDRB = 0xFF;
	DDRA = 0xFF;
	PORTA = 0;
	PORTB = 0;
	UART_init(9600);
	EEPROM_init();
	_delay_ms(100);
	UART_sendByte(RECIEVER_IS_READY);
	Recieve_Password();


	while(1)
	{
	}
}

void Recieve_Password(void)
{
	uint8 counter = 0;
	uint8 Op = 0;
	Ptr2EEPROM = EEPROM_ADDRESS;
	Password_Checking_Flag = RIGHT_PASSWORD;

	while (counter != MAX_PASSWORD)
	{
		key = UART_recieveByte();
		EEPROM_writeByte(Ptr2EEPROM, key);
		counter++;
		Ptr2EEPROM++;
	}

	counter = 0;
	Ptr2EEPROM = EEPROM_ADDRESS;

	while (counter != MAX_PASSWORD)
	{
		key = UART_recieveByte();
		_delay_ms(10);
		EEPROM_readByte(Ptr2EEPROM , &Password_Checking);
		counter++;
		Ptr2EEPROM++;
		if (Password_Checking != key)
		{
			Password_Checking_Flag = WRONG_PASSWORD;
		}

	}

	while (UART_recieveByte() != RECIEVER_IS_READY);
	UART_sendByte(Password_Checking_Flag);

	if (Password_Checking_Flag == WRONG_PASSWORD)
	{
		Recieve_Password();
	}

	Op = UART_recieveByte();

	if (Op == '+')
	{
		OpenDoor();
	}
	else
	{
		ChangePassword();
	}

}

void OpenDoor(void)
{

	PasswordChecking();

	if (Password_Checking_Flag == RIGHT_PASSWORD)
	{
		PORTB |= (1 << PB0);
	}
	else
	{
		OpenDoor();
	}

}

void PasswordChecking(void)
{
	static uint8 Trials = 0;
	Ptr2EEPROM = EEPROM_ADDRESS;
	counter = 0;
	Password_Checking_Flag = RIGHT_PASSWORD;

	while (counter != MAX_PASSWORD)
	{
		key = UART_recieveByte();
		_delay_ms(10);

		EEPROM_readByte(Ptr2EEPROM , &Password_Checking);
		counter++;
		Ptr2EEPROM++;

		if (Password_Checking != key)
		{
			Password_Checking_Flag = WRONG_PASSWORD;
		}

	}

	UART_sendByte(Password_Checking_Flag);

	Trials++;

	if (Trials == 3)
	{
		PORTA = 0xFF;
	}
}

void ChangePassword(void)
{
	PasswordChecking();

	if (Password_Checking_Flag == RIGHT_PASSWORD)
	{
		Recieve_Password();
	}
	else
	{
		ChangePassword();
	}
}
