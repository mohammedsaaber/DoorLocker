/*
 * HMI.c
 *
 *  Created on: Aug 5, 2021
 *      Author: MohamedSaber
 */

#include "micro_config.h"
#include "Keypad.h"
#include "LCD.h"
#include "USART.h"

 /**************************** Prototypes ****************************************/

void InsertPassword(void);

void PasswordChecking(uint8 option);

void OpenDoor(void);

void ChangePassword(void);

void Options(uint8 option);


/********************************************************************************/

/****************************** Macros ******************************************/

#define MAX_PASSWORD 8
#define RECIEVER_IS_READY 10
#define WRONG_PASSWORD 1
#define RIGHT_PASSWORD 0

/********************************************************************************/

/******************************* Global Variables *******************************/
uint8 key = 0;
uint8 Counter = 0;
uint8 Password_Checking_Flag = RIGHT_PASSWORD;

/********************************************************************************/

int main(void)
{

	LCD_init();
	UART_init(9600);
	while (UART_recieveByte() != RECIEVER_IS_READY);
	InsertPassword();


	while(1)
	{

	}
}

void InsertPassword(void)
{

	Counter = 0;
	Password_Checking_Flag = RIGHT_PASSWORD;
	LCD_clearScreen();
	LCD_displayString("Enter Pass:");
	LCD_goToRowColumn(2,0);

	while (Counter != MAX_PASSWORD)
	{
		key = KeyPad_getPressedKey();
		LCD_displayCharacter('*');
		UART_sendByte(key);
		Counter++;
		_delay_ms(250);
	}

	Counter = 0;

	LCD_clearScreen();
	LCD_displayString("Re-enter Pass:");
	LCD_goToRowColumn(2,0);

	while (Counter != MAX_PASSWORD)
	{
		key = KeyPad_getPressedKey();
		LCD_displayCharacter('*');
		UART_sendByte(key);
		Counter++;
		_delay_ms(250);
	}

	UART_sendByte(RECIEVER_IS_READY);
	Password_Checking_Flag = UART_recieveByte();
	LCD_clearScreen();

	if (Password_Checking_Flag == WRONG_PASSWORD)
	{
		LCD_goToRowColumn(0,4);
		LCD_displayString("UN-MATCHED");
		_delay_ms(500);
		InsertPassword();
	}
	else
	{
		LCD_goToRowColumn(0,4);
		LCD_displayString("MATCHED");
		_delay_ms(500);

		LCD_clearScreen();
		LCD_displayString("+ : Open Door");
		LCD_goToRowColumn(2,0);
		LCD_displayString("- : Change Pass");

		do
		{
			key = KeyPad_getPressedKey();

		}while ((key != '+') && (key != '-'));

		if (key == '+')
		{
			OpenDoor();
		}
		else
		{
			ChangePassword();
		}

	}

}

void PasswordChecking(uint8 option)
{
	Counter = 0;
	LCD_clearScreen();
	_delay_ms(100);

	if (option == '+')
	{
		LCD_displayString("Enter Pass:");
	}
	else
	{
		LCD_displayString("Enter Old Pass:");
	}

	LCD_goToRowColumn(2,0);


	while (Counter != MAX_PASSWORD)
	{
		key = KeyPad_getPressedKey();
		LCD_displayCharacter('*');
		UART_sendByte(key);
		Counter++;
		_delay_ms(250);
	}

	Password_Checking_Flag = UART_recieveByte();

	if (Password_Checking_Flag == RIGHT_PASSWORD)
	{
		Options(option);
	}
	else
	{
		PasswordChecking(option);
	}
}

void OpenDoor(void)
{

	UART_sendByte('+');
	PasswordChecking('+');

}

void ChangePassword(void)
{
	UART_sendByte('-');
	PasswordChecking('-');
}

void Options(uint8 option)
{
	LCD_clearScreen();

	if (option == '+')
	{
		LCD_goToRowColumn(0,4);
		LCD_displayString("OPENED..");
	}
	else
	{
		InsertPassword();
	}
}
