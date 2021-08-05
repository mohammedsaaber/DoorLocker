/*
 * LCD.h
 *
 *  Created on: Jul 29, 2021
 *      Author: MohamedSaber
 */

#ifndef LCD_H_
#define LCD_H_


#define LCD_CONTROL_PORT_DIRECTION DDRB /* RS, RW, E PINS */
#define LCD_CONTROL_PORT_OUTPUT PORTB
#define LCD_DATA_PORT_DIRECTION DDRC /* D0--D7 PINS */
#define LCD_DATA_PORT_OUTPUT PORTC

#define RS PB0
#define RW PB1
#define E PB2

#define CLEAR_SCREEN 0x01
#define DISPLAY_ON_CURSOR_OFF 0x0C
#define RETURN_HOME 0x02

#include <util/delay.h>
#include "std_types.h"

void LCD_init(void);

void LCD_sendCommand(uint8 Command);

void LCD_displayCharacter(uint8 Character);

void LCD_displayString(uint8 * String);

void LCD_goToRowColumn(uint8 Row, uint8 Column);

void LCD_displayStringRowColumn(uint8 Row, uint8 Column, uint8 * String);

void LCD_clearScreen(void);




#endif /* LCD_H_ */
