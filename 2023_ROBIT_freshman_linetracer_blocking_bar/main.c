/*
* 2023_robit_linetracer_blockgin_bar.c
*
* Created: 2023-08-11 오전 11:59:58
* Author : 0311b
*/
#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD_Text.h"

void drive_dynamixel(int id,int spd,int pos);
void uart0_tx(unsigned char data);
void uart0_init();
void adc_init();
void timer0_init();
int adc_val(int num);

int main(void)
{
	/* Replace with your application code */
	adc_init();
	uart0_init();
	timer0_init();
	
	lcdInit();
	lcdClear();
	DDRA=0xFF;
	drive_dynamixel(4,0,512);
	_delay_ms(1000);
	sei();
	
	while (1)
	{
		
	}
}
void drive_dynamixel(int id,int spd,int pos){
	unsigned char packet[11];
	packet[0]=0xFF;
	packet[1]=0xFF;
	packet[2]=id&0xFF;
	packet[3]=7;
	packet[4]=3;
	packet[5]=30;
	packet[6]=pos&0xFF;
	packet[7]=(pos>>8)&0xFF;
	packet[8]=spd&0xFF;
	packet[9]=(spd>>8)&0xFF;
	packet[10]=0;
	for(int i=2;i<10;i++){
		packet[10]+=packet[i];
	}
	packet[10]=~packet[10];
	for(int i=0;i<11;i++){
		uart0_tx(packet[i]);
		_delay_ms(2);
	}
}

void adc_init(){
	DDRF=0x00;
	ADMUX=0x40;
	ADCSRA=0x87;
}
int adc_val(int num){
	ADMUX=0x40|num;
	ADCSRA|=0x40;
	while((ADCSRA&0x10)==0);
	return ADC;
}

void uart0_init(){
	
	//ATMEGA128 DYNAMIXEL
	UBRR0L = 1;  //0.5M bps
	UBRR0H = 0;
	UCSR0A = 0x20;//송신, 수신 상태비트 초기화
	UCSR0B = 0x18;//송신, 수신 기능 활성화//인터럽트 활성화X
	UCSR0C = 0x06;//start 1비트/ data 8비트/stop 1비트
}
void uart0_tx(unsigned char data){
	while(!(UCSR0A & (1 << 5)));
	UDR0 = data;
}
void timer0_init(){
	
	TCNT0 = 0x06; //1ms 만들기 위한 시스템 타이머 레지스터 설정
	TCCR0 = 0x04; // freescale 64배수
	TIMSK = 0x01; // 오버플로우 인터럽트 사용
	TCNT0 = 6;	  // 256-6 = 250 count = 1ms
}
ISR(TIMER0_OVF_vect)
{
	TCNT0 = 6;
	static int bar_time=0;
	static int sleep_time=0;
	if(sleep_time>0){
		sleep_time--;
	}
	else if(bar_time>0){
		bar_time--;
		if(bar_time==0){
			drive_dynamixel(4,0,512);
			sleep_time=10000;
		}
	}
	else {
		static int time_1ms=0;
		time_1ms++;
		if(time_1ms>10){
			int psd_val=adc_val(0);
			if(psd_val>300&&psd_val<600){
				drive_dynamixel(4,0,512-310);
				bar_time=(psd_val%90)*100+1000;	//1~10초 사이
			}
			time_1ms=0;
		}
	}
	
}