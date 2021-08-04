/*
 * Keyboard_7segment.c
 *
 * Created: 7/12/2021 11:39:10 PM
 * Author : ESMAEILY
 * Chip   : ATmega32
      D0 D1 D2
	--|--|--|--D3 
	--|--|--|--D4
	--|--|--|--D5
	--|--|--|--D6
	
	PORTA0 -> a
	PORTA1 -> b
	PORTA2 -> c
	PORTA3 -> d
	PORTA4 -> e
	PORTA5 -> f
	PORTA6 -> g
	PORTA7 -> dp
	
	PORTB0 -> 7seg0
	PORTB1 -> 7seg1
	PORTB2 -> 7seg2
	PORTB3 -> 7seg3
 */ 

#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#define TRUE 1
#define FALSE 0
#define NO_KEY_CODE 12

//Global Variables
unsigned const char seg_table[] = {0xF9, 0xA4, 0xB0, 0x99, 0x92,
								   0x82, 0xF8, 0x80, 0x90, 0xC0,
								   0xFF, 0xBF};
unsigned const char key_table[] = {0x76, 0x75, 0x73,
								   0x6E, 0x6D, 0x6B,
								   0x5E, 0x5D, 0x5B,
								   0x3E, 0x3D, 0x3B};
volatile unsigned char disp_data[] = {11, 11, 11, 11};//Display initialize (- - - -)
volatile unsigned char key_code=NO_KEY_CODE;	

int main(void)
{ 
	//Variables
	unsigned char old_key_code = NO_KEY_CODE,temp_key_code;
	unsigned char i = 4;
	//7 segment port configuration
	DDRA |= (1 << DDA0) | (1 << DDA1) | (1 << DDA2) | (1 << DDA3) | (1 << DDA4) | (1 << DDA5) | (1 << DDA6) | (1 << DDA7);
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB3);
	//Timer/Counter configuration
	OCR0 = 124;
	TIMSK = (1 << OCIE0);//Enable compare match  interrupt
	TCCR0 = (1 << WGM01) | (1 << CS01) ;//Timer clock:fclk_io/8 ; mode:CTC
	sei();
    while (1)
    {
		temp_key_code = key_code;
		if(temp_key_code != old_key_code){
			old_key_code = temp_key_code;
			if(temp_key_code != NO_KEY_CODE){
				disp_data[i] = temp_key_code;
				if (--i == 0xFF) i=4;
			}
		}
    }
}

ISR(TIMER0_COMP_vect){
	static unsigned char x20 = 0;
	static unsigned char pointer = 0;
	unsigned char temp, i;
	//7 segment refresh routine
	PORTB = 0;//Turn off all segments
	PORTA = seg_table[disp_data[pointer]];
	PORTB = (1 << pointer);
	if(++pointer == 4) pointer = 0;
	//Keyboard scan routine
	if (++x20 == 20){
		x20 = 0;
		DDRD = (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6);
		PORTD = (1 << PD0) | (1 << PD1) | (1 << PD2);
		_delay_us(5);//signal stabilization
		temp = (PIND & 0x07);
		DDRD = (1 << DDD0) | (1 << DDD1) | (1 << DDD2);
		PORTD = (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6);
		_delay_us(5);
		temp |= (PIND & 0x78);
		for(i=0;i<12;i++){
			if(temp == key_table[i]){
				key_code = i;
				break;
			}
			else key_code = NO_KEY_CODE;
		}
	}
}
