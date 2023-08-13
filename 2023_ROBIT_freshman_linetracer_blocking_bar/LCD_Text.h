/*==============================================================================
 *
 *   CLCD
 *		LCD Driver		: KS0066U
 *
 *   	File Name   	: LCD_Text.h
 *  	Version        	: 2.0
 *    	Date           	: Dec 24, 2013
 *		Modified       	: Dec 9, 2016 By Kim Jihun
 *		Author         	: KimJihun  (ROBIT 9th, Kwangwoon University)
 *		MPU_Type       	: Atmega128 (16MHz)
 *		Compiler		: AtmelStudio6 6.1
 *		Copyright(c) 2016 ROBIT, Kwangwoon University.
 *    	All Rights Reserved.
 *
==============================================================================*/

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#ifndef __Text_Lcd_H
#define __Text_Lcd_H



//------------------------------------------------------------------------------
//                      Control, Data PORTX
//	Bit			7		6		5		4		3		2		1		0
//	LCD Pin		DB7		DB6		DB5		DB4		X		E		R/W		RS
//------------------------------------------------------------------------------
#define LCD_PORT			PORTC
#define LCD_PORT_SETUP()	DDRC = 0xFF

//Text Lcd 명령어 셋팅
#define FUNCSET		0x28	//Function set		0010 1000
#define ENTMODE		0x06	//Entry Mode Set	0000 0110
#define ALLCLR		0x01	//All Clear			0000 0001
#define DISPON		0x0c	//Display On		0000 1100
#define LINE1		0x80	//1st line Move		1000 0000
#define LINE2		0xc0	//2nd line Move		1100 0000

#define U8 unsigned char
#define U16 unsigned int



//------------------------------------------------------------------------------
//                       ===== Funtion Prototype =====
//------------------------------------------------------------------------------
void lcdInit(void);
void lcdClear(void);
void lcdString(U8 line, U8 col, char  *str);
void lcdNumber(U8 line, U8 col, int num);




void lcdCommand(U8 byte);
void lcdData(U8 byte);
void lcdDisplayPosition( U8 line, U8 col );

void Busy(void);
void delay_us(U16 time_us);

#endif