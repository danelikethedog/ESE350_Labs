#include <avr/io.h> 
#include <avr/interrupt.h>
#include "util/delay.h" 
#include <stdio.h>
#include "uart.h"

int main(void) {


	DDRD = (1<<PD6);

	uart_init();

	TCCR0A = ((1<<COM0A1)|(1<<WGM01)|(1<<WGM00));

	TCCR0B = ((1<<CS02)|(1<<CS00));

	sei();


	while(1) {

		OCR0A = 10;
		_delay_ms(2000);


		OCR0A = 50;
		_delay_ms(2000);

	}

}