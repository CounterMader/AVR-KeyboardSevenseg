/*
 * Keyboard_7segment.c
 *
 * Created: 7/12/2021 11:39:10 PM
 * Author : ESMAEILY
 
      D0 D1 D3
	--|--|--|--D4 
	--|--|--|--D5
	--|--|--|--D6
	--|--|--|--D7
	
	PORTA0 -> a
	PORTA1 -> b
	PORTA2 -> c
	PORTA3 -> d
	PORTA4 -> e
	PORTA5 -> f
	PORTA6 -> g
	PORTA7 -> dp
	
	PORTC0 -> 7seg0
	PORTC1 -> 7seg1
	PORTC2 -> 7seg2
	PORTC3 -> 7seg3
 */ 

#ifndef F_CPU
#define F_CPU 1000000UL
#endif
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#define TRUE 1
#define FALSE 0
#define NO_KEY_CODE 12

//Global Variables
volatile const char seg_table[] PROGMEM = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
										   0x92, 0x82, 0xF8, 0x80, 0x90};
volatile const char key_table[] PROGMEM = {};
volatile char disp_data[] = {11, 11, 11, 11};//Display initialize (- - - -)
volatile char key_code=NO_KEY_CODE;	
int main(void)
{
	

    while (1) 
    {
    }
}

ISR(TIMER0_COMP_vect){
	static char x20 = 0;
	char temp, i;
	if (++x20 == 20){
		DDRD |= (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6);
		PORTD |= (1 << PD0) | (1 << PD1) | (1 << PD2);
		_delay_us(5);//signal stabilization
		temp = (PIND & 0x07);
		DDRD = (1 << DDD0) | (1 << DDD1) | (1 << DDD2);
		PORTD = (1 << PD3) | (1 << PD4) | (1 << PD5) | (1 << PD6);
		_delay_us(5);
		temp |= (PIND & 0x78);
		for(i=0;i<=12;i++){
			if(temp == key_table[i]){
				key_code = i;
				break;
			}
			else{
				key_code = NO_KEY_CODE;
			}
		}
	}
	
	
}