#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include "uart.h"


int main(void) {

	DDRD = 0xFF;

	PORTD &= ~(1<<PD7);
	PORTD &= ~(1<<PD6);
	PORTD &= ~(1<<PD5);
	PORTD &= ~(1<<PD4);

	while(1) {

		PORTD |= (1<<PD7);
		PORTD |= (1<<PD6);
		PORTD &= ~(1<<PD5);
		PORTD &= ~(1<<PD4);

		_delay_ms(250);

		PORTD |= (1<<PD6);
		PORTD |= (1<<PD5);
		PORTD &= ~(1<<PD4);
		PORTD &= ~(1<<PD7);

		_delay_ms(250);

		PORTD |= (1<<PD5);
		PORTD |= (1<<PD4);
		PORTD &= ~(1<<PD6);
		PORTD &= ~(1<<PD7);

		_delay_ms(250);

		PORTD |= (1<<PD4);
		PORTD |= (1<<PD7);
		PORTD &= ~(1<<PD6);
		PORTD &= ~(1<<PD5);

		_delay_ms(250);

	}

}