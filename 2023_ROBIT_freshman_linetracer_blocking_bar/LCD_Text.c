#include "LCD_Text.h"

//------------------------------------------------------------------------------
// LCD 초기화 함수
//------------------------------------------------------------------------------
void lcdInit(void)
{
	_delay_ms(30);
	LCD_PORT_SETUP();
	LCD_PORT &= 0xFB;	//E = 0
	_delay_ms(15);
	
	lcdCommand(0x20);
	_delay_ms(5);
	
	lcdCommand(0x20);
	delay_us(200);
	
	lcdCommand(0x20);
	lcdCommand(FUNCSET);
	lcdCommand(DISPON);
	lcdCommand(ALLCLR);
	lcdCommand(ENTMODE);
	
	lcdString(0,0,"Init OK");
}

//------------------------------------------------------------------------------
// LCD 지우기 함수
//------------------------------------------------------------------------------
void lcdClear(void)
{
	lcdCommand(ALLCLR);
}


//------------------------------------------------------------------------------
// 문자열 출력 함수
//------------------------------------------------------------------------------
void lcdString(U8 line, U8 col, char  *str)
{
	char  *pStr = 0;
	
	lcdDisplayPosition( line, col );
	pStr = str;
	while(*pStr)
	{
		lcdData(*pStr++);
	}
}

//------------------------------------------------------------------------------
// 숫자 출력 함수
//------------------------------------------------------------------------------
void lcdNumber(U8 line, U8 col, int num)
{
	char byte[100] = {0, };
	
	sprintf(byte,"%d",num);
	
	lcdString(line, col, byte);
}




//------------------------------------------------------------------------------
// 명령 함수
//------------------------------------------------------------------------------
void lcdCommand(U8 byte)
{
	busy();
	//인스트럭션 상위 바이트
	LCD_PORT = (byte & 0xf0); //data
	LCD_PORT &= 0xfe;  //RS = 0
	LCD_PORT &= 0xfd;  //RW = 0
	delay_us(1);
	LCD_PORT |= 0x04;  //E = 1
	delay_us(1);
	LCD_PORT &= 0xfb;  //E = 0
	//instruction low byte
	LCD_PORT = ((byte<<4) & 0xf0); //data
	LCD_PORT &= 0xfe;  //RS = 0
	LCD_PORT &= 0xfd;  //RW = 0
	delay_us(1);
	LCD_PORT |= 0x04;  //E = 1
	delay_us(1);
	LCD_PORT &= 0xfb;  //E = 0
}
//------------------------------------------------------------------------------
// 데이타 쓰기 함수
//------------------------------------------------------------------------------
void lcdData(U8 byte)
{
	busy();
	//data high byte
	LCD_PORT = (byte & 0xF0); //data
	LCD_PORT |= 0x01;  //RS = 1
	LCD_PORT &= 0xfd;  //RW = 0
	delay_us(1);
	LCD_PORT |= 0x04;  //E = 1
	delay_us(1);
	LCD_PORT &= 0xfb;  //E = 0
	//data low byte
	LCD_PORT = ((byte<<4) & 0xF0); //data
	LCD_PORT |= 0x01;  //RS = 1
	LCD_PORT &= 0xfd;  //RW = 0
	delay_us(1);
	LCD_PORT |= 0x04;  //E = 1
	delay_us(1);
	LCD_PORT &= 0xfb;  //E = 0
}

//------------------------------------------------------------------------------
//                 Display Position Setting Function
//------------------------------------------------------------------------------
void lcdDisplayPosition( U8 line, U8 col )
{
	if(line == 0)		lcdCommand( LINE1 + col );
	else				lcdCommand( LINE2 + col );
}



//------------------------------------------------------------------------------
//                 Delay Function
//------------------------------------------------------------------------------
void busy(void)
{
	_delay_ms(2);
}

// 16Mhz
//	1 cycle = 0.0625 us
// 16 cycke = 1 us
void delay_us(U16 time_us)
{
	register U16 i;
	
	for(i = 0; i < time_us; i++)				/* 4 cycle +				*/
	{
		asm volatile(" PUSH  R0 ");				/* 2 cycle +				*/
		asm volatile(" POP   R0 ");				/* 2 cycle +				*/
		asm volatile(" PUSH  R0 ");				/* 2 cycle +				*/
		asm volatile(" POP   R0 ");				/* 2 cycle +				*/
		asm volatile(" PUSH  R0 ");				/* 2 cycle +				*/
		asm volatile(" POP   R0 ");				/* 2 cycle    =  16 cycle		*/
	}
}