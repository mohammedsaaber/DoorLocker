/*
 * LCD.c
 *
 *  Created on: Jul 29, 2021
 *      Author: MohamedSaber
 */

#include <avr/io.h>
#include "LCD.h"

void LCD_init(void)
{
	LCD_CONTROL_PORT_DIRECTION |= (1 << RS) | (1 << RW) | (1 << E);
	LCD_DATA_PORT_DIRECTION = 0xFF;
	LCD_sendCommand(CLEAR_SCREEN);
	LCD_sendCommand(DISPLAY_ON_CURSOR_OFF);
}

void LCD_sendCommand(uint8 Command)
{
	LCD_CONTROL_PORT_OUTPUT &= (~(1 << RW));
	LCD_CONTROL_PORT_OUTPUT &= (~(1 << RS));
	LCD_CONTROL_PORT_OUTPUT &= (~(1 << E));
	_delay_ms(1);
	LCD_CONTROL_PORT_OUTPUT |= (1 << E);
	_delay_ms(1);
	LCD_DATA_PORT_OUTPUT = Command;
	_delay_ms(1);
	LCD_CONTROL_PORT_OUTPUT &= (~(1 << E));
	_delay_ms(1);
}

void LCD_displayCharacter(uint8 Character)
{
	LCD_CONTROL_PORT_OUTPUT &= (~(1 << RW));
	LCD_CONTROL_PORT_OUTPUT |= (1 << RS);
	LCD_CONTROL_PORT_OUTPUT &= (~(1 << E));
	_delay_ms(1);
	LCD_CONTROL_PORT_OUTPUT |= (1 << E);
	_delay_ms(1);
	LCD_DATA_PORT_OUTPUT = Character;
	_delay_ms(1);
	LCD_CONTROL_PORT_OUTPUT &= (~(1 << E));
	_delay_ms(1);
}

void LCD_displayString(uint8 * String)
{
	uint8 i = 0;

	while (String[i] != '\0')
	{
		LCD_displayCharacter(String[i]);
		i++;
	}
}

void LCD_goToRowColumn(uint8 Row, uint8 Column)
{
	uint8 Address = 0;

	switch (Row)
	{

	case 0: Address = Column;
	break;

	case 1: Address = Column + 0x40;
	break;

	case 2: Address = Column + 0x10;
	break;

	case 3: Address = Column + 0x50;
	break;

	default: break;

	}

	LCD_sendCommand(0x80 + Address);

}

void LCD_displayStringRowColumn(uint8 Row, uint8 Column, uint8 * String)
{
	LCD_goToRowColumn(Row, Column);
	LCD_displayString(String);
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_SCREEN);
}

